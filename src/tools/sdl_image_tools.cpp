#include "tools/sdl_image_tools.hpp"

SDL_Texture * IMG_LoadTexture (SDL_Renderer * renderer, const char * file)
{
    SDL_Texture * texture = NULL;
    SDL_Surface * surface = IMG_Load (file);
    if (surface)
    {
        texture = SDL_CreateTextureFromSurface (renderer, surface);
        SDL_FreeSurface (surface);
    }
    else
    {
        std::cerr << "unable to load texture: " << file << "\n";
    }
    return texture;
}

bool rect_intersect (SDL_Rect r1, SDL_Rect r2)
{
    if (r1.w < 0)
    {
        r1.x += r1.w;
        r1.w = fabs (r1.w);
    }

    if (r2.w < 0)
    {
        r2.x += r2.w;
        r2.w = fabs (r2.w);
    }

    /*std::cout << "(" << r1.x << "," << r1.y << "," << r1.w << "," << r1.h << ") (" << r2.x << "," << r2.y << "," << r2.w
              << "," << r2.h << ")\n";
    std::cout << SDL_HasIntersection (&r1, &r2) << "\n";*/

    return SDL_HasIntersection (&r1, &r2);
}