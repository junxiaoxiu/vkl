#include "swapchain.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>

namespace vkl {

bool SwapChain::querySwapChainSupport(vk::PhysicalDevice& phyDevice, vk::SurfaceKHR& surface) {
    swapchainDetails.capabilities = phyDevice.getSurfaceCapabilitiesKHR(surface);
    swapchainDetails.formats      = phyDevice.getSurfaceFormatsKHR(surface);
    swapchainDetails.presentModes = phyDevice.getSurfacePresentModesKHR(surface);

    return !swapchainDetails.formats.empty() && !swapchainDetails.presentModes.empty();
}

void SwapChain::createSwapchain(vk::PhysicalDevice& phyDevice, vk::Device& logicalDevice, vk::SurfaceKHR& surface, vkl::QueueFamilyIndices& indices, uint32_t width, uint32_t height) {
    querySwapChainSupport(phyDevice, surface);
    info.format = chooseSwapchainSurfaceFormat();
    info.present = chooseSwapchainPresentModeKHR();
    info.imageExtent = chooseSwapExtent(width, height);
    info.imageCount = chooseImageCount();

    vk::SwapchainCreateInfoKHR createInfo{};
    createInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
    createInfo.setSurface(surface)
              .setMinImageCount(info.imageCount)
              .setImageFormat(info.format.format)
              .setImageColorSpace(info.format.colorSpace)
              .setImageExtent(info.imageExtent)
              .setImageArrayLayers(1)
              .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
    uint32_t queueFamilyIndices[] = {
        static_cast<uint32_t>(indices.graphicsFamily.value()),
        static_cast<uint32_t>(indices.presentFamily.value()),
    };
    if(indices.graphicsFamily.value() != indices.presentFamily.value()) {
        createInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
                  .setQueueFamilyIndexCount(2)
                  .setPQueueFamilyIndices(queueFamilyIndices);
    } else {
        createInfo.setImageSharingMode(vk::SharingMode::eExclusive)
                  .setQueueFamilyIndexCount(0)
                  .setPQueueFamilyIndices(nullptr);
    }
    createInfo.setPreTransform(swapchainDetails.capabilities.currentTransform)
              .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
              .setPresentMode(info.present)
              .setClipped(vk::True)
              .setOldSwapchain(VK_NULL_HANDLE);
    
    swapchain = logicalDevice.createSwapchainKHR(createInfo);

    swapChainImages = logicalDevice.getSwapchainImagesKHR(swapchain);
}

vk::SurfaceFormatKHR SwapChain::chooseSwapchainSurfaceFormat() {
    if(swapchainDetails.formats.size() == 1 && swapchainDetails.formats[0].format == vk::Format::eUndefined) {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear};
    }

    for(const auto& availableFormat : swapchainDetails.formats) {
      if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
          availableFormat.colorSpace == vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear) {
        return availableFormat;
      }
    }

    return swapchainDetails.formats[0];
}

vk::PresentModeKHR SwapChain::chooseSwapchainPresentModeKHR() {
    vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;
    for(const auto& availablePresentMode : swapchainDetails.presentModes) {
        if(availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        } else if(availablePresentMode == vk::PresentModeKHR::eImmediate) {
            bestMode = availablePresentMode;
        }
    }
    return bestMode;
}

vk::Extent2D SwapChain::chooseSwapExtent(uint32_t width, uint32_t height) {
    auto& capabilities = swapchainDetails.capabilities;
    if(swapchainDetails.capabilities.maxImageExtent != std::numeric_limits<uint32_t>::max()) {
        return swapchainDetails.capabilities.currentExtent;
    } else {
        vk::Extent2D actualExtent{width, height};
        actualExtent.width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, height));

        return actualExtent;
    }
}

uint32_t SwapChain::chooseImageCount() {
    uint32_t imageCount = swapchainDetails.capabilities.minImageCount + 1;
    if(swapchainDetails.capabilities.maxImageCount > 0 && imageCount > swapchainDetails.capabilities.maxImageCount) {
        imageCount = swapchainDetails.capabilities.maxImageCount;
    }
    return imageCount;
}

void SwapChain::createImageViews(vk::Device& logicalDevice) {
    swapChainImageViews.resize(swapChainImages.size());

    for(size_t i = 0; i < swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo{};
        vk::ComponentMapping mapping{};
        vk::ImageSubresourceRange range{};

        range.setAspectMask(vk::ImageAspectFlagBits::eColor)
             .setBaseMipLevel(0)
             .setLevelCount(1)
             .setBaseArrayLayer(0)
             .setLayerCount(1);
        createInfo.setImage(swapChainImages[i])
                  .setViewType(vk::ImageViewType::e2D)
                  .setFormat(info.format.format)
                  .setComponents(mapping)
                  .setSubresourceRange(range);
        swapChainImageViews[i] = logicalDevice.createImageView(createInfo);
    }
}

void SwapChain::createFramebuffers(vkl::RendeProcess& renderProcess, vk::Device& logicalDevice) {
    framebuffers.resize(swapChainImageViews.size());
    for(int i = 0; i < swapChainImageViews.size();i++) {
        vk::FramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.setAttachments(swapChainImageViews[i])
                             .setWidth(width)
                             .setHeight(height)
                             .setRenderPass(renderProcess.renderpass)
                             .setLayers(1);
        framebuffers[i] = logicalDevice.createFramebuffer(framebufferCreateInfo);
    }
}

void SwapChain::destroyImageViews(vk::Device &logicalDevice) {
    for(auto& framebuffer : framebuffers) {
        logicalDevice.destroyFramebuffer(framebuffer);
    }

    for(auto& view : swapChainImageViews) {
        logicalDevice.destroyImageView(view);
    }

    logicalDevice.destroySwapchainKHR(swapchain);
}
}