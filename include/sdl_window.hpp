#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"
#undef main

SDL_Window* SDL_window_create(int width, int height);

void SDL_window_run();

