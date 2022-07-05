#include "scenes/online_game_scene.hpp"

OnlineGameScene::OnlineGameScene (SceneContainer * canvas, Player * p1, Player * p2) : GameScene (canvas, p1, p2) {}

bool OnlineGameScene::init (SDL_Renderer * renderer)
{
    // Initialization flag
    bool success = true;

    success = GameScene::init (renderer);

    // change to connecting screen
    connecting_screen = IMG_LoadTexture (renderer, "res/ui/game/connecting_screen.png");
    SDL_RenderCopy (renderer, connecting_screen, NULL, NULL);
    SDL_RenderPresent (renderer);

    // networking
    /* Open a socket on random port */
    if (!(sd = SDLNet_UDP_Open (0)))
    {
        fprintf (stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Resolve server name */
    if (SDLNet_ResolveHost (&srvadd, "127.0.0.1", 9600))
    {
        fprintf (stderr, "SDLNet_ResolveHost(%s %d): %s\n", "127.0.0.1", 9600, SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Allocate memory for the packet */
    if (!(p_out = SDLNet_AllocPacket (512)))
    {
        fprintf (stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Allocate memory for the packet */
    if (!(p_in = SDLNet_AllocPacket (512)))
    {
        fprintf (stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    p_out->data = (Uint8 *) "sync_me";

    p_out->address.host = srvadd.host; /* Set the destination host */
    p_out->address.port = srvadd.port; /* And destination port */

    p_out->len = strlen ((char *) p_out->data) + 1;
    SDLNet_UDP_Send (sd, -1, p_out); /* This sets the p->channel */

    printf ("sent sync_me\n");
    printf ("listening for server reply...  ");

    while (true)
    {
        if (SDLNet_UDP_Recv (sd, p_in) && p_in->address.host == srvadd.host && p_in->address.port == srvadd.port)
        {
            printf ("%s\n", (char *) p_in->data);

            if (strcmp ((char *) p_in->data, "player_1") == 0)
            {
                current_player = 0;
                break;
            }
            if (strcmp ((char *) p_in->data, "player_2") == 0)
            {
                current_player = 1;
                break;
            }
        }
    }

    return success;
}

bool OnlineGameScene::step_event (SDL_Event & e)
{
    // std::cout << e.type << "\n";

    switch (e.type)
    {
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                case SDLK_UP: players.at (current_player)->input (UP, PRESSED); break;
                case SDLK_s:
                case SDLK_DOWN: players.at (current_player)->input (DOWN, PRESSED); break;
                case SDLK_a:
                case SDLK_LEFT: players.at (current_player)->input (LEFT, PRESSED); break;
                case SDLK_d:
                case SDLK_RIGHT: players.at (current_player)->input (RIGHT, PRESSED); break;
                case SDLK_j: players.at (current_player)->input (A, PRESSED); break;
                case SDLK_k: players.at (current_player)->input (B, PRESSED); break;
                case SDLK_l: players.at (current_player)->input (C, PRESSED); break;
                case SDLK_SEMICOLON: players.at (current_player)->input (D, PRESSED); break;

                case SDLK_F3: is_debug = !is_debug; break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                case SDLK_UP: players.at (current_player)->input (UP, RELEASED); break;
                case SDLK_s:
                case SDLK_DOWN: players.at (current_player)->input (DOWN, RELEASED); break;
                case SDLK_a:
                case SDLK_LEFT: players.at (current_player)->input (LEFT, RELEASED); break;
                case SDLK_d:
                case SDLK_RIGHT: players.at (current_player)->input (RIGHT, RELEASED); break;
                case SDLK_j: players.at (current_player)->input (A, RELEASED); break;
                case SDLK_k: players.at (current_player)->input (B, RELEASED); break;
                case SDLK_l: players.at (current_player)->input (C, RELEASED); break;
                case SDLK_SEMICOLON: players.at (current_player)->input (D, RELEASED); break;
            }
            break;
        case SDL_JOYBUTTONDOWN:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0: players.at (current_player)->input (A, PRESSED); break;    // x
                    case 1: players.at (current_player)->input (B, PRESSED); break;    // circle
                    case 2: players.at (current_player)->input (C, PRESSED); break;    // square
                    case 3: players.at (current_player)->input (D, PRESSED); break;    // triangle
                    case 4:                                                            // select
                    case 5:                                                            // logo
                    case 6:                                                            // start
                    case 7:                                                            // l stick
                    case 8:                                                            // r stick
                    case 9:                                                            // l bumper
                    case 10: break;                                                    // r bumper

                    case 11: players.at (current_player)->input (UP, PRESSED); break;       // up
                    case 12: players.at (current_player)->input (DOWN, PRESSED); break;     // down
                    case 13: players.at (current_player)->input (LEFT, PRESSED); break;     // left
                    case 14: players.at (current_player)->input (RIGHT, PRESSED); break;    // right
                }
            }
            break;
        case SDL_JOYBUTTONUP:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0: players.at (current_player)->input (A, RELEASED); break;    // x
                    case 1: players.at (current_player)->input (B, RELEASED); break;    // circle
                    case 2: players.at (current_player)->input (C, RELEASED); break;    // square
                    case 3: players.at (current_player)->input (D, RELEASED); break;    // triangle
                    case 4:                                                             // select
                    case 5:                                                             // logo
                    case 6:                                                             // start
                    case 7:                                                             // l stick
                    case 8:                                                             // r stick
                    case 9:                                                             // l bumper
                    case 10: break;                                                     // r bumper

                    case 11: players.at (current_player)->input (UP, RELEASED); break;       // up
                    case 12: players.at (current_player)->input (DOWN, RELEASED); break;     // down
                    case 13: players.at (current_player)->input (LEFT, RELEASED); break;     // left
                    case 14: players.at (current_player)->input (RIGHT, RELEASED); break;    // right
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

void OnlineGameScene::step_scene ()
{
    GameScene::step_scene ();

    // network
    MatchPacket packet_data
        = {frame_time, players[current_player]->x, players[current_player]->y, "", players[current_player]->counter};

    strcpy (packet_data.state, players[current_player]->state->name.c_str ());

    // std::cout << players[current_player]->state->name << "            \r";

    p_out->address.host = srvadd.host;
    p_out->address.port = srvadd.port;
    p_out->len          = sizeof (packet_data) + 1;
    p_out->data         = (Uint8 *) &packet_data;


    SDLNet_UDP_Send (sd, -1, p_out); /* This sets the p->channel */

    // recieve
    while (SDLNet_UDP_Recv (sd, p_in))
    {
        std::cout << "current frame time difference = " << frame_time - ((MatchPacket *) p_in->data)->frame_time
                  << "    state = " << ((MatchPacket *) p_in->data)->state << "        \r";

        players[current_player == 0 ? 1 : 0]->x = ((MatchPacket *) p_in->data)->x;
        players[current_player == 0 ? 1 : 0]->y = ((MatchPacket *) p_in->data)->y;
        players[current_player == 0 ? 1 : 0]->change_state (((MatchPacket *) p_in->data)->state);
        players[current_player == 0 ? 1 : 0]->counter = ((MatchPacket *) p_in->data)->counter;
    }

    if (players[current_player]->state->state_type == State::MOVE_GRAB
        || players[current_player]->state->state_type == State::MOVE_COMMAND_GRAB)
        players[current_player == 0 ? 1 : 0]->change_state ("grabbed");
}

void OnlineGameScene::close ()
{
    GameScene::close ();

    SDLNet_FreePacket (p_in);
    SDLNet_FreePacket (p_out);


    SDL_DestroyTexture (connecting_screen);
    connecting_screen = NULL;
}
