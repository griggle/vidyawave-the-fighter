#pragma once

#include "SDL.h"
#include "tools/json.hpp"
#include "tools/sdl_image_tools.hpp"

#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>


// // MOVEMENT
// Movement state indexes
constexpr int NEUTRAL_STATE   = 0;
constexpr int CROUCHING_STATE = 1;
constexpr int AIRBORNE_STATE  = 2;
// Transitional states
constexpr int WALK_FORWARD_STATE      = 3;
constexpr int WALK_BACKWARD_STATE     = 4;
constexpr int NEUTRAL_TO_CROUCH_STATE = 5;
constexpr int NEUTRAL_TO_AIR_STATE    = 6;
constexpr int CROUCH_TO_NEUTRAL_STATE = 7;

// // MOVES
constexpr int MOVE_FIRST_STATE = 8;
// Neutral move indexes
constexpr int MOVE_A_STATE = 8;
constexpr int MOVE_B_STATE = 9;
constexpr int MOVE_C_STATE = 10;
constexpr int MOVE_D_STATE = 11;

struct MoveInfo
{
    int damage;

    int startup;
    int active;
    int recovery;
};

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

    SDL_Rect      src_area;    // area on texture to blit from
    SDL_Rect      dst_area;    // area on screen to blit to
    SDL_Texture * texture = NULL;

    MoveInfo move;

  protected:
    float frame_counter = 0;    // frame counter for animations
    int   counter       = 0;    // counter for transitions and moves

    float floor_level;    // lowest y level

    int texture_height = 600;
    int texture_width  = 500;

    float jump_height        = 50;
    float walk_speed         = 10;
    float reverse_walk_speed = 5;

    float v_x     = 0;
    float v_y     = 0;    // velocity in x and y
    float gravity = 5;    // acceleration under gravity in px/s^2

    std::vector<unsigned int> directional_input_history;
    unsigned int              directional_current_input = 0;

    std::vector<unsigned int> button_input_history;
    unsigned int              button_current_input = 0;

    bool can_jump = true;    // resets on being on the ground in neutral (used to stop constant jumping on holding up)
    int  jump_direction = 0;    // -1 left, 0 up, 1 right

    int state = NEUTRAL_STATE;

    std::vector<int> animation_frame_counts;

    // only store x offset, y offset, width, and height
    std::vector<std::vector<std::vector<SDL_Rect>>> state_hurtboxes;    // all hurtbox vector values for each state
    std::vector<std::vector<std::vector<SDL_Rect>>> state_hitboxes;     // all hurtbox vector values for each state

    std::vector<MoveInfo> moves;

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
    void update_state ();
    void update_moves ();
    void update_physics ();
    void update_hitboxes ();
    void update_textures ();

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