#pragma once

#include "player.hpp"

class PlayerBilly : public Player
{
  public:
    PlayerBilly ();

  protected:
    // update functions for each state type
    void update_moves ();
};