#include "scenes/game_scene.hpp"

GameScene::GameScene (int width, int height) : Scene (width, height) {}

bool GameScene::init (SDL_Renderer * renderer)
{
    // Initialization flag
    bool success = true;

    stage.load (renderer);

    players.push_back (new PlayerJohnDebug ());
    players.push_back (new PlayerJohnDebug ());

    players.at (0)->other_player = players.at (1);
    players.at (1)->other_player = players.at (0);

    players.at (0)->x = ((stage.right_wall - stage.left_wall) / 2) - 300;
    players.at (1)->x = ((stage.right_wall - stage.left_wall) / 2) + 300;

    for (auto & player : players)
    {
        player->ground         = stage.ground_y;
        player->y              = stage.ground_y;
        player->left_wall      = stage.left_wall;
        player->right_wall     = stage.right_wall;
        player->max_separation = stage.tex_height;

        player->load_states (renderer);
    }

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

    stage.update_viewport (players[0], players[1]);
}

void GameScene::step_render (SDL_Window * window, SDL_Renderer * renderer, int & width, int & height)
{
    // draw stage
    SDL_RenderCopy (renderer, stage.texture, &stage.src_rect, &stage.dst_rect);

    // std::cout << players[0]->x << "," << players[1]->x << "\n";

    // draw players
    for (auto & player : players)
    {
        // // Player
        SDL_SetRenderDrawColor (renderer, 0, 0, 180, 0xFF);
        SDL_RendererFlip flip = {};
        if (!player->is_left ()) flip = SDL_FLIP_HORIZONTAL;
        auto dest = stage.project_rect (player->dst_area);
        SDL_RenderCopyEx (renderer, player->texture, NULL, &dest, 0, NULL, flip);

        if (is_debug) [[unlikely]]
            {
                // // Debug lines
                // hit and hurtboxes
                dest = stage.project_rect (player->dst_area);
                SDL_RenderDrawRect (renderer, &dest);
                SDL_SetRenderDrawColor (renderer, 180, 180, 0, 0xFF);
                dest = stage.project_rect (player->collision);
                SDL_RenderDrawRect (renderer, &dest);

                // floor and walls
                SDL_SetRenderDrawColor (renderer, 40, 40, 0, 0xFF);
                SDL_RenderDrawLine (renderer, 0, stage.ground_y, width, stage.ground_y);
                SDL_RenderDrawLine (renderer, 50, 0, 50, height);
                SDL_RenderDrawLine (renderer, 1870, 0, 1870, height);

                // player position
                dest = stage.project_rect ({(int) player->x, (int) player->y, 0, 0});
                SDL_SetRenderDrawColor (renderer, 255, 255, 255, 0xFF);
                SDL_RenderDrawLine (renderer, dest.x + 10, dest.y, dest.x - 10, dest.y);
                SDL_RenderDrawLine (renderer, dest.x, dest.y + 10, dest.x, dest.y - 10);


                // player hurtboxes
                if (player->guard == Player::NONE)
                    SDL_SetRenderDrawColor (renderer, 0, 180, 0, 40);
                else
                    SDL_SetRenderDrawColor (renderer, 0, 180, 180, 40);

                for (auto & hurtbox : player->hurtboxes)
                {
                    dest = stage.project_rect (hurtbox);
                    SDL_RenderFillRect (renderer, &dest);
                }

                if (player->guard == Player::NONE)
                    SDL_SetRenderDrawColor (renderer, 0, 180, 0, 255);
                else
                    SDL_SetRenderDrawColor (renderer, 0, 180, 180, 255);

                for (auto & hurtbox : player->hurtboxes)
                {
                    dest = stage.project_rect (hurtbox);
                    SDL_RenderDrawRect (renderer, &dest);
                }

                // player hitboxes
                SDL_SetRenderDrawColor (renderer, 180, 0, 0, 40);
                for (auto & hitbox : player->hitboxes)
                {
                    dest = stage.project_rect (hitbox);
                    SDL_RenderFillRect (renderer, &dest);
                }
                SDL_SetRenderDrawColor (renderer, 180, 0, 0, 255);
                for (auto & hitbox : player->hitboxes)
                {
                    dest = stage.project_rect (hitbox);
                    SDL_RenderDrawRect (renderer, &dest);
                }
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

    stage.close ();
}

Stage::Stage (std::string texture_path, int left_wall, int right_wall, int ground_y)
    : texture_path (texture_path), left_wall (left_wall), right_wall (right_wall), ground_y (ground_y)
{
}

void Stage::load (SDL_Renderer * renderer)
{
    texture = IMG_LoadTexture (renderer, "res/stages/debug_city/frames/0000.png");

    SDL_QueryTexture (texture, NULL, NULL, &tex_width, &tex_height);
}

void Stage::close ()
{
    SDL_DestroyTexture (texture);
    texture = NULL;
}

SDL_Rect Stage::project_rect (SDL_Rect in)
{
    SDL_Rect ret = SDL_Rect {in.x - viewport.x, in.y, in.w, in.h};

    ret.x = ((ret.x + (in.w / 2.0)) * viewport_sf);
    ret.y = ((ret.y + (in.h) - ground_y) * viewport_sf);
    ret.w *= viewport_sf;
    ret.h *= viewport_sf;

    ret.x -= (ret.w / 2.0);
    ret.y -= ret.h - ground_y;

    return ret;
}

void Stage::update_viewport (Player * p1, Player * p2)
{
    int width  = std::max (1280, (int) std::fabs (p1->x - p2->x) + 400);
    int height = width * (9 / 16.0);

    viewport_sf = 1920.0 / width;

    int x = ((p1->x + p2->x) / 2) - (width / 2);
    if (x < 0) x = 0;
    if (x > tex_width - width) x = tex_width - width;
    int y = tex_height - height;

    /*std::cout << x << "," << y << "," << width << "," << height << "," << viewport_sf << ","
              << std::fabs ((y / (tex_height * 2.0))) << "\n";*/


    src_rect = SDL_Rect {x, std::max (0, y - (int) (180 - (180 / viewport_sf))), width, height};
    dst_rect = SDL_Rect {0, 0, 1920, 1080};

    if (y < 0)
    {
        dst_rect.y = (int) (1080 * std::fabs ((y / (tex_height * 2.0))));
        dst_rect.h = 1080 - (int) (1080 * std::fabs ((y / (tex_height * 2.0))));
    }

    if (x < 0)
    {
        dst_rect.x = (int) (1920 * std::fabs ((x / (tex_width * 2.0))));
        dst_rect.w = 1920 - (int) (1920 * std::fabs ((x / (tex_width * 2.0))));
    }

    if ((int) (190 - (190 / viewport_sf)) < 0) { dst_rect.y += (1080.0 / height) * (int) (190 - (190 / viewport_sf)); }

    viewport = SDL_Rect {x, y, width, height};
}