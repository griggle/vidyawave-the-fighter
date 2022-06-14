#pragma once

#include "players/player.hpp"
#include "scene.hpp"

#include <SDL.h>
#include <iostream>

class GameScene : public Scene
{
  protected:
    const int JOYSTICK_DEADZONE = 2000;

    int ground_y;

    std::vector<Player> players;

  public:
    GameScene (int width, int height);

    bool init (SDL_Renderer *);         // init canvas and memory
    bool step_event (SDL_Event & e);    // run a frame
    void step_scene ();                 // update internal state
    void step_render (SDL_Window *, SDL_Renderer *, int & width,
                      int & height);    // run a frame
    void close ();                      // free all memory
};