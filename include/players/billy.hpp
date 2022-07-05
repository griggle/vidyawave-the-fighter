#pragma once

#include "player.hpp"

class PlayerBilly : public Player
{
  protected:
    float jump_v              = 50;
    float jump_forward_v      = 10;
    float jump_backward_v     = 10;
    float walk_forward_speed  = 8;
    float walk_backward_speed = 4;
    float sprint_speed        = 16;
    float backdash_speed      = 20;

  public:
    PlayerBilly ();

  protected:
    // update functions for each state type
    void update_moves ();
};