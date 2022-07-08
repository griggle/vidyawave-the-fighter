#pragma once

#include "SDL.h"
#include "animation.hpp"

#include <fstream>
#include <queue>
#include <vector>

class HitAnimation : public Animation
{
  protected:
    std::vector<std::vector<SDL_Rect>> hitboxes;
    std::vector<std::vector<SDL_Rect>> hurtboxes;

  public:
    HitAnimation (std::vector<std::string> atlas, std::vector<std::vector<SDL_Rect>> hitboxes,
                  std::vector<std::vector<SDL_Rect>> hurtboxes, int fps = 60);

    std::vector<SDL_Rect> get_hitboxes (int frame);
    std::vector<SDL_Rect> get_hurtboxes (int frame);

  protected:
    //std::vector<std::vector<SDL_Rect>> load_hiurtboxes_from_file (std::string path);
};