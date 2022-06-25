#pragma once

#include "hit_animation.hpp"

#include <string>
#include <vector>

typedef enum
{ 
    MID,
    HIGH,
    LOW,
} MOVE_TYPE;

struct Move
{
    float damage;
    float knockback_x;
    float knockback_y;
    int   stun;

    int frame_length;

    bool is_hard_knockdown = false;
    bool is_grab           = false;

    MOVE_TYPE type;

    int final_state;

    std::vector<Move> successful_grab_move;
};