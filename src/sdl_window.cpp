#include "sdl_window.hpp"

SDL_Window* SDL_window_create(int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("hello vklkan",
                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           width, height,
                                           SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    if(!window) {
        SDL_Log("create window failed");
        exit(-1);
    }

    return window;
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