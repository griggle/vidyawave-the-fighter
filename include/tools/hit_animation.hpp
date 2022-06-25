#pragma once

#include "SDL.h"
#include "animation.hpp"

#include <fstream>
#include <vector>
#include <queue>

class HitAnimation : public Animation
{
  protected:
    std::vector<std::vector<SDL_Rect>> hitboxes;
    std::vector<std::vector<SDL_Rect>> hurtboxes;

    std::string hurtbox_path;
    std::string hitbox_path;

  public:
    HitAnimation (std::string atlas_path   = "res/characters/missing_texture.png",
                  std::string hurtbox_path = "res/characters/john_debug/animations/neutral.hurtboxes",
                  std::string hitbox_path  = "res/characters/john_debug/animations/neutral.hitboxes");

    std::vector<SDL_Rect> get_hitboxes (int frame);
    std::vector<SDL_Rect> get_hurtboxes (int frame);

  protected:
    std::vector<std::vector<SDL_Rect>> load_hiurtboxes_from_file (std::string path);
};