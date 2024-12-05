#pragma once

#include "vulkan/vulkan.hpp"
#include "vector"
#include "vulkan/vulkan_handles.hpp"
#include <optional>

namespace vkl {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

class Vklapp {
public:
    void init();
    void run();
    void quit();

    void createInstance();
    void pickPhysicalDevice();
    bool isSuitableDevice(vk::PhysicalDevice device);
    QueueFamilyIndices findQueueFamilys(vk::PhysicalDevice device);

// arguments main function pass on to Vklapp
struct Arguments {
    std::vector<const char*> extensionLayers;
};
Arguments arguments;


private:
    vk::Instance instance{};
    vk::PhysicalDevice physicalDevice{};

};

}