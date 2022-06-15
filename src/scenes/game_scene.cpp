#include "scenes/game_scene.hpp"

GameScene::GameScene (int width, int height)
    : Scene (width, height), ground_y (height * 0.8)
{
}

bool GameScene::init (SDL_Renderer * renderer)
{
    // Initialization flag
    bool success = true;

    players.push_back (Player {(float) ground_y});
    players.push_back (Player {(float) ground_y});

    for (auto & player : players) player.load_textures (renderer);

    players.at (1).x = 500;

    return success;
}

bool GameScene::step_event (SDL_Event & e)
{
    // std::cout << e.type << "\n";

    switch (e.type)
    {
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                // 1 for down, -1 for up
                case SDLK_w:
                case SDLK_UP: players.at (1).directional_input (1, 0, 0, 0); break;
                case SDLK_s:
                case SDLK_DOWN: players.at (1).directional_input (0, 1, 0, 0); break;
                case SDLK_a:
                case SDLK_LEFT: players.at (1).directional_input (0, 0, 1, 0); break;
                case SDLK_d:
                case SDLK_RIGHT: players.at (1).directional_input (0, 0, 0, 1); break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                // 1 for down, -1 for up
                case SDLK_w:
                case SDLK_UP: players.at (1).directional_input (-1, 0, 0, 0); break;
                case SDLK_s:
                case SDLK_DOWN: players.at (1).directional_input (0, -1, 0, 0); break;
                case SDLK_a:
                case SDLK_LEFT: players.at (1).directional_input (0, 0, -1, 0); break;
                case SDLK_d:
                case SDLK_RIGHT: players.at (1).directional_input (0, 0, 0, -1); break;
            }
            break;
        case SDL_JOYBUTTONDOWN:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0: players.at (0).button_input (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // x
                    case 1: players.at (0).button_input (0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // circle
                    case 2: players.at (0).button_input (0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // square
                    case 3: players.at (0).button_input (0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // triangle
                    case 4: players.at (0).button_input (0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); break;     // select
                    case 5: players.at (0).button_input (0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0); break;     // logo
                    case 6: players.at (0).button_input (0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0); break;     // start
                    case 7: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0); break;     // l stick
                    case 8: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0); break;     // r stick
                    case 9: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0); break;     // l bumper
                    case 10: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0); break;    // r bumper

                    case 11: players.at (0).directional_input (1, 0, 0, 0); break;    // up
                    case 12: players.at (0).directional_input (0, 1, 0, 0); break;    // down
                    case 13: players.at (0).directional_input (0, 0, 1, 0); break;    // left
                    case 14: players.at (0).directional_input (0, 0, 0, 1); break;    // right
                }
            }
            break;
        case SDL_JOYBUTTONUP:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0: players.at (0).button_input (-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // x
                    case 1: players.at (0).button_input (0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // circle
                    case 2: players.at (0).button_input (0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // square
                    case 3: players.at (0).button_input (0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0); break;     // triangle
                    case 4: players.at (0).button_input (0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0); break;     // select
                    case 5: players.at (0).button_input (0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0); break;     // logo
                    case 6: players.at (0).button_input (0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0); break;     // start
                    case 7: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0); break;     // l stick
                    case 8: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0); break;     // r stick
                    case 9: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0); break;     // l bumper
                    case 10: players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0); break;    // r bumper

                    case 11: players.at (0).directional_input (-1, 0, 0, 0); break;    // up
                    case 12: players.at (0).directional_input (0, -1, 0, 0); break;    // down
                    case 13: players.at (0).directional_input (0, 0, -1, 0); break;    // left
                    case 14: players.at (0).directional_input (0, 0, 0, -1); break;    // right
                }
            }
            break;
        case SDL_JOYHATMOTION:
            // Motion on controller 0
            if (e.jhat.which == 0)
            {
                switch (e.jhat.value)
                {
                    // 1 for down, -1 for up
                    case SDL_HAT_LEFTUP: players.at (0).directional_input (1, -1, 1, -1); break;
                    case SDL_HAT_UP: players.at (0).directional_input (1, -1, -1, -1); break;
                    case SDL_HAT_RIGHTUP: players.at (0).directional_input (1, -1, -1, 1); break;
                    case SDL_HAT_LEFT: players.at (0).directional_input (-1, -1, 1, -1); break;
                    case SDL_HAT_CENTERED: players.at (0).directional_input (-1, -1, -1, -1); break;
                    case SDL_HAT_RIGHT: players.at (0).directional_input (-1, -1, -1, 1); break;
                    case SDL_HAT_LEFTDOWN: players.at (0).directional_input (-1, 1, 1, -1); break;
                    case SDL_HAT_DOWN: players.at (0).directional_input (-1, 1, -1, -1); break;
                    case SDL_HAT_RIGHTDOWN: players.at (0).directional_input (-1, 1, -1, 1); break;
                }
            }
            break;
        case SDL_JOYAXISMOTION:
            // if (fabs (e.jaxis.value) > JOYSTICK_DEADZONE) std::cout << (int) e.jaxis.axis << "\n";

            // Motion on controller 0
            if (e.jaxis.which == 0)
            {
                // X axis motion on left stick
                if (e.jaxis.axis == 0)
                {
                    joystick_x = e.jaxis.value;
                    if (abs (joystick_x) > JOYSTICK_DEADZONE)
                    {
                        if (joystick_x > 0)
                            players.at (0).directional_input (0, 0, -1, 1);
                        else
                            players.at (0).directional_input (0, 0, 1, -1);
                    }
                    else
                    {
                        players.at (0).directional_input (0, 0, -1, -1);
                    }
                }
                // Y axis motion on left stick
                else if (e.jaxis.axis == 1)
                {
                    joystick_y = e.jaxis.value;
                    if (abs (joystick_y) > JOYSTICK_DEADZONE)
                    {
                        if (joystick_y > 0)
                            players.at (0).directional_input (-1, 1, 0, 0);
                        else
                            players.at (0).directional_input (1, -1, 0, 0);
                    }
                    else
                    {
                        players.at (0).directional_input (-1, -1, 0, 0);
                    }
                }
                // X axis motion on right stick
                else if (e.jaxis.axis == 2)
                {
                }
                // Y axis motion on right stick
                else if (e.jaxis.axis == 3)
                {
                }
                // left trigger
                else if (e.jaxis.axis == 4)
                {
                    if (e.jaxis.value > -28000)
                        players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
                    else
                        players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0);
                }
                // right trigger
                else if (e.jaxis.axis == 5)
                {
                    if (e.jaxis.value > -28000)
                        players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
                    else
                        players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);
                }
            }
            break;
    }

    // std::cout << joystick_x << "," << joystick_y << "               \r";

    return quit;
}

void GameScene::step_scene ()
{
    // player logic
    for (auto & player : players) player.update ();

    // side checks
    if (players[0].x > players[1].x)
    {
        players[0].is_left = true;
        players[1].is_left = false;
    }
    else
    {
        players[1].is_left = true;
        players[0].is_left = false;
    }

    // // hit detection

    bool hit = false;

    // p1 hitting p2
    for (auto & hitbox : players[0].hitboxes)
    {
        for (auto & hurtbox : players[1].hurtboxes)
        {
            // if there is a hit
            if (rect_intersect (hitbox, hurtbox))
            {
                players.at (1).health -= players.at (0).current_move_damage;
                // std::cout << "p1 hit p2 for " << players.at (0).current_move_damage << " damage, new health of p2 is
                // "
                //          << players.at (1).health << "\n";
                players.at (0).current_move_damage = 0;
                hit                                = true;
            }

            if (hit) break;
        }
        if (hit) break;
    }

    hit = false;

    // p2 hitting p1
    for (auto & hitbox : players[1].hitboxes)
    {
        for (auto & hurtbox : players[0].hurtboxes)
        {
            // if there is a hit
            if (SDL_IntersectRect (&hitbox, &hurtbox, NULL))
            {
                // std::cout << "p2 hit p1 for "
                //          << "\n";
                players.at (0).health -= players.at (1).current_move_damage;
                players.at (1).current_move_damage = 0;
                hit                                = true;
            }
            if (hit) break;
        }
        if (hit) break;
    }
}

void GameScene::step_render (SDL_Window * window, SDL_Renderer * renderer, int & width, int & height)
{
    // draw players
    for (auto & player : players)
    {
        // // Player
        SDL_RendererFlip flip = {};
        if (player.is_left) flip = SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx (renderer, player.texture, NULL, &player.render_area, 0, NULL, flip);

        // // Debug lines
        // floor
        SDL_SetRenderDrawColor (renderer, 40, 40, 0, 0xFF);
        SDL_RenderDrawLine (renderer, 0, ground_y, width, ground_y);

        // player position
        SDL_SetRenderDrawColor (renderer, 255, 255, 255, 0xFF);
        SDL_RenderDrawLine (renderer, player.x + 10, player.y, player.x - 10, player.y);
        SDL_RenderDrawLine (renderer, player.x, player.y + 10, player.x, player.y - 10);

        // player hurtboxes
        SDL_SetRenderDrawColor (renderer, 0, 180, 0, 0xFF);
        for (auto & hurtbox : player.hurtboxes) { SDL_RenderDrawRect (renderer, &hurtbox); }

        // player hitboxes
        SDL_SetRenderDrawColor (renderer, 180, 0, 0, 0xFF);
        for (auto & hitbox : player.hitboxes) { SDL_RenderDrawRect (renderer, &hitbox); }
    }

    // draw UI
    SDL_Rect player1_healthbar = {width / 2, 0, -(width / 2) * (players.at (0).health / players.at (0).max_health), 10};
    SDL_Rect player2_healthbar = {width / 2, 0, (width / 2) * (players.at (1).health / players.at (1).max_health), 10};

    SDL_SetRenderDrawColor (renderer, 0, 255, 0, 0xFF);
    SDL_RenderFillRect (renderer, &player1_healthbar);
    SDL_RenderFillRect (renderer, &player2_healthbar);

    SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect (renderer, &player1_healthbar);
    SDL_RenderDrawRect (renderer, &player2_healthbar);
}

void GameScene::close ()
{
    for (auto & player : players) player.close_textures ();
}
