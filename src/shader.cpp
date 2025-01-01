#include <fstream>
#include <iostream>
#include "shader.hpp"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace vkl {

static std::string readFile(const std::string& filename) {
    std::string fileContent;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(!file.is_open()) {
        std::cout << "read" << filename << "failed" << std::endl;
        return {};
    }

    size_t fileSize = file.tellg();
    fileContent.resize(fileSize);
    file.seekg(0);
    file.read(fileContent.data(), fileContent.size());
    file.close();

    return fileContent;
}

void Shader::InitShader(const std::string &vertexShader, const std::string &fragShader) {
    auto vertexShaderCode = readFile(vertexShader);
    auto fragShaderCode = readFile(fragShader);

    vk::ShaderModuleCreateInfo createInfo{};
    createInfo.setCodeSize(vertexShaderCode.size())
              .setPCode((uint32_t*)vertexShaderCode.data());
    vertexModule = logicalDevice.createShaderModule(createInfo);

    createInfo.setCodeSize(fragShaderCode.size())
              .setPCode((uint32_t*)fragShaderCode.data());
    fragModule = logicalDevice.createShaderModule(createInfo);
}

void Shader::InitStage() {
    stage.resize(2);
    stage[0].setStage(vk::ShaderStageFlagBits::eVertex)
            .setModule(vertexModule)
            .setPName("main");

    stage[1].setStage(vk::ShaderStageFlagBits::eFragment)
            .setModule(fragModule)
            .setPName("main");
}

std::vector<vk::PipelineShaderStageCreateInfo> Shader::getStage() {
    return stage;
}

Shader::Shader(vk::Device &logicalDevice, const std::string& vertexShader, const std::string& fragShader)
: logicalDevice(logicalDevice)
{
    InitShader(vertexShader, fragShader);
}

Shader::~Shader() {
    logicalDevice.destroyShaderModule(vertexModule);
    logicalDevice.destroyShaderModule(fragModule);
}

}