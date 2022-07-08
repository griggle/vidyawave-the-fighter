#pragma once

#include "SDL.h"
#include "tools/sdl_image_tools.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <regex>

class Animation
{
  protected:
    int frame_width  = 500;
    int frame_height = 600;
    int width        = -1;
    int height       = -1;

    int frames = -1;

    int fps = 60;

    std::vector<std::string> atlas;

    std::vector<SDL_Texture *> textures;

  public:
    Animation (std::vector<std::string> atlas, int fps = 60);

    void load_texture (SDL_Renderer * renderer);
    void close_texture ();

    inline int size () { return floor (textures.size () * (60.0 / fps)); };

    SDL_Texture * get_frame (int frame);
};