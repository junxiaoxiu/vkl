#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_vulkan.h"
#include <vector>
#undef main

SDL_Window* SDL_window_create(int width, int height);

std::vector<const char*> SDL_getInstanceExtension(SDL_Window* window);

void SDL_window_run();

