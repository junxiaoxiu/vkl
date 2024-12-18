#include <iostream>
#include "vulkan/vulkan.hpp"

#include "vkl.hpp"
#include "sdl_window.hpp"
#include "SDL2/SDL_vulkan.h"
#include "vulkan/vulkan_core.h"

int main() {
    SDL_Window* window = SDL_window_create(1024, 720);

    vkl::Vklapp vulkan_app;
    vkl::Vklapp::Arguments& arguments = vulkan_app.arguments;
    // layers need to open
    arguments.extensions = SDL_getInstanceExtension(window);
    // extensions which device needs to support
    arguments.deviceExteneisons = {"VK_KHR_swapchain"};

    vulkan_app.init(
        [&window](vk::Instance &instance) {
            VkSurfaceKHR surface;
            if(!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
                std::cout << "surface create failure\n";
                exit(-1);
            }
            return surface;
        },
        1024, 720);

    SDL_window_run();

    vulkan_app.quit();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

