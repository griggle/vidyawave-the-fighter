#pragma once

#include "state.hpp"

#include <functional>
#include <string>
#include <vector>

class Move : public State
{
  public:
    float damage      = 0;
    float knockback_x = 0;
    float knockback_y = 0;
    int   stun        = 0;

    bool is_knockdown = false;
    bool is_grab      = true;

    bool hit = false;

    enum MoveArea
    {
        MID,
        AIR,
        LOW,
    } move_area;

  public:
    Move (std::string player_name, std::string state_name, MoveArea move_area, StateType state_type, float damage,
          float knockback_x, float knockback_y, int stun, bool is_knockdown, bool is_grab, std::function<void ()> update_function,
          int fps = 60);
};