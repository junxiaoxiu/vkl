#pragma once
#include "vkl.hpp"
#include "vulkan/vulkan.hpp"
#include <array>
#include <cstdint>
#include <iostream>

namespace vkl {

void Vklapp::init(int width, int height) {
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
    swapchain.createSwapchain(physicalDevice, logicalDevice, surface, queueFamilyIndices, width, height);
}

void Vklapp::quit() {
    logicalDevice.destroySwapchainKHR(swapchain.swapchain);
    logicalDevice.destroy();
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

    vk::InstanceCreateInfo createInfo{};
    createInfo.setPApplicationInfo(&appInfo)
               .setPEnabledLayerNames(arguments.extensionLayers)
               .sType = vk::StructureType::eInstanceCreateInfo;

    instance = vk::createInstance(createInfo);
}

void Vklapp::pickPhysicalDevice() {
    auto devices= instance.enumeratePhysicalDevices();
    if(devices.empty()) {
        std::cout << "no suitable PhysicalDevice\n";
    }

    for(const auto& device : devices) {
        if(isSuitableDevice(device)) {
            physicalDevice = device;
            std::cout << "pick device: " << device.getProperties().deviceName;
            break;
        }
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
    auto availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

    // judge if this phyDevice support all needed extensions 
    uint32_t satisfyExtensionsCnt = 0;
    for(const auto& availableExtension : availableExtensions) {
        if(std::find(arguments.deviceExteneisons.begin(),
                     arguments.deviceExteneisons.end(),
                     availableExtension.extensionName.data()) != arguments.deviceExteneisons.end())
            satisfyExtensionsCnt += 1;
    }

    return satisfyExtensionsCnt == arguments.deviceExteneisons.size();
}

void Vklapp::createLogicalDevice() {
    findQueueFamilys(physicalDevice);

    std::array extensions = {vk::KHRSwapchainExtensionName};
    float queuePriority = 1.0f;

    vk::DeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
    queueCreateInfo.setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value())
                   .setQueueCount(1)
                   .setPQueuePriorities(&queuePriority);

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo;
    createInfo.sType = vk::StructureType::eDeviceCreateInfo;
    createInfo.setPQueueCreateInfos(&queueCreateInfo)
              .setQueueCreateInfoCount(1)
              .setPEnabledFeatures(&deviceFeatures)
              .setEnabledExtensionCount(arguments.deviceExteneisons.size())
              .setPEnabledExtensionNames(arguments.deviceExteneisons);
    
    logicalDevice = physicalDevice.createDevice(createInfo);

    logicalDevice.getQueue(queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
}

}