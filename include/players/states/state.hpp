#pragma once

#include "animations/h_animation.hpp"
#include "players/generated/generated_states.hpp"

#include <functional>

class State
{
  public:
    Animation * animation;
    std::string name;

    std::function<void ()> update_function;

    int stun_frames;

    enum StateType
    {
        STATE,
        MOVE_NORMAL,
        MOVE_COMMAND,
        MOVE_GRAB,
        MOVE_SPECIAL,
        MOVE_COMMAND_GRAB,
        MOVE_SUPER,
    } state_type;

  public:
    State (std::string player_name, std::string state_name, std::function<void ()> update_function, int fps = 60);
    State (std::string player_name, std::string state_name, std::function<void ()> update_function,
           StateType state_type, int fps = 60);

    void load_texture (SDL_Renderer * renderer);
    void close ();

    SDL_Texture *         get_texture (int frame);
    std::vector<SDL_Rect> get_hitboxes (int frame);
    std::vector<SDL_Rect> get_hurtboxes (int frame);

    int size () { return animation->size (); };
};