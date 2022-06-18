#include "players/player.hpp"

Player::Player (float floor) : floor_level (floor), y (floor)
{
    for (int i = 0; i < 64; i++) directional_input_history.push_back (0);
    for (int i = 0; i < 64; i++) button_input_history.push_back (0);

    dst_area = SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                         (int) texture_height};

    // hurtboxes
    std::ifstream hiurt_stream ("res/config/debug/hiurtboxes.json");

    auto hiurtboxes = nlohmann::json::parse (hiurt_stream);

    hiurt_stream.close ();

    for (auto & state : hiurtboxes.at ("hurtboxes"))
    {
        std::vector<std::vector<SDL_Rect>> this_state;
        for (auto & frame : state)
        {
            std::vector<SDL_Rect> this_frame;
            for (auto & hiurtbox : frame)
            {
                std::vector<int> temp;

                for (auto & val : hiurtbox) temp.push_back ((int) val);

                this_frame.push_back (SDL_Rect {temp.at (0), temp.at (1), temp.at (2), temp.at (3)});
            }
            this_state.push_back (this_frame);
        }
        state_hurtboxes.push_back (this_state);
    }

    for (auto & state : hiurtboxes.at ("hitboxes"))
    {
        std::vector<std::vector<SDL_Rect>> this_state;
        for (auto & frame : state)
        {
            std::vector<SDL_Rect> this_frame;
            for (auto & hiurtbox : frame)
            {
                std::vector<int> temp;

                for (auto & val : hiurtbox) temp.push_back ((int) val);

                this_frame.push_back (SDL_Rect {temp.at (0), temp.at (1), temp.at (2), temp.at (3)});
            }
            this_state.push_back (this_frame);
        }
        state_hitboxes.push_back (this_state);
    }

    // Frame counts for each state animation
    for (auto & state : state_hurtboxes) { animation_frame_counts.push_back (state.size ()); }
    // animation_frame_counts = {11, 10, 1, 2, 2, 4, 5, 4, 11};

    // Move info
    moves.push_back ({10, 2, 6, 3});    // neutral a
}

void Player::update_state ()
{
    // if we are in a move, it is in control of states. See update_moves().
    if (state >= MOVE_FIRST_STATE) return;

    int new_state = is_airborne () ? AIRBORNE_STATE : NEUTRAL_STATE;

    // update with inputs
    switch (directional_current_input)
    {
        // no input
        case 0b0000:
        case 0b0011:
        case 0b1100:
        case 0b1111:
            switch (state)
            {
                case NEUTRAL_STATE: can_jump = true; break;
                case CROUCHING_STATE:
                    can_jump  = true;
                    new_state = CROUCH_TO_NEUTRAL_STATE;
                    break;
                case AIRBORNE_STATE: break;
                case WALK_FORWARD_STATE:
                case WALK_BACKWARD_STATE: new_state = NEUTRAL_STATE; break;
                default: break;
            }
            break;

        // jumping
        // up
        case 0b1011:
        case 0b1000:
            switch (state)
            {
                case NEUTRAL_STATE:
                case WALK_FORWARD_STATE:
                case WALK_BACKWARD_STATE:
                    if (can_jump)
                    {
                        new_state      = NEUTRAL_TO_AIR_STATE;
                        jump_direction = 0;
                        can_jump       = false;
                    }
                    break;
                default: break;
            }
            break;
        // up left
        case 0b1010:
            switch (state)
            {
                case NEUTRAL_STATE:
                case WALK_FORWARD_STATE:
                case WALK_BACKWARD_STATE:
                    if (can_jump)
                    {
                        new_state      = NEUTRAL_TO_AIR_STATE;
                        jump_direction = -1;
                        can_jump       = false;
                    }
                    break;
                default: break;
            }
            break;
        // up right
        case 0b1001:
            switch (state)
            {
                case NEUTRAL_STATE:
                case WALK_FORWARD_STATE:
                case WALK_BACKWARD_STATE:
                    if (can_jump)
                    {
                        new_state      = NEUTRAL_TO_AIR_STATE;
                        jump_direction = 1;
                        can_jump       = false;
                    }
                    break;
                default: break;
            }
            break;

        // crouched
        case 0b0100:
        case 0b0111:
        case 0b0110:
        case 0b0101:
            switch (state)
            {
                case 0: new_state = NEUTRAL_TO_CROUCH_STATE; break;
                case 1: new_state = CROUCHING_STATE; break;
                case 2: break;
                case 3:
                case 4: new_state = NEUTRAL_TO_CROUCH_STATE; break;
                default: break;
            }
            break;

        // walking
        // left
        case 0b1110:
        case 0b0010:
            switch (state)
            {
                case NEUTRAL_STATE:
                case WALK_FORWARD_STATE:
                case WALK_BACKWARD_STATE:
                    new_state = is_left ? WALK_FORWARD_STATE : WALK_BACKWARD_STATE;
                    x = std::max (100.0f, (x - (is_left ? walk_speed : reverse_walk_speed)));
                    break;
                default: break;
            }
            break;
        // right
        case 0b1101:
        case 0b0001:
            switch (state)
            {
                case NEUTRAL_STATE:
                case WALK_FORWARD_STATE:
                case WALK_BACKWARD_STATE:
                    new_state = is_left ? WALK_BACKWARD_STATE : WALK_FORWARD_STATE;
                    x         = std::min (1920.f - 100, x + (is_left ? reverse_walk_speed : walk_speed));
                    break;
                default: break;
            }
            break;
    }

    // step transitional states
    switch (state)
    {
        case NEUTRAL_TO_CROUCH_STATE:
            if (counter >= 3)
                new_state = CROUCHING_STATE;
            else
            {
                new_state = NEUTRAL_TO_CROUCH_STATE;
                counter++;
            }
            break;
        case NEUTRAL_TO_AIR_STATE:
            if (counter >= 6)
            {
                new_state = AIRBORNE_STATE;
                v_y       = jump_height;
                v_x       = jump_direction * walk_speed;
            }
            else
            {
                new_state = NEUTRAL_TO_AIR_STATE;
                counter++;
            }
            break;
        case CROUCH_TO_NEUTRAL_STATE:
            if (counter >= 3)
                new_state = NEUTRAL_STATE;
            else
            {
                new_state = CROUCH_TO_NEUTRAL_STATE;
                counter++;
            }
            break;

        default: break;
    }

    // set new state and reset state counter
    if (new_state != state)
    {
        state   = new_state;
        counter = 0;
    }
}

void Player::update_moves ()
{
    // see if we are in a move
    if (state < MOVE_FIRST_STATE) return;

    switch (MOVE_FIRST_STATE)
    {
        case MOVE_A_STATE:
            if (counter == 0) { move = moves[MOVE_A_STATE - MOVE_FIRST_STATE]; }
            else if (counter < move.startup)
            {
                // startup
            }
            else if (counter < move.startup + move.active)
            {
            }
            else if (counter < move.startup + move.active + move.recovery)
            {
                // recovery
            }
            else if (counter >= move.startup + move.active + move.recovery)
            {
                // quit
                state   = NEUTRAL_STATE;
                counter = 0;
            }

            counter++;
            break;
    }
}

void Player::update_physics ()
{
    x += v_x;
    y -= v_y;

    v_y -= gravity;

    if ((abs (floor_level - round (y)) <= 0.01) || y > floor_level)
    {
        y   = floor_level;
        v_x = 0;
        v_y = 0;
    }
}

void Player::update_hitboxes ()
{
    if (state > 4)
    {
        hurtboxes = state_hurtboxes[state][((int) floor (counter)) % state_hurtboxes[state].size ()];
        hitboxes  = state_hitboxes[state][((int) floor (counter)) % state_hitboxes[state].size ()];
    }
    else
    {
        hurtboxes = state_hurtboxes[state][((int) floor (frame_counter * 0.1)) % state_hurtboxes[state].size ()];
        hitboxes  = state_hitboxes[state][((int) floor (frame_counter * 0.1)) % state_hitboxes[state].size ()];
    }

    if (is_left)
    {
        for (int i = 0; i < hurtboxes.size (); i++)
        {
            hurtboxes.at (i).x = -hurtboxes.at (i).x;
            hurtboxes.at (i).w = -hurtboxes.at (i).w;
        }

        for (int i = 0; i < hitboxes.size (); i++)
        {
            hitboxes.at (i).x = -hitboxes.at (i).x;
            hitboxes.at (i).w = -hitboxes.at (i).w;
        }
    }

    for (int i = 0; i < hurtboxes.size (); i++)
    {
        hurtboxes.at (i).x = hurtboxes.at (i).x + x;
        hurtboxes.at (i).y = hurtboxes.at (i).y + y;
    }

    for (int i = 0; i < hitboxes.size (); i++)
    {
        hitboxes.at (i).x = hitboxes.at (i).x + x;
        hitboxes.at (i).y = hitboxes.at (i).y + y;
    }
}

void Player::update_textures ()
{
    if (state > 4)
    {
        // use new texture each frame (60 fps) for transitions and moves
        src_area.x = texture_width * (((int) floor (counter)) % animation_frame_counts[state]);
        src_area.y = texture_height * state;
    }
    else
    {
        // use global frame counter (6 fps) for idle animations
        src_area.x = texture_width * (((int) floor (frame_counter * 0.1)) % animation_frame_counts[state]);
        src_area.y = texture_height * state;
    }

    dst_area.x = (int) (x - (dst_area.w / 2));
    dst_area.y = (int) (y - dst_area.h);

    frame_counter++;
}

void Player::directional_input (int up, int down, int left, int right)
{
    if (up == 1) directional_current_input |= 0b1000;
    if (down == 1) directional_current_input |= 0b0100;
    if (left == 1) directional_current_input |= 0b0010;
    if (right == 1) directional_current_input |= 0b0001;

    if (up == -1) directional_current_input &= 0b0111;
    if (down == -1) directional_current_input &= 0b1011;
    if (left == -1) directional_current_input &= 0b1101;
    if (right == -1) directional_current_input &= 0b1110;
}

void Player::button_input (int x, int circle, int square, int triangle, int select, int logo, int start, int l_stick,
                           int r_stick, int l_bumper, int r_bumper, int l_trigger, int r_trigger)
{
    /*std::cout << "\n\n\n\n\n\n";
    for (auto entry : button_input_history) std::cout << std::bitset<16> (entry) << "\n";*/

    // process moves ending in square
    if (square == 1 && !input_square (button_current_input))
    {
        if (state == NEUTRAL_STATE)
        {
            state   = MOVE_A_STATE;
            counter = 0;
        }
    }


    if (x == 1) button_current_input |= 0b1000000000000000;
    if (circle == 1) button_current_input |= 0b0100000000000000;
    if (square == 1) button_current_input |= 0b0010000000000000;
    if (triangle == 1) button_current_input |= 0b0001000000000000;
    if (select == 1) button_current_input |= 0b0000100000000000;
    if (logo == 1) button_current_input |= 0b0000010000000000;
    if (start == 1) button_current_input |= 0b0000001000000000;
    if (l_stick == 1) button_current_input |= 0b0000000100000000;
    if (r_stick == 1) button_current_input |= 0b0000000010000000;
    if (l_bumper == 1) button_current_input |= 0b0000000001000000;
    if (r_bumper == 1) button_current_input |= 0b0000000000100000;
    if (l_trigger == 1) button_current_input |= 0b0000000000010000;
    if (r_trigger == 1) button_current_input |= 0b0000000000001000;

    if (x == -1) button_current_input &= 0b0111111111111111;
    if (circle == -1) button_current_input &= 0b1011111111111111;
    if (square == -1) button_current_input &= 0b1101111111111111;
    if (triangle == -1) button_current_input &= 0b1110111111111111;
    if (select == -1) button_current_input &= 0b1111011111111111;
    if (logo == -1) button_current_input &= 0b1111101111111111;
    if (start == -1) button_current_input &= 0b1111110111111111;
    if (l_stick == -1) button_current_input &= 0b1111111011111111;
    if (r_stick == -1) button_current_input &= 0b1111111101111111;
    if (l_bumper == -1) button_current_input &= 0b1111111110111111;
    if (r_bumper == -1) button_current_input &= 0b1111111111011111;
    if (l_trigger == -1) button_current_input &= 0b1111111111101111;
    if (r_trigger == -1) button_current_input &= 0b1111111111110111;
}

void Player::load_textures (SDL_Renderer * renderer)
{
    texture = IMG_LoadTexture (renderer, "res/atlas/debug.png");

    src_area = SDL_Rect {0, 0, texture_width, texture_height};
    dst_area = SDL_Rect {0, 0, texture_width, texture_height};
}

void Player::close_textures ()
{
    // Free loaded images

    SDL_DestroyTexture (texture);
    texture = NULL;
}

void Player::update ()
{
    directional_input_history.insert (directional_input_history.begin (), directional_current_input);
    directional_input_history.pop_back ();

    button_input_history.insert (button_input_history.begin (), button_current_input);
    button_input_history.pop_back ();

    update_state ();
    update_moves ();
    update_physics ();
    update_hitboxes ();
    update_textures ();
}

bool Player::is_crouched () { return !is_airborne () && input_down (directional_current_input) && state != 3; }

bool Player::is_blocking ()
{
    // todo: make this change depending on which side you're on
    return input_left (directional_current_input);
}

bool Player::is_airborne () { return y < floor_level; }

bool Player::input_up (unsigned int input) { return (input & 0b1000) >> 3 && !((input & 0b0100) >> 2); }
bool Player::input_down (unsigned int input) { return (input & 0b0100) >> 2 && !((input & 0b1000) >> 3); }
bool Player::input_left (unsigned int input) { return ((input & 0b0010) >> 1) && !(input & 0b0001); }
bool Player::input_right (unsigned int input) { return (input & 0b0001) && !((input & 0b0010) >> 1); }

bool Player::input_x (unsigned int input) { return ((input & 0b1000000000000000) >> 15); }
bool Player::input_circle (unsigned int input) { return ((input & 0b0100000000000000) >> 14); }
bool Player::input_square (unsigned int input) { return ((input & 0b0010000000000000) >> 13); }
bool Player::input_triangle (unsigned int input) { return ((input & 0b0001000000000000) >> 12); }
bool Player::input_select (unsigned int input) { return ((input & 0b0000100000000000) >> 11); }
bool Player::input_logo (unsigned int input) { return ((input & 0b0000010000000000) >> 10); }
bool Player::input_start (unsigned int input) { return ((input & 0b0000001000000000) >> 9); }
bool Player::input_l_stick (unsigned int input) { return ((input & 0b0000000100000000) >> 8); }
bool Player::input_r_stick (unsigned int input) { return ((input & 0b0000000010000000) >> 7); }
bool Player::input_l_bumper (unsigned int input) { return ((input & 0b0000000001000000) >> 6); }
bool Player::input_r_bumper (unsigned int input) { return ((input & 0b0000000000100000) >> 5); }
bool Player::input_l_trigger (unsigned int input) { return ((input & 0b0000000000010000) >> 4); }
bool Player::input_r_trigger (unsigned int input) { return ((input & 0b0000000000001000) >> 3); }