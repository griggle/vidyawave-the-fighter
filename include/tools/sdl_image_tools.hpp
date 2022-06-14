#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

SDL_Texture * IMG_LoadTexture (SDL_Renderer * renderer, const char * file);

bool rect_intersect (SDL_Rect r1, SDL_Rect r2);