#include "players/player.hpp"

Player::Player (bool is_lefthand_player) : is_lefthand_player (is_lefthand_player)
{
    collision = SDL_Rect {is_lefthand_player ? 1000 : -1000, 0, 100, 300};
}

void Player::load_textures (SDL_Renderer * renderer)
{
    std::string              path_base = "res/characters/john_debug/animations/";
    std::vector<std::string> states    = {
        "neutral",
        "crouching",
        "airborne",

        "walk_forward",
        "walk_backward",
        "sprint",
        "backdash",

        "block_neutral",
        "block_crouch",
        "block_air",
        "grabbed",
        "stun_neutral",
        "stun_crouch",
        "stun_air",
        "knocked_down",

        "neutral_to_crouch",
        "neutral_to_air",
        "neutral_to_forward_air",
        "neutral_to_backward_air",
        "crouch_to_neutral",

        "a",
        "b",
        "c",
        "c_grab",
        "d",
    };

    for (int i = 0; i < states.size (); i++)
        animations.push_back (HitAnimation (path_base + states.at (i) + ".atlas",
                                            path_base + states.at (i) + ".hurtboxes",
                                            path_base + states.at (i) + ".hitboxes"));


    for (auto & animation : animations) animation.load_texture (renderer);

    dst_area = SDL_Rect {0, 0, texture_width, texture_height};
}

void Player::close_textures ()
{
    for (auto & animation : animations) animation.close_texture ();

    texture = NULL;
}

void Player::update ()
{
    if (!had_input_this_frame) input_history.push (input_history.at (0));

    update_state ();

    if (state != GRABBED)
    {
        update_physics ();
        update_collision ();
    }

    update_hitboxes ();

    frame_counter++;
    had_input_this_frame = false;

    dst_area.x = x - (texture_width / 2);
    dst_area.y = y - texture_height;

    i_frames = std::max (0, i_frames - 1);

    

    // std::cout << std::bitset<32> (input_history.at (0)) << "\n";
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

void Player::hit (Move move)
{
    // check if blocking
    if (is_pressed (is_lefthand_player ? LEFT : RIGHT) && state < TRANSITION_FIRST_STATE && !move.is_grab)
    {
        if (((state == CROUCHING || state == BLOCK_CROUCH) && move.type != HIGH)
            || ((state != CROUCHING && state != BLOCK_CROUCH) && move.type != LOW))
        {
            switch (state)
            {
                case CROUCHING:
                case BLOCK_CROUCH: state = BLOCK_CROUCH; break;
                case AIRBORNE:
                case BLOCK_AIR: state = BLOCK_AIR; break;
                case BLOCK_NEUTRAL:
                default: state = BLOCK_NEUTRAL; break;
            }

            health -= move.damage / 10;
            v_x += (is_lefthand_player ? -move.knockback_x : move.knockback_x) / 10;
            v_y += (is_lefthand_player ? -move.knockback_y : move.knockback_y) / 10;
            counter = 10;

            return;
        }
    }

    // check if can be hit
    if (i_frames == 0)
    {
        i_frames = 10;

        health -= move.damage;
        v_x += is_lefthand_player ? -move.knockback_x : move.knockback_x;
        v_y += is_lefthand_player ? -move.knockback_y : move.knockback_y;

        if (other_player->move.is_hard_knockdown)
            state = KNOCKED_DOWN;
        else if (other_player->move.is_grab)
            state = GRABBED;
        else
            switch (state)
            {
                case CROUCHING: state = STUN_CROUCH; break;
                case AIRBORNE: state = STUN_AIR; break;
                default: state = STUN_NEUTRAL; break;
            }

        counter = move.stun;
        if (move.is_grab)
        {
            state                  = GRABBED;
            other_player->grab_hit = true;
        }

        return;
    }
}

void Player::update_state ()
{
    update_move_states ();

    // select update function based upon state
    switch (state)
    {
        case NEUTRAL: update_neutral (); break;
        case CROUCHING: update_crouching (); break;
        case AIRBORNE: update_airborne (); break;

        case WALK_FORWARD: update_walk_forward (); break;
        case WALK_BACKWARD: update_walk_backward (); break;
        case SPRINT: update_sprint (); break;
        case BACKDASH: update_backdash (); break;

        case BLOCK_NEUTRAL:
        case BLOCK_CROUCH:
        case BLOCK_AIR:
        case STUN_NEUTRAL:
        case STUN_CROUCH:
        case STUN_AIR:
        case KNOCKED_DOWN: update_stun (); break;
        case GRABBED: update_frame_animation (); break;

        case NEUTRAL_TO_CROUCH: update_neutral_to_crouch (); break;
        case NEUTRAL_TO_AIR: update_neutral_to_air (); break;
        case NEUTRAL_TO_FORWARD_AIR: update_neutral_to_forward_air (); break;
        case NEUTRAL_TO_BACKWARD_AIR: update_neutral_to_backward_air (); break;
        case CROUCH_TO_NEUTRAL: update_crouch_to_neutral (); break;

        case MOVE_A: update_standard_move (); break;
        case MOVE_B: update_standard_move (); break;
        case MOVE_C: update_standard_move (); break;
        case MOVE_C_GRAB: update_grab (); break;
        case MOVE_D: update_standard_move (); break;
    }

    std::cout << state << "\r";
}

void Player::update_physics ()
{
    // apply gravity and momentum to the player
    x += v_x;
    y += -v_y;

    v_y -= gravity;


    // todo: change this from hardcoded floor
    if (y >= ground)
    {
        y = ground;
        v_x *= 1 - friction;
        v_y = 0;
    }
}

void Player::update_move_states ()
{
    // grabs cannot be overidden
    if (state == GRABBED) return;

    // check for normal moves
    // only override movement
    if (state < MOVE_FIRST_STATE)
    {
        if (is_pressed (A) && !is_pressed (A, input_history.at (1)))
        {
            switch (state)
            {
                case CROUCHING: break;    // c. a
                case AIRBORNE: break;     // j. a
                default:
                    move  = Move {10, 10, 0, 10, 10, false, false, MID, NEUTRAL};
                    state = MOVE_A;
                    break;    // a
            }
        }

        if (is_pressed (B) && !is_pressed (B, input_history.at (1)))
        {
            switch (state)
            {
                case CROUCHING: break;    // c. b
                case AIRBORNE: break;     // j. b
                default:
                    move  = {10, 10, 0, 5, 20, false, false, LOW, NEUTRAL};
                    state = MOVE_B;
                    break;    // b
            }
        }

        if (is_pressed (C) && !is_pressed (C, input_history.at (1)))
        {
            switch (state)
            {
                case CROUCHING: break;    // c. c
                case AIRBORNE: break;     // j. c
                default:
                    move  = {0, 0, 0, 0, 11, false, true, MID, NEUTRAL, {Move {50, 0, 20, 120, 51}}};
                    state = MOVE_C;
                    break;    // c
            }
        }

        if (is_pressed (D) && !is_pressed (D, input_history.at (1)))
        {
            switch (state)
            {
                case CROUCHING: break;    // c. d
                case AIRBORNE: break;     // j. d
                default:
                    move  = {10, 10, 0, 120, 26, true, false, HIGH, NEUTRAL};
                    state = MOVE_D;
                    break;    // d
            }
        }
    }
}

void Player::update_movement_states ()
{
    if (state < TRANSITION_FIRST_STATE)
    {
        // jumping
        if (is_pressed (UP))
        {
            if (is_pressed (LEFT) && is_lefthand_player)
                state = NEUTRAL_TO_BACKWARD_AIR;
            else if (is_pressed (LEFT) && !is_lefthand_player)
                state = NEUTRAL_TO_FORWARD_AIR;
            else if (is_pressed (RIGHT) && is_lefthand_player)
                state = NEUTRAL_TO_FORWARD_AIR;
            else if (is_pressed (RIGHT) && !is_lefthand_player)
                state = NEUTRAL_TO_BACKWARD_AIR;
            else
                state = NEUTRAL_TO_AIR;
        }

        // crouching
        else if (is_pressed (DOWN))
        {
            state = NEUTRAL_TO_CROUCH;
        }
    }
}

void Player::update_idle_animation ()
{
    texture = animations.at (state).get_frame (frame_counter / 10);

    hitboxes  = animations.at (state).get_hitboxes (frame_counter / 10);
    hurtboxes = animations.at (state).get_hurtboxes (frame_counter / 10);

    SDL_SetTextureAlphaMod (texture, i_frames == 0 ? 255 : 40);
}

void Player::update_frame_animation ()
{
    texture = animations.at (state).get_frame (counter);

    hitboxes  = animations.at (state).get_hitboxes (counter);
    hurtboxes = animations.at (state).get_hurtboxes (counter);

    SDL_SetTextureAlphaMod (texture, i_frames == 0 ? 255 : 40);
}

void Player::update_collision ()
{
    // update collision rect
    collision = {-35, -300, 70, 300};

    switch (state)
    {
        case CROUCHING:
            collision.h = 200;
            collision.y = -200;
            break;
        case AIRBORNE: collision.h = 100; break;
        default: collision.h = 300; break;
    }

    collision.x *= (is_lefthand_player ? 1 : -1);
    collision.w *= (is_lefthand_player ? 1 : -1);

    collision.x += x;
    collision.y += y;

    if (collision.w < 0)
    {
        collision.x += collision.w;
        collision.w = fabs (collision.w);
    }

    // if against a wall, stop moving
    if (collision.x < left_wall)
    {
        x           = left_wall + fabs (collision.x - x);
        collision.x = left_wall;
    }
    else if (collision.x + collision.w > right_wall)
    {
        x           = right_wall - fabs (collision.x - x + collision.w);
        collision.x = right_wall - collision.w;
    }

    // if there is a collision, move half the difference (other player moves other half)
    else if (rect_intersect (collision, other_player->collision))
    {
        if (!is_lefthand_player)
            x += (other_player->collision.x + other_player->collision.w - collision.x) / 2;
        else
            x -= (collision.x + collision.w - other_player->collision.x) / 2;
    }
}

void Player::update_hitboxes ()
{
    // frame selection done is update_*_animation

    if (!is_lefthand_player)
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

bool Player::find_input_string (std::vector<unsigned long> pattern, int fuzziness)
{
    int  current_history_index    = 0;
    bool found_end_of_repetitions = false;

    for (auto & key : pattern)
    {
        if (input_history.at (current_history_index) != key) return false;

        found_end_of_repetitions = false;

        for (int i = 0; i < fuzziness; i++)
        {
            if (current_history_index > input_history.size ()) break;

            if (input_history.at (current_history_index++) != key)
            {
                found_end_of_repetitions = true;
                break;
            }
        }

        if (!found_end_of_repetitions) return false;
    }

    return true;
}

void Player::update_neutral ()
{
    // walking / sprinting
    if ((is_pressed (LEFT) || is_pressed (RIGHT)))
    {
        unsigned long direction         = is_pressed (LEFT) ? LEFT : RIGHT;
        bool          is_local_lefthand = direction == LEFT ? is_lefthand_player : !is_lefthand_player;

        if (y >= ground)
        {
            // sprint start
            if (state == SPRINT) state = is_local_lefthand ? NEUTRAL : SPRINT;

            // check for sprint start
            else if (find_input_string ({direction, 0, direction}))
            {
                state = is_local_lefthand ? BACKDASH : SPRINT;
                input_history.clear ();
            }

            // check for walking
            else
                state = is_local_lefthand ? WALK_BACKWARD : WALK_FORWARD;
        }
    }

    update_movement_states ();
    update_idle_animation ();
}

void Player::update_crouching ()
{
    if (!is_pressed (DOWN)) state = CROUCH_TO_NEUTRAL;

    update_idle_animation ();
}

void Player::update_airborne ()
{
    if (y >= ground) state = NEUTRAL;

    update_idle_animation ();
}

void Player::update_walk_forward ()
{
    x += is_lefthand_player ? walk_speed : -walk_speed;
    if (!is_pressed (is_lefthand_player ? RIGHT : LEFT)) state = NEUTRAL;

    update_movement_states ();
    update_idle_animation ();
}

void Player::update_walk_backward ()
{
    x += is_lefthand_player ? -reverse_walk_speed : reverse_walk_speed;
    if (!is_pressed (is_lefthand_player ? LEFT : RIGHT)) state = NEUTRAL;

    update_movement_states ();
    update_idle_animation ();
}

void Player::update_sprint ()
{
    x += is_lefthand_player ? sprint_speed : -sprint_speed;

    if (!is_pressed (is_lefthand_player ? RIGHT : LEFT))
        state = NEUTRAL;
    else
        state = SPRINT;

    update_movement_states ();
    update_idle_animation ();
}

void Player::update_backdash ()
{
    if (counter > time_to_backdash)
    {
        state   = NEUTRAL;
        counter = 0;
    }
    else
    {
        x += is_lefthand_player ? -backdash : backdash;
        counter++;
    }

    update_frame_animation ();
}

void Player::update_stun ()
{
    if (counter <= 0)
    {
        state   = state == BLOCK_CROUCH ? CROUCHING : NEUTRAL;
        counter = 0;
    }
    else
    {
        counter--;

        // change stun when landing
        if (y >= ground) switch (state)
            {
                case BLOCK_AIR: state = BLOCK_NEUTRAL; break;
                case STUN_AIR: state = STUN_NEUTRAL; break;
            }
    }

    update_frame_animation ();
}

void Player::update_neutral_to_crouch ()
{
    if (counter > time_to_crouch)
    {
        state   = CROUCHING;
        counter = 0;
    }
    else
        counter++;

    update_frame_animation ();
}

void Player::update_neutral_to_air ()
{
    if (counter > time_to_jump)
    {
        state   = AIRBORNE;
        v_y     = jump_height;
        counter = 0;
    }
    else
        counter++;

    update_frame_animation ();
}

void Player::update_neutral_to_forward_air ()
{
    if (counter > time_to_jump)
    {
        state   = AIRBORNE;
        v_x     = is_lefthand_player ? 15 : -15;
        v_y     = jump_height;
        counter = 0;
    }
    else
        counter++;

    update_frame_animation ();
}

void Player::update_neutral_to_backward_air ()
{
    if (counter > time_to_jump)
    {
        state   = AIRBORNE;
        v_x     = is_lefthand_player ? -15 : 15;
        v_y     = jump_height;
        counter = 0;
    }
    else
        counter++;

    update_frame_animation ();
}

void Player::update_crouch_to_neutral ()
{
    if (counter > time_to_stand)
    {
        state   = NEUTRAL;
        counter = 0;
    }
    else
        counter++;

    update_frame_animation ();
}

void Player::update_standard_move ()
{
    if (move.is_grab && grab_hit)
    {
        grab_hit = false;
        state += 1;
        counter = 0;

        if (move.successful_grab_move.size () > 0) { move = move.successful_grab_move.at (0); }
    }

    if (counter >= move.frame_length)
    {
        state   = move.final_state;
        counter = 0;
    }
    else
    {
        counter++;
    }

    update_frame_animation ();
}

void Player::update_grab ()
{
    if (counter >= move.frame_length)
    {
        state                  = move.final_state;
        counter                = 0;
        other_player->i_frames = 0;
        other_player->hit (move);
        other_player->state = KNOCKED_DOWN;
    }
    else
    {
        counter++;
    }

    update_frame_animation ();

    if (hitboxes.size () > 0)
    {
        other_player->x = ((is_lefthand_player ? 1 : -1) * hitboxes.at (0).x) + x;
        other_player->y = hitboxes.at (0).y + y + 200;
    }
}
