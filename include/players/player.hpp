#pragma once

#include "tools/sdl_image_tools.hpp"

#include <SDL.h>
#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>


class Player
{
  public:
    float x = 300;    // x
    float y;          // y dimensions

    bool is_left = true;    // if this player is on the left

    float max_health = 100;    // maximum health
    float health     = 100;    // current health

    std::vector<SDL_Rect> hitboxes;
    std::vector<SDL_Rect> hurtboxes;

    SDL_Rect      render_area;
    SDL_Texture * texture = NULL;

    int current_move          = 0;    // index of current move, 0 if none
    int current_move_priority = 0;    // 0 = normal, 1 = directional, 2 = special, 3 = super
    int current_move_startup  = 0;    // startup time of current move
    int current_move_duration = 0;    // open time of current move
    int current_move_recovery = 0;    // recovery time of current move
    int current_move_damage   = 0;    // damage of current move

  protected:
    unsigned short counter;    // frame counter for animations
    float          floor;      // lowest y level

    float texture_height = 600;
    float texture_width  = 500;

    float jump_height = 50;
    float walk_speed  = 10;

    float v_x     = 0;
    float v_y     = 0;    // velocity in x and y
    float gravity = 5;    // acceleration under gravity in px/s^2

    std::vector<unsigned int> directional_input_history;
    unsigned int              directional_current_input = 0;

    std::vector<unsigned int> button_input_history;
    unsigned int              button_current_input = 0;

    bool just_jumped = false;    // if just jumped (used to stop constant jumping on holding up)

    // 0 - neutral
    // 1 - crouching
    // 2 - airborne
    // 3 - walking forwards
    // 4 - walking backwards
    // 5 - move

    int current_state = 0;

    std::vector<std::vector<SDL_Texture *>> textures;

    // only store x offset, y offset, width, and height
    std::vector<std::vector<SDL_Rect>>              render_areas;       // texture sizes
    std::vector<std::vector<std::vector<SDL_Rect>>> state_hurtboxes;    // all hurtbox vector values for each state
    std::vector<std::vector<std::vector<SDL_Rect>>> state_hitboxes;     // all hurtbox vector values for each state

  public:
    Player (float floor);

    void load_textures (SDL_Renderer *);
    void close_textures ();
    void update ();

    void directional_input (int up, int down, int left, int right);
    void button_input (int x, int circle, int square, int triangle, int select, int logo, int start, int l_stick,
                       int r_stick, int l_bumper, int r_bumper, int l_trigger, int r_trigger);

    inline bool is_crouched ();
    inline bool is_blocking ();
    inline bool is_airborne ();

  protected:
    void apply_force (float dx, float dy);

    void update_movement ();
    void update_physics ();
    void update_hitboxes ();
    void update_textures ();
    void update_current_move ();

    inline bool input_up (unsigned int input);
    inline bool input_down (unsigned int input);
    inline bool input_left (unsigned int input);
    inline bool input_right (unsigned int input);

    inline bool input_x (unsigned int input);
    inline bool input_circle (unsigned int input);
    inline bool input_square (unsigned int input);
    inline bool input_triangle (unsigned int input);
    inline bool input_select (unsigned int input);
    inline bool input_logo (unsigned int input);
    inline bool input_start (unsigned int input);
    inline bool input_l_stick (unsigned int input);
    inline bool input_r_stick (unsigned int input);
    inline bool input_l_bumper (unsigned int input);
    inline bool input_r_bumper (unsigned int input);
    inline bool input_l_trigger (unsigned int input);
    inline bool input_r_trigger (unsigned int input);
};