#pragma once

#include "player.hpp"
#include "billy/billy_states.hpp"

class PlayerBilly : public Player
{
  public:
    PlayerBilly ();

  protected:
    // update functions for each state type
    void update_moves ();
};