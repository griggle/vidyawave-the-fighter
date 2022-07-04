#pragma once

#include "player.hpp"

class PlayerJohnDebug : public Player
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
    PlayerJohnDebug ();


  protected:
    // update functions for each state type
    void update_moves ();

    // movement update functions
    void update_sprint ();
    void update_backdash ();

    // update functions for each move
    void update_a ();
    void update_b ();
    void update_c ();
    void update_c_grab ();
    void update_d ();
};