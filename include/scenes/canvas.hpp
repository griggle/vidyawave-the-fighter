#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "scenes/game_scene.hpp"
#include "scenes/online_game_scene.hpp"
#include "scenes/main_menu_scene.hpp"
#include "scenes/scene.hpp"
#include "scenes/scene_container.hpp"

#include <cmath>
#include <stdio.h>
#include <string>
#include <vector>

class Canvas : public SceneContainer
{
  protected:
    SDL_Window *   gWindow   = NULL;    // The window we'll be rendering to
    SDL_Renderer * gRenderer = NULL;    // The window renderer

    SDL_Joystick * gGameController = NULL;    // Game Controller 1 handler

    SDL_Event e;       // Event handler

    Scene * current_scene;    // The current scene

    // The music that will be played
    Mix_Music * gMusic = NULL;

  public:
    Canvas (int width, int height);

    bool init ();           // init canvas and memory
    bool step ();           // run a frame
    void screen_shot ();    // save screen to file
    void close ();          // free all memory

    bool set_scene (Scene * new_scene);    // change current scene
};