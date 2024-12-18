#include "sdl_window.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_vulkan.h"

#include <vector>

SDL_Window* SDL_window_create(int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("hello vulkan",
                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           width, height,
                                           SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    if(!window) {
        SDL_Log("create window failed");
        exit(-1);
    }

    return window;
}

std::vector<const char*> SDL_getInstanceExtension(SDL_Window* window) {
    unsigned int count;
    std::vector<const char*> extensions;
    SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);
    extensions.resize(count);
    SDL_Vulkan_GetInstanceExtensions(window, &count,extensions.data());
    return extensions;
}

void SDL_window_run() {
    bool closeWindow = false;
    SDL_Event event;

    while(!closeWindow) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                closeWindow = true;
            }
        }
    }
}