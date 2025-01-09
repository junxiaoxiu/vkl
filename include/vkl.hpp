#pragma once

#include "vulkan/vulkan.hpp"
#include "vector"
#include <memory>
#include <optional>
#include <random>
#include "tool.hpp"
#include "swapchain.hpp"
#include "shader.hpp"
#include "render_process.hpp"

namespace vkl {

class Vklapp {
public:
    void init(CreateSurfaceFunc func, int width, int height);
    void run();
    void quit();

    void createInstance();
    void createSurface(CreateSurfaceFunc func);
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool isSuitableDevice(vk::PhysicalDevice device);
    bool checkDeviceExtensionsSupport(vk::PhysicalDevice device);
    void findQueueFamilys(vk::PhysicalDevice device);
    void getQueues();

    // arguments main function pass on to Vklapp
    struct Arguments {
        std::vector<const char*> extensions;
        std::vector<const char*> deviceExteneisons;
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
    };
    Arguments arguments;

    vk::Instance instance{};
    vk::PhysicalDevice physicalDevice{};
    vk::Device logicalDevice{};
    vk::Queue graphicsQueue{};
    vk::Queue presentQueue{};
    vk::SurfaceKHR surface{};
    vkl::SwapChain swapchain{};
    std::unique_ptr<vkl::Shader> shader_;
    std::unique_ptr<vkl::RendeProcess> renderProcess;
    QueueFamilyIndices queueFamilyIndices{};

};

}