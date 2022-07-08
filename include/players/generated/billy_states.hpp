#pragma once

#include <map>
#include <string>
#include <vector>

namespace PlayerBillyGenerated
{
    std::map<std::string, std::vector<std::string>>                        atlas_map;
    std::map<std::string, std::vector<std::vector<std::vector<SDL_Rect>>>> hitbox_map;
    std::map<std::string, std::vector<std::vector<std::vector<SDL_Rect>>>> hurtbox_map;
}