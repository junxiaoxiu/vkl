#pragma once
#include "vulkan/vulkan.hpp"
#include "tool.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace vkl {

struct SwapChain final {
private:
    // all device support 
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    SwapChainSupportDetails swapchainDetails{}; 

public:
    // actual use swapchain Configure
    struct SwapchainInfo {
        uint32_t imageCount;
        vk::Extent2D imageExtent; 
        vk::PresentModeKHR present;
        vk::SurfaceFormatKHR format;
        vk::SurfaceTransformFlagBitsKHR transform;
    };
    SwapchainInfo info{};

    void createSwapchain(vk::PhysicalDevice& phyDevice, vk::Device& logicalDevice, vk::SurfaceKHR& surface, vkl::QueueFamilyIndices& indices, uint32_t width, uint32_t height);
    bool querySwapChainSupport(vk::PhysicalDevice& phyDevice, vk::SurfaceKHR& surface);
    vk::SurfaceFormatKHR chooseSwapchainSurfaceFormat();
    vk::PresentModeKHR chooseSwapchainPresentModeKHR();
    vk::Extent2D chooseSwapExtent(uint32_t width, uint32_t height);
    uint32_t chooseImageCount();

    int width, height;
    vk::SwapchainKHR swapchain{};
    std::vector<vk::Image> swapChainImages;
};

}