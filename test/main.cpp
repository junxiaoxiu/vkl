#include <iostream>
#include "vulkan/vulkan.hpp"

#include "vkl.hpp"
#include "sdl_window.hpp"

int main() {
    SDL_Window* window = SDL_window_create(1024, 720);

    vkl::Vklapp vulkan_app;
    vkl::Vklapp::Arguments& arguments = vulkan_app.arguments;
    arguments.extensionLayers = {"VK_LAYER_KHRONOS_validation"};

    vulkan_app.init();

    SDL_window_run();

    vulkan_app.quit();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

