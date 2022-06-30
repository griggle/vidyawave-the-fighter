#pragma once

#include "animations/animation.hpp"
#include "players/john_debug.hpp"
#include "players/player.hpp"
#include "scene.hpp"

#include <SDL.h>
#include <iostream>

class GameScene : public Scene
{
  protected:
    const int JOYSTICK_DEADZONE = 2000;

    int ground_y;

    bool is_debug = false;

    std::vector<Player *> players;

    SDL_Texture * stage;
    SDL_Rect      viewport;
    SDL_Rect      stage_src_rect;
    SDL_Rect      stage_dst_rect;
    float         viewport_sf;

    int left_wall = 50;
    int right_wall = 7630;

  public:
    GameScene (int width, int height);

    bool init (SDL_Renderer *);         // init canvas and memory
    bool step_event (SDL_Event & e);    // run a frame
    void step_scene ();                 // update internal state
    void step_render (SDL_Window *, SDL_Renderer *, int & width,
                      int & height);    // run a frame
    void close ();                      // free all memory

  protected:
    SDL_Rect project_rect (SDL_Rect in);
    SDL_Rect get_viewport ();
};