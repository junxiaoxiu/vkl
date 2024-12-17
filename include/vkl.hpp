#pragma once

#include "vulkan/vulkan.hpp"
#include "vector"
#include <optional>
#include "tool.hpp"
#include "swapchain.hpp"

namespace vkl {

class Vklapp {
public:
    void init(int width, int height);
    void run();
    void quit();

    void createInstance();
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool isSuitableDevice(vk::PhysicalDevice device);
    bool checkDeviceExtensionsSupport(vk::PhysicalDevice device);
    void findQueueFamilys(vk::PhysicalDevice device);

    // arguments main function pass on to Vklapp
    struct Arguments {
        std::vector<const char*> extensionLayers;
        std::vector<const char*> deviceExteneisons;
    };
    Arguments arguments;

    vk::Instance instance{};
    vk::PhysicalDevice physicalDevice{};
    vk::Device logicalDevice{};
    vk::Queue graphicsQueue{};
    vk::SurfaceKHR surface{};
    vkl::SwapChain swapchain{};
    QueueFamilyIndices queueFamilyIndices{};

};

}