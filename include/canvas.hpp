#pragma once

#include "scenes/game_scene.hpp"
#include "scenes/scene.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <stdio.h>
#include <string>
#include <vector>

class Canvas
{
  protected:
    SDL_Window *   gWindow   = NULL;    // The window we'll be rendering to
    SDL_Renderer * gRenderer = NULL;    // The window renderer
    int            width, height;

    SDL_Joystick * gGameController = NULL;    // Game Controller 1 handler

    bool      quit;    // Main loop flag
    SDL_Event e;       // Event handler

    Scene * current_scene;    // The current scene

  public:
    Canvas (int width, int height);

    bool init ();           // init canvas and memory
    bool step ();           // run a frame
    void screen_shot ();    // save screen to file
    void close ();          // free all memory

    bool set_scene (Scene * new_scene);    // change current scene
};