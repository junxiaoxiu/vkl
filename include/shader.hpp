#pragma once

#include "vulkan/vulkan.hpp"

namespace vkl {

class Shader {
public:
    Shader(vk::Device& logicalDevice, const std::string& vertexShader, const std::string& fragShader);
    ~Shader();
    void InitShader(const std::string& vertexShader, const std::string& fragShader);
    void InitStage();
    std::vector<vk::PipelineShaderStageCreateInfo> getStage();

private:
    vk::Device& logicalDevice;
    std::string vertexShader;
    std::string fragShader;

    vk::ShaderModule vertexModule;
    vk::ShaderModule fragModule;
    std::vector<vk::PipelineShaderStageCreateInfo> stage;
};

}