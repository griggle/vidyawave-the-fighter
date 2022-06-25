#pragma once

#include "SDL.h"
#include "tools/hit_animation.hpp"
#include "tools/input_queue.hpp"
#include "tools/move.hpp"
#include "tools/sdl_image_tools.hpp"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
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

typedef enum
{
    // // MOVEMENT
    // Idle state indexes
    NEUTRAL = 0,
    CROUCHING,
    AIRBORNE,

    // Movement states
    WALK_FORWARD,
    WALK_BACKWARD,
    SPRINT,
    BACKDASH,

    // Stun states
    BLOCK_NEUTRAL,
    BLOCK_CROUCH,
    BLOCK_AIR,
    GRABBED,
    STUN_NEUTRAL,
    STUN_CROUCH,
    STUN_AIR,
    KNOCKED_DOWN,

    // Transitional states
    NEUTRAL_TO_CROUCH,
    NEUTRAL_TO_AIR,
    NEUTRAL_TO_FORWARD_AIR,
    NEUTRAL_TO_BACKWARD_AIR,
    CROUCH_TO_NEUTRAL,


    // // MOVES
    // Neutral move indexes
    MOVE_A,
    MOVE_B,
    MOVE_C,
    MOVE_C_GRAB,
    MOVE_D,

} STATE_INDEXES;

constexpr int IDLE_FIRST_STATE       = NEUTRAL;
constexpr int MOVEMENT_FIRST_STATE   = WALK_FORWARD;
constexpr int STUN_FIRST_STATE       = BLOCK_NEUTRAL;
constexpr int TRANSITION_FIRST_STATE = NEUTRAL_TO_CROUCH;
constexpr int MOVE_FIRST_STATE       = MOVE_A;


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

    bool is_lefthand_player;    // if this player is on the left
    bool grab_hit = false;      // if a grab just hit

    //SDL_Rect      src_area;          // area on texture to blit from
    SDL_Rect      dst_area;          // area on texture to blit from
    SDL_Texture * texture = NULL;    // texture atlas of all textures

    std::vector<SDL_Rect> hitboxes;
    std::vector<SDL_Rect> hurtboxes;

    SDL_Rect collision;    // area to collide with other players / wall with

    Player * other_player = NULL;

    Move move;

    int state = NEUTRAL;

  protected:
    InputQueue<128> input_history;    // last 128 inputs
    bool            had_input_this_frame;

    std::vector<HitAnimation> animations;    // animations for each state

    int frame_counter = 0;    // frame counter for animations
    int counter       = 0;    // counter for transitions and moves

    float v_x        = 0;      // velocity in x
    float v_y        = 0;      // velocity in y
    float gravity    = 4.5;    // acceleration under gravity in px/s^2
    float friction   = 0.4;
    float ground     = 900;     // max y level
    float left_wall  = 50;       // min x
    float right_wall = 1870;    // max x

    int texture_height = 600;    // height of each texture in atlas
    int texture_width  = 500;    // width of each texture in atlas

    float jump_height        = 50;
    float sprint_speed       = 20;
    float backdash           = 20;
    float walk_speed         = 8;
    float reverse_walk_speed = 4;

    int time_to_jump     = 5;     // frames to spend in NEUTRAL_TO_AIR
    int time_to_crouch   = 2;     // frames to spend in NEUTRAL_TO_CROUCH
    int time_to_stand    = 2;     // frames to spend in CROUCH_TO_NEUTRAL
    int time_to_backdash = 10;    // frames to spend in BACKDASH

  public:
    Player (bool is_lefthand_player);

    void load_textures (SDL_Renderer *);
    void close_textures ();
    void update ();

    void input (unsigned long button, bool state);

    void hit (Move move);

  protected:
    void update_state ();
    void update_physics ();
    void update_move_states ();

    void update_movement_states ();
    void update_idle_animation ();
    void update_frame_animation ();
    void update_collision ();
    void update_hitboxes ();

    inline bool is_pressed (unsigned long button);
    inline bool is_pressed (unsigned long button, unsigned long state);

    bool find_input_string (std::vector<unsigned long> pattern, int fuzziness = 10);

    void update_neutral ();
    void update_crouching ();
    void update_airborne ();

    void update_walk_forward ();
    void update_walk_backward ();
    void update_sprint ();
    void update_backdash ();

    void update_stun ();    // counter decriments rather than increments

    void update_neutral_to_crouch ();
    void update_neutral_to_air ();
    void update_neutral_to_forward_air ();
    void update_neutral_to_backward_air ();
    void update_crouch_to_neutral ();

    void update_standard_move ();
    void update_grab ();
};