#include "vkl.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <array>
#include <cstdint>
#include <iostream>

namespace vkl {

void Vklapp::init() {
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
}

void Vklapp::quit() {
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

void Vklapp::createLogicalDevice() {
    vkl::QueueFamilyIndices indices = findQueueFamilys(physicalDevice);

    std::array extensions = {vk::KHRSwapchainExtensionName};
    float queuePriority = 1.0f;

    vk::DeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
    queueCreateInfo.setQueueFamilyIndex(indices.graphicsFamily.value())
                   .setQueueCount(1)
                   .setPQueuePriorities(&queuePriority);

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo;
    createInfo.sType = vk::StructureType::eDeviceCreateInfo;
    createInfo.setPQueueCreateInfos(&queueCreateInfo)
              .setQueueCreateInfoCount(1)
              .setPEnabledFeatures(&deviceFeatures)
              .setEnabledExtensionCount(0)
              .setEnabledExtensionCount(extensions.size())
              .setPEnabledExtensionNames(extensions);
    
    logicalDevice = physicalDevice.createDevice(createInfo);

    logicalDevice.getQueue(indices.graphicsFamily.value(), 0, &graphicsQueue);
}

bool Vklapp::isSuitableDevice(vk::PhysicalDevice device) {
    vkl::QueueFamilyIndices indices = findQueueFamilys(device);
    return indices.isComplete();
}

QueueFamilyIndices Vklapp::findQueueFamilys(vk::PhysicalDevice physicalDevice) {
    vkl::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    auto queueFamilys = physicalDevice.getQueueFamilyProperties();

    for(int i = 0; i < queueFamilys.size(); i++) {
        if(queueFamilys[i].queueCount > 0 && queueFamilys[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }
        if(indices.isComplete()) {
            break;
        }
    }
    return indices;
}

}