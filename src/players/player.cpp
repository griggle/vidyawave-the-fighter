#include "players/player.hpp"

Player::Player (std::string player_name) : player_name (player_name)
{
    dst_area = SDL_Rect {0, 0, texture_width, texture_height};

    // clang-format off
    states["neutral"] = new State (player_name, "neutral", std::bind (&Player::update_neutral, this), 6);
    states["crouch"] = new State (player_name, "crouch", std::bind (&Player::update_crouch, this), 6);
    states["air"] = new State (player_name, "air", std::bind (&Player::update_air, this), 6);

    states["neutral_to_crouch"] = new State (player_name, "neutral_to_crouch", std::bind (&Player::update_neutral_to_crouch, this));
    states["crouch_to_neutral"] = new State (player_name, "crouch_to_neutral", std::bind (&Player::update_crouch_to_neutral, this));

    states["neutral_to_air"] = new State (player_name, "neutral_to_air", std::bind (&Player::update_neutral_to_air, this));
    states["neutral_to_forward_air"] = new State (player_name, "neutral_to_forward_air", std::bind (&Player::update_neutral_to_forward_air, this));
    states["neutral_to_backward_air"] = new State (player_name, "neutral_to_backward_air", std::bind (&Player::update_neutral_to_backward_air, this));

    states["walk_forward"] = new State (player_name, "walk_forward", std::bind (&Player::update_walk_forward, this), 6);
    states["walk_backward"] = new State (player_name, "walk_backward", std::bind (&Player::update_walk_backward, this), 6);

    states["block_neutral"] = new State(player_name, "block_neutral", std::bind (&Player::update_block_neutral, this));
    states["block_crouch"] = new State(player_name, "block_crouch", std::bind (&Player::update_block_crouch, this));
    states["block_air"] = new State(player_name, "block_air", std::bind (&Player::update_block_air, this));

    states["stun"] = new State(player_name, "stun", std::bind (&Player::update_stun, this));
    states["knocked_down"] = new State(player_name, "knocked_down", std::bind (&Player::update_knocked_down, this));
    states["grabbed"] = new State(player_name, "grabbed", std::bind (&Player::update_grabbed, this));
    // clang-format on
}

void Player::load_states (SDL_Renderer * renderer)
{
    for (auto & state : states) state.second->load_texture (renderer);

    state = states["neutral"];
}

void Player::close ()
{
    for (auto & state : states)
    {
        state.second->close ();
        delete state.second;
        state.second = NULL;
    }

    texture = NULL;
}

void Player::update ()
{
    // update frame counters
    counter++;
    i_frames = std::max (0, i_frames - 1);

    // if no change in inputs, duplicate last input
    if (!had_input_this_frame) input_history.push (input_history.at (0));
    had_input_this_frame = false;

    // change the players internal state and handle inputs
    update_state ();

    // do physics step for gravity, etc...
    if (state->name != "grabbed" && state->state_type != State::MOVE_GRAB
        && state->state_type != State::MOVE_COMMAND_GRAB)
        update_physics ();

    // update the animation with a new frame (and accompanying collision & hitboxes)
    if (state->name != "grabbed" && state->state_type != State::MOVE_GRAB
        && state->state_type != State::MOVE_COMMAND_GRAB)
        update_collision ();

    update_animation ();
    update_hitboxes ();


    // todo: move this to the game_scene - player should not know (or care) where it is being rendered to
    dst_area.x = x - (texture_width / 2);
    dst_area.y = y - texture_height;

    // if(is_left())std::cout << std::bitset<32> (input_history.at (0)) << "\n";
}

void Player::input (unsigned long button, bool state)
{
    if (state == PRESSED)
    {
        if ((input_history.at (0) | button) != (input_history.at (0)))
            input_history.push (input_history.at (0) | button);
    }
    else
        input_history.push (input_history.at (0) & (~button));

    had_input_this_frame = true;
}

void Player::hit (Move * move)
{
    // cannot block a grab
    if (!move->is_grab)
    {
        // if blocked
        if (guard == AIR && move->move_area == Move::AIR)
        {
            state   = states["block_air"];
            counter = 0;

            return;
        }
        else if (guard == MID && (move->move_area == Move::MID || move->move_area == Move::AIR))
        {
            state   = states["block_neutral"];
            counter = 0;

            return;
        }
        else if (guard == LOW && (move->move_area == Move::LOW || move->move_area == Move::MID))
        {
            state   = states["block_crouch"];
            counter = 0;

            return;
        }
    }


    health -= move->damage;
    guard = NONE;

    // if we're grabbed, stop processing updates until let go
    if (move->is_grab)
    {
        state   = states["grabbed"];
        counter = 0;

        return;
    }

    i_frames += 10;
    state              = states["stun"];
    counter            = 0;
    state->stun_frames = move->stun;

    v_x += is_left () ? -move->knockback_x : move->knockback_x;
    v_y -= move->knockback_y;
}

void Player::update_state ()
{
    // moves overide all movement inputs
    update_moves ();

    // update this state
    state->update_function ();

    std::cout << state->name << "                \r";
}

void Player::update_physics ()
{
    // apply gravity and momentum to the player
    move (v_x, -v_y);

    v_y -= gravity;


    // todo: change this from hardcoded floor
    if (y >= ground)
    {
        y = ground;
        v_x *= 1 - friction;
        v_y = 0;
    }
}

void Player::update_animation ()
{
    texture = state->get_texture (counter);

    SDL_SetTextureAlphaMod (texture, i_frames == 0 ? 255 : 40);
}

void Player::update_collision ()
{
    // update collision rect
    collision = {-35, -250, 70, 250};

    if (state->name == "crouch")
    {
        collision.h = 150;
        collision.y = -150;
    }
    else if (state->name == "air")
    {
        collision.h = 150;
    }

    collision.x *= (is_left () ? 1 : -1);
    collision.w *= (is_left () ? 1 : -1);

    collision.x += x;
    collision.y += y;

    if (collision.w < 0)
    {
        collision.x += collision.w;
        collision.w = fabs (collision.w);
    }

    // if there is a collision, move half the difference (other player moves other half)
    if (rect_intersect (collision, other_player->collision))
    {
        if (!is_left ())
            x += (other_player->collision.x + other_player->collision.w - collision.x) / 2;
        else
            x -= (collision.x + collision.w - other_player->collision.x) / 2;
    }

    // if against a wall, stop moving
    if (collision.x <= left_wall)
    {
        x           = left_wall + fabs (collision.x - x);
        collision.x = left_wall;
    }
    else if (collision.x + collision.w >= right_wall)
    {
        x           = right_wall - fabs (collision.x - x + collision.w);
        collision.x = right_wall - collision.w;
    }
}

void Player::update_hitboxes ()
{
    hitboxes  = state->get_hitboxes (counter);
    hurtboxes = state->get_hurtboxes (counter);

    if (!is_left ())
    {
        for (auto & hitbox : hitboxes)
        {
            hitbox.w *= -1;
            hitbox.x *= -1;
        }

        for (auto & hurtbox : hurtboxes)
        {
            hurtbox.w *= -1;
            hurtbox.x *= -1;
        }
    }

    for (auto & hitbox : hitboxes)
    {
        hitbox.x += x;
        hitbox.y += y;
    }

    for (auto & hurtbox : hurtboxes)
    {
        hurtbox.x += x;
        hurtbox.y += y;
    }
}

bool Player::is_pressed (unsigned long button) { return is_pressed (button, input_history.at (0)); }

bool Player::is_pressed (unsigned long button, unsigned long state)
{
    // if we are checking for directionals, consider cancelling inputs
    if (button == UP)
        return ((UP & state) == UP) && !((DOWN & state) == DOWN);
    else if (button == DOWN)
        return !((UP & state) == UP) && ((DOWN & state) == DOWN);
    else if (button == LEFT)
        return ((LEFT & state) == LEFT) && !((RIGHT & state) == RIGHT);
    else if (button == RIGHT)
        return !((LEFT & state) == LEFT) && ((RIGHT & state) == RIGHT);

    // default case
    return (button & state) == button;
}

bool Player::is_left ()
{
    if (state->name != "grabbed" && state->state_type != State::MOVE_GRAB
        && state->state_type != State::MOVE_COMMAND_GRAB)
        is_left_cache = x < other_player->x;
    return is_left_cache;
}

void Player::move (float dx, float dy)
{
    if (std::fabs (x + dx - other_player->x) < max_separation)
    {
        x += dx;
        y += dy;
    }
}

bool Player::find_input_string (std::vector<unsigned long> pattern, int fuzziness)
{
    int  current_history_index    = 0;
    bool found_end_of_repetitions = false;

    for (int i = 0; i < pattern.size (); i++)
    {
        if (input_history.at (current_history_index) != pattern[i]) return false;

        if (i < pattern.size () - 1 && pattern[i] == pattern[i + 1]) { current_history_index++; }
        else
        {
            found_end_of_repetitions = false;

            for (int j = 0; j < fuzziness; j++)
            {
                if (current_history_index > input_history.size ()) break;

                if (input_history.at (current_history_index++) != pattern[i])
                {
                    found_end_of_repetitions = true;
                    break;
                }
            }

            if (!found_end_of_repetitions) return false;
        }
    }

    return true;
}

bool Player::hit_check ()
{
    if (other_player->i_frames > 0) return false;

    for (auto & hitbox : hitboxes)
    {
        for (auto & hurtbox : other_player->hurtboxes)
            if (rect_intersect (hitbox, hurtbox)) return true;
    }

    return false;
}

void Player::update_neutral ()
{
    // crouching
    if (is_pressed (DOWN))  
    {
        state   = states["neutral_to_crouch"];
        counter = 0;
    }

    // jumping
    else if (is_pressed (UP))
    {
        if (is_pressed (LEFT))
            state = is_left () ? states["neutral_to_backward_air"] : states["neutral_to_forward_air"];
        else if (is_pressed (RIGHT))
            state = !is_left () ? states["neutral_to_backward_air"] : states["neutral_to_forward_air"];
        else
            state = states["neutral_to_air"];

        counter = 0;
    }

    // walking
    else if (is_pressed (LEFT))
        state = is_left () ? states["walk_backward"] : states["walk_forward"];
    else if (is_pressed (RIGHT))
        state = !is_left () ? states["walk_backward"] : states["walk_forward"];
}

void Player::update_crouch ()
{
    if (is_pressed (is_left () ? LEFT : RIGHT))
        guard = LOW;
    else
        guard = NONE;

    if (!is_pressed (DOWN))
    {
        state   = states["crouch_to_neutral"];
        counter = 0;
        guard   = NONE;
    }
}

void Player::update_air ()
{
    if (y >= ground) state = states["neutral"];

    if (is_pressed (is_left () ? LEFT : RIGHT))
        guard = AIR;
    else
        guard = NONE;
}

void Player::update_neutral_to_crouch ()
{
    if (counter >= state->size ())
    {
        state   = states["crouch"];
        counter = 0;
    }
}

void Player::update_crouch_to_neutral ()
{
    if (counter >= state->size ())
    {
        state   = states["neutral"];
        counter = 0;
    }
}

void Player::update_neutral_to_air ()
{
    if (counter >= state->size ())
    {
        state = states["air"];
        v_y += jump_v;
        counter = 0;
    }
}

void Player::update_neutral_to_forward_air ()
{
    if (counter >= state->size ())
    {
        state = states["air"];
        v_y += jump_v;
        v_x += is_left () ? jump_forward_v : -jump_forward_v;
        counter = 0;
    }
}

void Player::update_neutral_to_backward_air ()
{
    if (counter >= state->size ())
    {
        state = states["air"];
        v_y += jump_v;
        v_x += is_left () ? -jump_backward_v : jump_backward_v;
        counter = 0;
    }
}

void Player::update_walk_forward ()
{
    // everything you can do while walking can be done in neutral
    update_neutral ();

    if (is_pressed (is_left () ? RIGHT : LEFT)) { move(is_left () ? walk_forward_speed : -walk_forward_speed, 0); }
    else
    {
        state = states["neutral"];
    }
}

void Player::update_walk_backward ()
{
    // everything you can do while walking can be done in neutral
    update_neutral ();

    if (is_pressed (is_left () ? LEFT : RIGHT))
    {
        move( is_left () ? -walk_backward_speed : walk_backward_speed, 0);
        guard = MID;
    }
    else
    {
        state = states["neutral"];
        guard = NONE;
    }
}

void Player::update_block_neutral ()
{
    if (counter >= blockstun)
    {
        state   = states["neutral"];
        counter = 0;
    }
}

void Player::update_block_crouch ()
{
    if (counter >= blockstun)
    {
        state   = states["crouch"];
        counter = 0;
    }
}

void Player::update_block_air ()
{
    if (counter >= blockstun)
    {
        state   = states["air"];
        counter = 0;
    }
}

void Player::update_stun ()
{
    if (counter >= state->stun_frames)
    {
        state   = states["neutral"];
        counter = 0;
    }
}

void Player::update_knocked_down ()
{
    if (counter >= knockdown_time)
    {
        state   = states["crouch_to_neutral"];
        counter = 0;
    }
}

void Player::update_grabbed () {}
