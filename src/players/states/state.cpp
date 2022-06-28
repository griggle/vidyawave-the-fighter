#include "players/states/state.hpp"

State::State (std::string player_name, std::string state_name, std::function<void ()> update_function,
              StateType state_type,
              int fps)
    : update_function (update_function), state_type (state_type)
{
    animation = new HitAnimation ("res/characters/" + player_name + "/animations/" + state_name + ".atlas",
                                  "res/characters/" + player_name + "/animations/" + state_name + ".hurtboxes",
                                  "res/characters/" + player_name + "/animations/" + state_name + ".hitboxes", fps);

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