#pragma once

#include "SDL.h"

class SceneContainer
{
  protected:
    int  width  = 1920;
    int  height = 1080;    // dimensions
    bool quit;             // Main loop flag

  public:
    SceneContainer (int width, int height);
};