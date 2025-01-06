#include "render_process.hpp"
#include "shader.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <iostream>

namespace vkl {

RendeProcess::RendeProcess(vk::Device& logicalDevice, vkl::Shader& shader, int width, int height) 
    : logicalDevice(logicalDevice), shader(shader)
{
    InitPipeline(width, height);
    InitPipelineLayout();
}

RendeProcess::~RendeProcess() {
    logicalDevice.destroyPipelineLayout(layout);
}

void RendeProcess::InitPipeline(int width, int height) {
    vk::GraphicsPipelineCreateInfo createInfo{};

    // vertex Input
    vk::PipelineVertexInputStateCreateInfo inputState{};
    createInfo.setPVertexInputState(&inputState);

    // vertex assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAsm{};
    inputAsm.setPrimitiveRestartEnable(false)
            .setTopology(vk::PrimitiveTopology::eTriangleList);
    createInfo.setPInputAssemblyState(&inputAsm);

    // shader
    auto stages = shader.getStage();
    createInfo.setStages(stages);

    // viewport
    vk::PipelineViewportStateCreateInfo viewportState{};
    vk::Viewport viewport(0, 0, width, height, 0, 1);
    viewportState.setViewports(viewport);
    vk::Rect2D rect{{0, 0}, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}};
    viewportState.setScissors(rect);
    createInfo.setPViewportState(&viewportState);

    // rasterization
    vk::PipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.setDepthClampEnable(vk::False)
              .setRasterizerDiscardEnable(vk::False)
              .setPolygonMode(vk::PolygonMode::eFill)
              .setLineWidth(1.0f)
              .setCullMode(vk::CullModeFlagBits::eBack)
              .setFrontFace(vk::FrontFace::eCounterClockwise)
              .setDepthBiasEnable(vk::False)
              .setDepthBiasConstantFactor(0.0f)
              .setDepthBiasClamp(0.0f)
              .setDepthBiasSlopeFactor(0.0f);

    // multi-sample
    vk::PipelineMultisampleStateCreateInfo multisampleInfo{};
    multisampleInfo.setSampleShadingEnable(vk::False)
                   .setRasterizationSamples(vk::SampleCountFlagBits::e1);
    createInfo.setPMultisampleState(&multisampleInfo);

    // colorBlending
    vk::PipelineColorBlendStateCreateInfo blendInfo{};
    vk::PipelineColorBlendAttachmentState attachs{};
    attachs.setBlendEnable(false)
           .setColorWriteMask(vk::ColorComponentFlagBits::eA |
                              vk::ColorComponentFlagBits::eB |
                              vk::ColorComponentFlagBits::eG |
                              vk::ColorComponentFlagBits::eR);
    blendInfo.setLogicOpEnable(false)
             .setAttachments(attachs);
    createInfo.setPColorBlendState(&blendInfo);

    auto result = logicalDevice.createGraphicsPipeline(nullptr, createInfo);
    if(result.result != vk::Result::eSuccess) {
        std::cout << "create graphics pipeline failed\n";
        exit(-1);
    }

    pipeline = result.value;
}

void RendeProcess::InitPipelineLayout() {
    vk::PipelineLayoutCreateInfo createInfo{};
    layout = logicalDevice.createPipelineLayout(createInfo);
}

void RendeProcess::DestroyPipeline() {
    logicalDevice.destroyPipeline(pipeline);
}

}