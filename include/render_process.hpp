#pragma once
#include "shader.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace vkl {

class RendeProcess {
public:
    vk::Pipeline pipeline;
    vk::PipelineLayout layout;
    vk::RenderPass renderpass;

    RendeProcess(vk::Device& logicalDevice, vk::Format& format, vkl::Shader& shader, int width, int height);
    ~RendeProcess();
    void InitPipeline(int width, int height);
    void InitPipelineLayout();
    void createRenderPass(vk::Format& format);
    void DestroyPipeline();

    vk::Device& logicalDevice;
    vkl::Shader& shader;
};

}
