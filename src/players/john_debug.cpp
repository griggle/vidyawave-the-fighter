#include "players/john_debug.hpp"

PlayerJohnDebug::PlayerJohnDebug () : Player ("john_debug")
{
    // clang-format off
    states["sprint"] = new State (player_name, "sprint", std::bind(&PlayerJohnDebug::update_sprint, this), 6);
    states["backdash"] = new State (player_name, "backdash", std::bind(&PlayerJohnDebug::update_backdash, this), 6);

	states["a"] = new Move (player_name, "a", Move::MID, State::MOVE_NORMAL, 10, 40, 0, 20, false, false, std::bind(&PlayerJohnDebug::update_a, this));
	states["b"] = new Move (player_name, "b", Move::LOW, State::MOVE_NORMAL, 10, 40, 0, 20, false, false, std::bind(&PlayerJohnDebug::update_b, this));
	states["c"] = new Move (player_name, "c", Move::MID, State::MOVE_NORMAL, 5, 40, 0, 20, false, true, std::bind(&PlayerJohnDebug::update_c, this));
	states["c_grab"] = new Move (player_name, "c_grab", Move::MID, State::MOVE_GRAB, 10, 30, -10, 20, false, false, std::bind(&PlayerJohnDebug::update_c_grab, this));
	states["d"] = new Move (player_name, "d", Move::HIGH, State::MOVE_NORMAL, 10, 40, 0, 20, true, false, std::bind(&PlayerJohnDebug::update_d, this));
    // clang-format on
}

void PlayerJohnDebug::update_moves ()
{
    // movement
    if (state->state_type == State::STATE)
    {
        if (find_input_string ({is_left () ? RIGHT : LEFT, 0, is_left () ? RIGHT : LEFT})) 
        {
            state   = states["sprint"];
            counter = 0;
            input_history.clear ();
        }
        if (find_input_string ({is_left () ? LEFT : RIGHT, 0, is_left () ? LEFT : RIGHT}))
        {
            state   = states["backdash"];
            counter = 0;
            input_history.clear ();
        }
    }

    // if not currently in a move
    if (state->state_type == State::STATE)
    {
        if (is_pressed (A) && !is_pressed (A, input_history.at (1)))
        {
            state   = states["a"];
            counter = 0;
        }
        if (is_pressed (B) && !is_pressed (B, input_history.at (1)))
        {
            state   = states["b"];
            counter = 0;
        }
        if (is_pressed (C) && !is_pressed (C, input_history.at (1)))
        {
            state   = states["c"];
            counter = 0;
        }
        if (is_pressed (D) && !is_pressed (D, input_history.at (1)))
        {
            state   = states["d"];
            counter = 0;
        }
    }
}

void PlayerJohnDebug::update_sprint ()
{
    if (!is_pressed (is_left () ? RIGHT : LEFT))
        state = states["neutral"];
    else
    {
        move (is_left () ? sprint_speed : -sprint_speed, 0);
    }

    // crouching
    if (is_pressed (DOWN))
    {
        state   = states["neutral_to_crouch"];
        counter = 0;
        v_x += is_left () ? sprint_speed : -sprint_speed;
    }

    // jumping
    else if (is_pressed (UP))
    {
        state = states["neutral_to_forward_air"];
        counter = 0;
        v_x += is_left () ? sprint_speed : -sprint_speed;
    }
}

void PlayerJohnDebug::update_backdash ()
{
    if (counter >= 10)
        state = states["neutral"];
    else
    {
        move (is_left () ? -backdash_speed : backdash_speed, 0);
    }
}

void PlayerJohnDebug::update_a ()
{
    auto s = static_cast<Move *> (state);

    if (counter >= state->size ())
    {
        s->hit  = false;
        state   = states["neutral"];
        counter = 0;
    }
    else if (!s->hit && hit_check ())
    {
        other_player->hit (s);
        s->hit = true;
    }
}

void PlayerJohnDebug::update_b ()
{
    auto s = static_cast<Move *> (state);

    if (counter >= state->size ())
    {
        s->hit  = false;
        state   = states["neutral"];
        counter = 0;
    }
    else if (!s->hit && hit_check ())
    {
        other_player->hit (s);
        s->hit = true;
    }
}

void PlayerJohnDebug::update_c ()
{
    auto s = static_cast<Move *> (state);

    if (counter >= state->size ())
    {
        s->hit  = false;
        state   = states["neutral"];
        counter = 0;
    }
    else if (!s->hit && hit_check ())
    {
        other_player->hit (s);
        state   = states["c_grab"];
        counter = 0;
    }
}


void PlayerJohnDebug::update_c_grab ()
{
    auto s = static_cast<Move *> (state);

    other_player->x = hitboxes.at (0).x;
    other_player->y = hitboxes.at (0).y + 100;

    other_player->state->stun_frames++;

    if (counter >= state->size ())
    {
        other_player->hit (s);
        other_player->state    = other_player->states["knocked_down"];
        other_player->i_frames = 120;

        s->hit  = false;
        state   = states["neutral"];
        counter = 0;
    }
}

void PlayerJohnDebug::update_d ()
{
    auto s = static_cast<Move *> (state);

    if (counter >= state->size ())
    {
        s->hit  = false;
        state   = states["neutral"];
        counter = 0;
    }
    else if (!s->hit && hit_check ())
    {
        other_player->hit (s);
        s->hit = true;
    }
}
