#include "vkl.hpp"
#include "vulkan/vulkan_handles.hpp"

namespace vkl {

void Vklapp::init() {

}

void Vklapp::quit() {

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

}