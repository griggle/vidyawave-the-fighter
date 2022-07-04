#pragma once

#include "SDL.h"
#include "animations/h_animation.hpp"
#include "players/states/move.hpp"
#include "tools/input_queue.hpp"
#include "tools/sdl_image_tools.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

// // BUTTON INPUT CODES
// States
constexpr bool PRESSED  = true;
constexpr bool RELEASED = false;
// Codes
constexpr unsigned long UP    = 1 << 0;
constexpr unsigned long DOWN  = 1 << 1;
constexpr unsigned long LEFT  = 1 << 2;
constexpr unsigned long RIGHT = 1 << 3;

constexpr unsigned long A = 1 << 4;
constexpr unsigned long B = 1 << 5;
constexpr unsigned long C = 1 << 6;
constexpr unsigned long D = 1 << 7;

/*
    About the coordinate axis:                              ^ negative y-axis
        To match SDL2's naitive                             |
        coordinate system,              negative x-axis <---+---> positive x-axis
        the following axes are                              |
        used ------>                        positive y-axis V
*/

class Player
{
  public:
    float x          = 0;      // x  (  <- (-x) (+x) ->  )
    float y          = 0;      // y  (  ^  (-y) (+y)  v  )
    float max_health = 100;    // maximum health
    float health     = 100;    // current health

    int i_frames = 0;    // frames until player starts to take damage again

    // SDL_Rect      src_area;          // area on texture to blit from
    SDL_Rect      dst_area = {};      // area on texture to blit from
    SDL_Texture * texture  = NULL;    // texture atlas of all textures

    std::vector<SDL_Rect> hitboxes;
    std::vector<SDL_Rect> hurtboxes;

    SDL_Rect collision;    // area to collide with other players / wall with

    Player * other_player = NULL;

    std::map<std::string, State *> states;
    State *                        state = NULL;

    float ground         = 900;     // max y level
    float left_wall      = 50;      // min x
    float right_wall     = 7630;    // max x
    float max_separation = 1750;    // max distance (in x) between players

    int counter = 0;    // counter for transitions and moves

    enum GuardType
    {
        NONE,
        HIGH,
        MID,
        LOW,
    } guard
        = NONE;

  protected:
    std::string player_name;

    InputQueue<128> input_history;    // last 128 inputs
    unsigned long   current_input        = 0;
    bool            had_input_this_frame = false;

    float v_x      = 0;      // velocity in x
    float v_y      = 0;      // velocity in y
    float gravity  = 4.5;    // acceleration under gravity in px/s^2
    float friction = 0.3;    // number between 0 and 1 - 1 being max

    int texture_height = 600;    // height of each texture in atlas
    int texture_width  = 500;    // width of each texture in atlas

    float jump_v              = 50;    /// the vertical velocity applied on jump
    float jump_forward_v      = 10;    /// the horizontal velocity applied on jump forward
    float jump_backward_v     = 10;    /// the horizontal velocity applied on jump backward
    float walk_forward_speed  = 8;     /// the change in x on walk forward
    float walk_backward_speed = 4;     /// the change in x on walk backward

    int blockstun      = 5;     /// number of frames to be stunned after a successful block
    int knockdown_time = 60;    /// frames to stay knocked down

    bool is_left_cache = false;

  public:
    Player (std::string player_name);

    void load_states (SDL_Renderer *);

    void close ();
    void update ();

    void input (unsigned long button, bool state);
    void change_state (std::string name);

    void hit (Move * move);
    bool is_left ();                   // if is left-hand player on screen
    void move (float dx, float dy);    // move the player with edge detection

  protected:
    void update_state ();
    void update_physics ();
    void update_collision ();
    void update_hitboxes ();
    void update_animation ();

    bool is_pressed (unsigned long button);
    bool is_pressed (unsigned long button, unsigned long state);
    bool find_input_string (std::vector<unsigned long> pattern, int fuzziness = 10);
    bool hit_check ();

    virtual void update_moves () = 0;

    void update_neutral ();
    void update_crouch ();
    void update_air ();

    void update_neutral_to_crouch ();
    void update_crouch_to_neutral ();

    void update_air_to_neutral ();
    void update_neutral_to_air ();
    void update_neutral_to_forward_air ();
    void update_neutral_to_backward_air ();

    void update_walk_forward ();
    void update_walk_backward ();

    void update_block_neutral ();
    void update_block_crouch ();
    void update_block_air ();

    void update_stun ();
    void update_knocked_down ();
    void update_grabbed ();
};