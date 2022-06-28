#pragma once

#include "SDL.h"
#include "tools/sdl_image_tools.hpp"

#include <string>
#include <vector>
#include <fstream>

class Animation
{
  protected:
    int frame_width  = 500;
    int frame_height = 600;
    int width        = -1;
    int height       = -1;

    int frames = -1;

    int fps = 60;

    std::string atlas_path;

    std::vector<SDL_Texture *> textures;

  public:
    Animation (std::string atlas_path = "res/characters/missing_texture.atlas", int fps = 60);

    void load_texture (SDL_Renderer * renderer);
    void close_texture ();

    inline int size () { return frames; };

    SDL_Texture * get_frame (int frame);
};