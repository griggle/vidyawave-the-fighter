#include "scenes/game_scene.hpp"

GameScene::GameScene (int width, int height) : Scene (width, height), ground_y (900) {}

bool GameScene::init (SDL_Renderer * renderer)
{
    // Initialization flag
    bool success = true;

    players.push_back (new PlayerJohnDebug ());
    players.push_back (new PlayerJohnDebug ());

    players.at (0)->other_player = players.at (1);
    players.at (1)->other_player = players.at (0);

    players.at (0)->x = 1200;
    players.at (1)->x = 500;
    players.at (0)->y = ground_y;
    players.at (1)->y = ground_y;

    for (auto & player : players) player->load_states (renderer);

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
                case SDLK_w:
                case SDLK_UP: players.at (1)->input (UP, PRESSED); break;
                case SDLK_s:
                case SDLK_DOWN: players.at (1)->input (DOWN, PRESSED); break;
                case SDLK_a:
                case SDLK_LEFT: players.at (1)->input (LEFT, PRESSED); break;
                case SDLK_d:
                case SDLK_RIGHT: players.at (1)->input (RIGHT, PRESSED); break;
                case SDLK_j: players.at (1)->input (A, PRESSED); break;
                case SDLK_k: players.at (1)->input (B, PRESSED); break;
                case SDLK_l: players.at (1)->input (C, PRESSED); break;
                case SDLK_SEMICOLON: players.at (1)->input (D, PRESSED); break;

                case SDLK_F3: is_debug = !is_debug; break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                case SDLK_UP: players.at (1)->input (UP, RELEASED); break;
                case SDLK_s:
                case SDLK_DOWN: players.at (1)->input (DOWN, RELEASED); break;
                case SDLK_a:
                case SDLK_LEFT: players.at (1)->input (LEFT, RELEASED); break;
                case SDLK_d:
                case SDLK_RIGHT: players.at (1)->input (RIGHT, RELEASED); break;
                case SDLK_j: players.at (1)->input (A, RELEASED); break;
                case SDLK_k: players.at (1)->input (B, RELEASED); break;
                case SDLK_l: players.at (1)->input (C, RELEASED); break;
                case SDLK_SEMICOLON: players.at (1)->input (D, RELEASED); break;
            }
            break;
        case SDL_JOYBUTTONDOWN:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0: players.at (0)->input (A, PRESSED); break;    // x
                    case 1: players.at (0)->input (B, PRESSED); break;    // circle
                    case 2: players.at (0)->input (C, PRESSED); break;    // square
                    case 3: players.at (0)->input (D, PRESSED); break;    // triangle
                    case 4:                                               // select
                    case 5:                                               // logo
                    case 6:                                               // start
                    case 7:                                               // l stick
                    case 8:                                               // r stick
                    case 9:                                               // l bumper
                    case 10: break;                                       // r bumper

                    case 11: players.at (0)->input (UP, PRESSED); break;       // up
                    case 12: players.at (0)->input (DOWN, PRESSED); break;     // down
                    case 13: players.at (0)->input (LEFT, PRESSED); break;     // left
                    case 14: players.at (0)->input (RIGHT, PRESSED); break;    // right
                }
            }
            break;
        case SDL_JOYBUTTONUP:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0: players.at (0)->input (A, RELEASED); break;    // x
                    case 1: players.at (0)->input (B, RELEASED); break;    // circle
                    case 2: players.at (0)->input (C, RELEASED); break;    // square
                    case 3: players.at (0)->input (D, RELEASED); break;    // triangle
                    case 4:                                                // select
                    case 5:                                                // logo
                    case 6:                                                // start
                    case 7:                                                // l stick
                    case 8:                                                // r stick
                    case 9:                                                // l bumper
                    case 10: break;                                        // r bumper

                    case 11: players.at (0)->input (UP, RELEASED); break;       // up
                    case 12: players.at (0)->input (DOWN, RELEASED); break;     // down
                    case 13: players.at (0)->input (LEFT, RELEASED); break;     // left
                    case 14: players.at (0)->input (RIGHT, RELEASED); break;    // right
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
                    /*case SDL_HAT_LEFTUP: players.at (0).input (UP | LEFT, PRESSED); break;
                    case SDL_HAT_UP: players.at (0).input (UP, PRESSED); break;
                    case SDL_HAT_RIGHTUP: players.at (0).input (UP | RIGHT, PRESSED); break;
                    case SDL_HAT_LEFT: players.at (0).input (LEFT, PRESSED); break;
                    case SDL_HAT_CENTERED: players.at (0).input (UP, PRESSED); break;
                    case SDL_HAT_RIGHT: players.at (0).input (UP, PRESSED); break;
                    case SDL_HAT_LEFTDOWN: players.at (0).input (UP, PRESSED); break;
                    case SDL_HAT_DOWN: players.at (0).input (UP, PRESSED); break;
                    case SDL_HAT_RIGHTDOWN: players.at (0).input (UP, PRESSED); break;*/
                }
            }
            break;
        case SDL_JOYAXISMOTION:
            //// if (fabs (e.jaxis.value) > JOYSTICK_DEADZONE) std::cout << (int) e.jaxis.axis << "\n";

            //// Motion on controller 0
            // if (e.jaxis.which == 0)
            //{
            //    // X axis motion on left stick
            //    if (e.jaxis.axis == 0)
            //    {
            //        joystick_x = e.jaxis.value;
            //        if (abs (joystick_x) > JOYSTICK_DEADZONE)
            //        {
            //            if (joystick_x > 0)
            //                players.at (0).directional_input (0, 0, -1, 1);
            //            else
            //                players.at (0).directional_input (0, 0, 1, -1);
            //        }
            //        else
            //        {
            //            players.at (0).directional_input (0, 0, -1, -1);
            //        }
            //    }
            //    // Y axis motion on left stick
            //    else if (e.jaxis.axis == 1)
            //    {
            //        joystick_y = e.jaxis.value;
            //        if (abs (joystick_y) > JOYSTICK_DEADZONE)
            //        {
            //            if (joystick_y > 0)
            //                players.at (0).directional_input (-1, 1, 0, 0);
            //            else
            //                players.at (0).directional_input (1, -1, 0, 0);
            //        }
            //        else
            //        {
            //            players.at (0).directional_input (-1, -1, 0, 0);
            //        }
            //    }
            //    // X axis motion on right stick
            //    else if (e.jaxis.axis == 2)
            //    {
            //    }
            //    // Y axis motion on right stick
            //    else if (e.jaxis.axis == 3)
            //    {
            //    }
            //    // left trigger
            //    else if (e.jaxis.axis == 4)
            //    {
            //        if (e.jaxis.value > -28000)
            //            players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
            //        else
            //            players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0);
            //    }
            //    // right trigger
            //    else if (e.jaxis.axis == 5)
            //    {
            //        if (e.jaxis.value > -28000)
            //            players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
            //        else
            //            players.at (0).button_input (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1);
            //    }
            //}
            break;
    }

    // std::cout << joystick_x << "," << joystick_y << "               \r";

    return quit;
}

void GameScene::step_scene ()
{
    // player logic
    for (auto & player : players) player->update ();
}

void GameScene::step_render (SDL_Window * window, SDL_Renderer * renderer, int & width, int & height)
{
    // draw players
    for (auto & player : players)
    {
        // // Player
        SDL_SetRenderDrawColor (renderer, 0, 0, 180, 0xFF);
        SDL_RendererFlip flip = {};
        if (!player->is_left ()) flip = SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx (renderer, player->texture, NULL, &player->dst_area, 0, NULL, flip);

        if (is_debug)
        {
            // // Debug lines
            // hit and hurtboxes
            SDL_RenderDrawRect (renderer, &player->dst_area);
            SDL_SetRenderDrawColor (renderer, 180, 180, 0, 0xFF);
            SDL_RenderDrawRect (renderer, &player->collision);

            // floor and walls
            SDL_SetRenderDrawColor (renderer, 40, 40, 0, 0xFF);
            SDL_RenderDrawLine (renderer, 0, ground_y, width, ground_y);
            SDL_RenderDrawLine (renderer, 50, 0, 50, height);
            SDL_RenderDrawLine (renderer, 1870, 0, 1870, height);

            // player position
            SDL_SetRenderDrawColor (renderer, 255, 255, 255, 0xFF);
            SDL_RenderDrawLine (renderer, player->x + 10, player->y, player->x - 10, player->y);
            SDL_RenderDrawLine (renderer, player->x, player->y + 10, player->x, player->y - 10);


            // player hurtboxes
            if (player->guard == Player::NONE)
                SDL_SetRenderDrawColor (renderer, 0, 180, 0, 40);
            else
                SDL_SetRenderDrawColor (renderer, 0, 180, 180, 40);

            for (auto & hurtbox : player->hurtboxes) { SDL_RenderFillRect (renderer, &hurtbox); }
            
            if (player->guard == Player::NONE)
                SDL_SetRenderDrawColor (renderer, 0, 180, 0, 255);
            else
                SDL_SetRenderDrawColor (renderer, 0, 180, 180, 255);

            for (auto & hurtbox : player->hurtboxes) { SDL_RenderDrawRect (renderer, &hurtbox); }

            // player hitboxes
            SDL_SetRenderDrawColor (renderer, 180, 0, 0, 40);
            for (auto & hitbox : player->hitboxes) { SDL_RenderFillRect (renderer, &hitbox); }
            SDL_SetRenderDrawColor (renderer, 180, 0, 0, 255);
            for (auto & hitbox : player->hitboxes) { SDL_RenderDrawRect (renderer, &hitbox); }
        }
    }

    // draw UI
    SDL_Rect player1_healthbar
        = {width / 2, 0, -(width / 2) * (players.at (0)->health / players.at (0)->max_health), 10};
    SDL_Rect player2_healthbar
        = {width / 2, 0, (width / 2) * (players.at (1)->health / players.at (1)->max_health), 10};

    SDL_SetRenderDrawColor (renderer, 0, 255, 0, 0xFF);
    SDL_RenderFillRect (renderer, &player1_healthbar);
    SDL_RenderFillRect (renderer, &player2_healthbar);

    SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect (renderer, &player1_healthbar);
    SDL_RenderDrawRect (renderer, &player2_healthbar);
}

void GameScene::close ()
{
    for (auto & player : players)
    {
        player->close ();
        delete player;
    }
}
