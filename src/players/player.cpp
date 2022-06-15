#include "players/player.hpp"

Player::Player (float floor) : floor (floor), y (floor)
{
    for (int i = 0; i < 64; i++) directional_input_history.push_back (0);
    for (int i = 0; i < 64; i++) button_input_history.push_back (0);

    hurtboxes.push_back (SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                   (int) texture_height});
    render_area = SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                            (int) texture_height};

    // neutral
    state_hurtboxes.push_back ({{SDL_Rect {(int) (-58.0), (int) (-443), (int) 101, (int) 102},
                                 SDL_Rect {(int) (-73.0), (int) (-358), (int) 106, (int) 187},
                                 SDL_Rect {(int) (-60.0), (int) (-164), (int) 71, (int) 163}},
                                {SDL_Rect {(int) (-58.0), (int) (-433), (int) 101, (int) 102},
                                 SDL_Rect {(int) (-73.0), (int) (-358), (int) 106, (int) 187},
                                 SDL_Rect {(int) (-60.0), (int) (-164), (int) 71, (int) 163}}});
    state_hitboxes.push_back ({{}});

    // crouched
    state_hurtboxes.push_back ({{SDL_Rect {(int) (-64.0), (int) (-358), (int) 101, (int) 103},
                                 SDL_Rect {(int) (-71.0), (int) (-226), (int) 97, (int) 137},
                                 SDL_Rect {(int) (-95.0), (int) (-82), (int) 136, (int) 81}}});
    state_hitboxes.push_back ({{}});

    // airborne
    state_hurtboxes.push_back ({{SDL_Rect {(int) (-88.0), (int) (-499), (int) 196, (int) 200},
                                 SDL_Rect {(int) (-61.0), (int) (-295), (int) 64, (int) 294}}});
    state_hitboxes.push_back ({{}});

    // walking forwards
    state_hurtboxes.push_back ({{SDL_Rect {(int) (-58.0), (int) (-443), (int) 101, (int) 102},
                                 SDL_Rect {(int) (-73.0), (int) (-308), (int) 106, (int) 137},
                                 SDL_Rect {(int) (-60.0), (int) (-164), (int) 71, (int) 163}}});
    state_hitboxes.push_back ({{}});

    // walking backwards
    state_hurtboxes.push_back ({{SDL_Rect {(int) (-58.0), (int) (-443), (int) 101, (int) 102},
                                 SDL_Rect {(int) (-73.0), (int) (-308), (int) 106, (int) 137},
                                 SDL_Rect {(int) (-60.0), (int) (-164), (int) 71, (int) 163}}});
    state_hitboxes.push_back ({{}});

    // light punch
    state_hurtboxes.push_back ({{SDL_Rect {(int) (-58.0), (int) (-444), (int) 99, (int) 99},
                                 SDL_Rect {(int) (-58.0), (int) (-338), (int) 65, (int) 337},
                                 SDL_Rect {(int) (15.0), (int) (-342), (int) 60, (int) 109},
                                 SDL_Rect {(int) (53.0), (int) (-338), (int) 127, (int) 48}}});
    state_hitboxes.push_back ({{SDL_Rect {(int) (79.0), (int) (-329), (int) 86, (int) 27}}});
}

void Player::update_movement ()
{
    bool jumped_this_frame = false;
    int  updated_state     = 0;

    // movement
    switch (directional_current_input)
    {
        // no input
        case 0b0000:
        case 0b0011:
        case 0b1100:
        case 0b1111: break;
        // jumping
        case 0b1011:
        case 0b1000:
            if (!just_jumped && !is_airborne ()) apply_force (0, jump_height);
            jumped_this_frame = true;
            break;
        case 0b1010:
            if (!just_jumped && !is_airborne ()) apply_force (-walk_speed, jump_height);
            jumped_this_frame = true;
            break;
        case 0b1001:
            if (!just_jumped && !is_airborne ()) apply_force (walk_speed, jump_height);
            jumped_this_frame = true;
            break;
        // crouched
        case 0b0100:
        case 0b0111:
        case 0b0110:
        case 0b0101: break;
        // walking
        case 0b1110:
        case 0b0010:
            if (!is_airborne () && current_move == 0)
            {
                x -= is_left ? walk_speed : reverse_walk_speed;
                updated_state = is_left ? 3 : 4;
            }
            break;
        case 0b1101:
        case 0b0001:
            if (!is_airborne () && current_move == 0)
            {
                x += is_left ? reverse_walk_speed : walk_speed;
                updated_state = is_left ? 4 : 3;
            }
            break;
    }

    just_jumped = jumped_this_frame;

    if (is_airborne ()) updated_state = 2;
    if (is_crouched ()) updated_state = 1;

    current_state = updated_state;
}

void Player::update_physics ()
{
    x += v_x;
    y -= v_y;

    v_y -= gravity;

    if ((abs (floor - round (y)) <= 0.01) || y > floor)
    {
        y   = floor;
        v_x = 0;
        v_y = 0;
    }

    /*std::cout << x << "," << y << "[" << v_x << "," << v_y << "]"
              << "\n";*/
}

void Player::update_hitboxes ()
{
    if (current_move > 0 && current_move_startup == 0 && current_move_duration > 0)
    {
        hurtboxes = state_hurtboxes.at (4 + current_move).at (0);
        hitboxes  = state_hitboxes.at (4 + current_move).at (0);
    }
    else
    {
        hurtboxes = state_hurtboxes.at (current_state).at ((counter/10) % state_hurtboxes.at (current_state).size ());
        hitboxes  = state_hitboxes.at (current_state).at ((counter/10) % state_hitboxes.at (current_state).size ());
    }

    if (is_left) {
        for (int i = 0; i < hurtboxes.size (); i++)
        {
            hurtboxes.at (i).x = -hurtboxes.at (i).x;
            //hurtboxes.at (i).y = -hurtboxes.at (i).y;
            hurtboxes.at (i).w = -hurtboxes.at (i).w;
            //hurtboxes.at (i).h = -hurtboxes.at (i).h;
        }

        for (int i = 0; i < hitboxes.size (); i++)
        {
            hitboxes.at (i).x  = -hitboxes.at (i).x;
            //hitboxes.at (i).y  = -hitboxes.at (i).y;
            hitboxes.at (i).w  = -hitboxes.at (i).w;
            //hitboxes.at (i).h  = -hitboxes.at (i).h;
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
    if (current_move > 0 && current_move_startup == 0 && current_move_duration > 0)
    {
        // todo: add animations for moves
        texture     = textures.at (4 + current_move).at (0);
        render_area = render_areas.at (4 + current_move).at (0);
    }
    else
    {
        texture     = textures.at (current_state).at ((counter / 10) % textures.at (current_state).size ());
        render_area = render_areas.at (current_state).at ((counter / 10) % render_areas.at (current_state).size ());
    }

    render_area.x = (int) (x - (render_area.w / 2));
    render_area.y = (int) (y - render_area.h);

    counter++;
}

void Player::update_current_move ()
{
    if (current_move_startup > 0)
        current_move_startup--;
    else if (current_move_duration > 0)
        current_move_duration--;
    else if (current_move_recovery > 0)
        current_move_recovery--;
    else
    {
        current_move          = 0;
        current_move_priority = 0;
        current_move_damage   = 0;
    }
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

    /*std::cout << "\n\n\n\n\n\n";
    for (auto entry : button_input_history) std::cout << std::bitset<16> (entry) << "\n";*/


    // std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    //          << "x: \t\t" << ((button_current_input & 0b1000000000000000) >> 15) << "\n"
    //          << "circle: \t" << ((button_current_input & 0b0100000000000000) >> 14) << "\n"
    //          << "square: \t" << ((button_current_input & 0b0010000000000000) >> 13) << "\n"
    //          << "triangle: \t" << ((button_current_input & 0b0001000000000000) >> 12) << "\n"
    //          << "select: \t" << ((button_current_input & 0b0000100000000000) >> 11) << "\n"
    //          << "logo: \t\t" << ((button_current_input & 0b0000010000000000) >> 10) << "\n"
    //          << "start: \t\t" << ((button_current_input & 0b0000001000000000) >> 9) << "\n"
    //          << "l_stick: \t" << ((button_current_input & 0b0000000100000000) >> 8) << "\n"
    //          << "r_stick: \t" << ((button_current_input & 0b0000000010000000) >> 7) << "\n"
    //          << "l_bumper: \t" << ((button_current_input & 0b0000000001000000) >> 6) << "\n"
    //          << "r_bumper: \t" << ((button_current_input & 0b0000000000100000) >> 5) << "\n"
    //          << "l_trigger: \t" << ((button_current_input & 0b0000000000010000) >> 4) << "\n"
    //          << "r_trigger: \t" << ((button_current_input & 0b0000000000001000) >> 3) << "\n";

    if (square == 1 && !is_airborne () && !is_crouched () && current_move_priority < 1)
    {
        current_move          = 1;
        current_move_priority = 1;
        current_move_startup  = 2;
        current_move_duration = 6;
        current_move_recovery = 1;
        current_move_damage   = 10;
    }
}

void Player::apply_force (float dx, float dy)
{
    v_x += dx;
    v_y += dy;
}

void Player::load_textures (SDL_Renderer * renderer)
{
    textures.push_back ({IMG_LoadTexture (renderer, "res/debug/debug_neutral.png"),
                         IMG_LoadTexture (renderer, "res/debug/debug_neutral1.png"),
                         IMG_LoadTexture (renderer, "res/debug/debug_neutral2.png"),
                         IMG_LoadTexture (renderer, "res/debug/debug_neutral3.png")});
    textures.push_back ({IMG_LoadTexture (renderer, "res/debug/debug_crouch.png")});
    textures.push_back ({IMG_LoadTexture (renderer, "res/debug/debug_airborne.png")});
    textures.push_back ({IMG_LoadTexture (renderer, "res/debug/debug_walking_forwards.png"),
                         IMG_LoadTexture (renderer, "res/debug/debug_walking_forwards1.png")});
    textures.push_back ({IMG_LoadTexture (renderer, "res/debug/debug_walking_backwards.png"),
                         IMG_LoadTexture (renderer, "res/debug/debug_walking_backwards1.png"),
                         IMG_LoadTexture (renderer, "res/debug/debug_walking_backwards2.png")});

    // moves
    textures.push_back ({IMG_LoadTexture (renderer, "res/debug/debug_lp.png")});

    texture = textures.at (0).at (0);

    render_areas.push_back ({SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                       (int) texture_height}});
    render_areas.push_back ({SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                       (int) texture_height}});
    render_areas.push_back ({SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                       (int) texture_height}});
    render_areas.push_back ({SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                       (int) texture_height}});
    render_areas.push_back ({SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                       (int) texture_height}});

    // moves
    render_areas.push_back ({SDL_Rect {(int) (x - (texture_width / 2)), (int) (y - texture_height), (int) texture_width,
                                       (int) texture_height}});

    render_area = render_areas.at (0).at (0);
}

void Player::close_textures ()
{
    // Free loaded images
    for (auto vec : textures)
        for (auto t : vec)
        {
            SDL_DestroyTexture (t);
            t = NULL;
        }
}

void Player::update ()
{
    directional_input_history.insert (directional_input_history.begin (), directional_current_input);
    directional_input_history.pop_back ();

    button_input_history.insert (button_input_history.begin (), button_current_input);
    button_input_history.pop_back ();

    update_movement ();
    update_physics ();
    update_hitboxes ();
    update_textures ();
    update_current_move ();
}

bool Player::is_crouched () { return !is_airborne () && input_down (directional_current_input); }

bool Player::is_blocking ()
{
    // todo: make this change depending on which side you're on
    return input_left (directional_current_input);
}

bool Player::is_airborne () { return y < floor; }

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