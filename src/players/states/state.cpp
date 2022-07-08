#include "players/states/state.hpp"

State::State (std::string player_name, std::string state_name, std::function<void ()> update_function,
              StateType state_type, int fps)
    : update_function (update_function), state_type (state_type)
{
    std::vector<std::string>           atlas     = {"res/missing_texture.png"};
    std::vector<std::vector<SDL_Rect>> hitboxes  = {};
    std::vector<std::vector<SDL_Rect>> hurtboxes = {};


    if (!GeneratedStates::atlas_map.contains (player_name)
        || !GeneratedStates::atlas_map.at (player_name).contains (state_name))
        std::cerr << player_name << "::" << state_name << " not found in state atlas map.\n";
    else
        atlas = GeneratedStates::atlas_map[player_name][state_name];


    if (!GeneratedStates::hitbox_map.contains (player_name) || !GeneratedStates::hitbox_map.at (player_name).contains (state_name))
        std::cerr << player_name << "::" << state_name << " not found in state hitbox map.\n";
    else
        hitboxes = GeneratedStates::hitbox_map[player_name][state_name];


    if (!GeneratedStates::hurtbox_map.contains (player_name) || !GeneratedStates::hurtbox_map.at (player_name).contains (state_name))
        std::cerr << player_name << "::" << state_name << " not found in state hurtbox map.\n";
    else
        hurtboxes = GeneratedStates::hurtbox_map[player_name][state_name];


    animation = new HitAnimation (atlas, hitboxes, hurtboxes, fps);

    name = state_name;
}

State::State (std::string player_name, std::string state_name, std::function<void ()> update_function, int fps)
    : State (player_name, state_name, update_function, STATE, fps)
{
}

void State::load_texture (SDL_Renderer * renderer) { animation->load_texture (renderer); }

void State::close ()
{
    animation->close_texture ();
    delete animation;
    animation = NULL;
}

SDL_Texture * State::get_texture (int frame) { return (animation)->get_frame (frame); }

std::vector<SDL_Rect> State::get_hitboxes (int frame)
{
    return static_cast<HitAnimation *> (animation)->get_hitboxes (frame);
}

std::vector<SDL_Rect> State::get_hurtboxes (int frame)
{
    return static_cast<HitAnimation *> (animation)->get_hurtboxes (frame);
}