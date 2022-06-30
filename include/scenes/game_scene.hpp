#pragma once

#include "animations/animation.hpp"
#include "players/john_debug.hpp"
#include "players/player.hpp"
#include "scene.hpp"

#include <SDL.h>
#include <iostream>

class Stage
{
  public:
    SDL_Texture * texture;
    std::string   texture_path;

    SDL_Rect viewport;          /// rect in world space that represents the screen
    SDL_Rect src_rect;    /// the area of the texture to use
    SDL_Rect dst_rect;    /// the rect that viewport maps to in camera space
    float    viewport_sf;       /// how much bigger / smaller the view is compared to 1080p

    int left_wall;
    int right_wall;
    int ground_y;

    int tex_width;
    int tex_height;

  public:
    Stage (std::string texture_path = "res/stages/debug_city/frames/0000.png", int left_wall = 50,
           int right_wall = 7630, int ground_y = 900);

    void load (SDL_Renderer * renderer);
    void close ();

    void update_viewport (Player * p1, Player * p2);

    SDL_Rect project_rect (SDL_Rect in);
};

class GameScene : public Scene
{
  protected:
    const int JOYSTICK_DEADZONE = 2000;

    bool is_debug = false;

    std::vector<Player *> players;

    Stage stage;

  public:
    GameScene (int width, int height);

    bool init (SDL_Renderer *);         // init canvas and memory
    bool step_event (SDL_Event & e);    // run a frame
    void step_scene ();                 // update internal state
    void step_render (SDL_Window *, SDL_Renderer *, int & width,
                      int & height);    // run a frame
    void close ();                      // free all memory
};