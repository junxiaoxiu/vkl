#pragma once
#include "vkl.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan_enums.hpp"
#include <cstdint>
#include <iostream>

namespace vkl {

void Vklapp::init(CreateSurfaceFunc func, int width, int height) {
    createInstance();
    createSurface(func);
    pickPhysicalDevice();
    createLogicalDevice();
    getQueues(); 
    swapchain.createSwapchain(physicalDevice, logicalDevice, surface, queueFamilyIndices, width, height);
}

void Vklapp::quit() {
    logicalDevice.destroySwapchainKHR(swapchain.swapchain);
    logicalDevice.destroy();
    instance.destroySurfaceKHR(surface);
    instance.destroy();
}

void Vklapp::createInstance() {
    vk::ApplicationInfo appInfo{};
    appInfo.setPApplicationName("hello vulkan")
           .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
           .setPEngineName("No Engine")
           .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
           .setApiVersion(VK_API_VERSION_1_3)
           .setPNext(nullptr)
           .sType = vk::StructureType::eApplicationInfo;

#ifdef _DEBUG
    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};
#endif

    vk::InstanceCreateInfo createInfo{};
    createInfo.setPApplicationInfo(&appInfo)
#ifdef _DEBUG
              .setPEnabledLayerNames(layers)
#endif
              .setPEnabledExtensionNames(arguments.extensions);

    instance = vk::createInstance(createInfo);
}

void Vklapp::createSurface(CreateSurfaceFunc func) {
    surface = func(instance);
}

void Vklapp::pickPhysicalDevice() {
    auto devices= instance.enumeratePhysicalDevices();
    if(devices.empty()) {
        std::cout << "error: no suitable PhysicalDevice\n";
        exit(-1);
    }

    // physicalDevice = devices[0];
    // std::cout << "pick device: " << devices[0].getProperties().deviceName << '\n';
    for(const auto& device : devices) {
        if(isSuitableDevice(device)) {
            physicalDevice = device;
            std::cout << "pick device: " << device.getProperties().deviceName;
            break;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE) {
        std::cout << "error: physical device pick up failed\n";
        exit(-1);
    }
}

bool Vklapp::isSuitableDevice(vk::PhysicalDevice phyDevice) {
    // 1、 support eGraphics
    findQueueFamilys(phyDevice);
    // 2、 support swapchain
    bool supportExtension = checkDeviceExtensionsSupport(phyDevice);
    // 3、 support image format & presentation
    bool swapchainAdequate = swapchain.querySwapChainSupport(phyDevice, surface);

    return queueFamilyIndices.isComplete() && supportExtension && swapchainAdequate;
}

void Vklapp::findQueueFamilys(vk::PhysicalDevice physicalDevice) {
    uint32_t queueFamilyCount = 0;
    auto queueFamilys = physicalDevice.getQueueFamilyProperties();

    for(int i = 0; i < queueFamilys.size(); i++) {
        // judge if this phyDevice support geometry shader
        if(queueFamilys[i].queueCount > 0 && queueFamilys[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            queueFamilyIndices.graphicsFamily = i;
        }
        if(physicalDevice.getSurfaceSupportKHR(i, surface)) {
            queueFamilyIndices.presentFamily = i;
        }
        if(queueFamilyIndices.isComplete()) {
            break;
        }
    }
}

bool Vklapp::checkDeviceExtensionsSupport(vk::PhysicalDevice phyDevice) {
    uint32_t count = 0;
    std::vector<VkExtensionProperties> availableExtensions;
    vkEnumerateDeviceExtensionProperties(phyDevice, nullptr, &count, nullptr);
    availableExtensions.resize(count);
    vkEnumerateDeviceExtensionProperties(phyDevice, nullptr, &count, availableExtensions.data());
    
    // program will crash if call api device.enumerateDeviceExtensionProperties(), don't know why
    // so use the api vkEnumerateDeviceExtensionProperties()
    // auto availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

    std::vector<std::string> deviceExtensions(arguments.deviceExteneisons.begin(), arguments.deviceExteneisons.end());

    // judge if this phyDevice support all needed extensions 
    uint32_t satisfyExtensionsCnt = 0;
    for(const auto& availableExtension : availableExtensions) {
        if(std::find(deviceExtensions.begin(),
                     deviceExtensions.end(),
                     availableExtension.extensionName) != deviceExtensions.end())
            satisfyExtensionsCnt += 1;
    }

    return satisfyExtensionsCnt == arguments.deviceExteneisons.size();
}

void Vklapp::createLogicalDevice() {
    findQueueFamilys(physicalDevice);

    float queuePriority = 1.0f;

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    
    if(queueFamilyIndices.graphicsFamily.value() == queueFamilyIndices.presentFamily.value()) {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value())
                       .setQueueCount(1)
                       .setPQueuePriorities(&queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }else {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value())
                       .setQueueCount(1)
                       .setPQueuePriorities(&queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
        queueCreateInfo.setQueueFamilyIndex(queueFamilyIndices.presentFamily.value())
                       .setQueueCount(1)
                       .setPQueuePriorities(&queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo;
    createInfo.setPQueueCreateInfos(queueCreateInfos.data())
              .setQueueCreateInfoCount(1)
              .setPEnabledFeatures(&deviceFeatures)
              .setEnabledExtensionCount(arguments.deviceExteneisons.size())
              .setPEnabledExtensionNames(arguments.deviceExteneisons);
    
    logicalDevice = physicalDevice.createDevice(createInfo);
}

void Vklapp::getQueues() {
    graphicsQueue = logicalDevice.getQueue(queueFamilyIndices.graphicsFamily.value(), 0);
    presentQueue = logicalDevice.getQueue(queueFamilyIndices.presentFamily.value(), 0);
}
}