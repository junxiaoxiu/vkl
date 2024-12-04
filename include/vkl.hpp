#pragma once

#include "vulkan/vulkan.hpp"
#include "vector"

namespace vkl {

class Vklapp {
public:
    void init();
    void run();
    void quit();

    void createInstance();

// arguments main function pass on to Vklapp
struct Arguments {
    std::vector<const char*> extensionLayers;
};
Arguments arguments;

private:
    vk::Instance instance;

};

}