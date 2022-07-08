#include "players/billy.hpp"

PlayerBilly::PlayerBilly () : Player ("billy")
{
    jump_v              = 50;
    jump_forward_v      = 10;
    jump_backward_v     = 10;
    walk_forward_speed  = 3;
    walk_backward_speed = 2;

    // clang-format off

    // moves

    // clang-format on
}

void PlayerBilly::update_moves () {}
