#include "scenes/character_select_scene.hpp"

CharacterSelectScene::CharacterSelectScene (SceneContainer * canvas) : Scene (canvas) {}

bool CharacterSelectScene::init (SDL_Renderer * renderer)
{
    bg               = IMG_LoadTexture (renderer, "./res/ui/character_select/bg.png");
    john_debug       = IMG_LoadTexture (renderer, "./res/characters/john_debug/avatar.png");
    billy            = IMG_LoadTexture (renderer, "./res/characters/billy/avatar.png");
    josh             = IMG_LoadTexture (renderer, "./res/characters/josh/avatar.png");
    bonk             = IMG_LoadTexture (renderer, "./res/characters/bonk/avatar.png");
    selection_square = IMG_LoadTexture (renderer, "./res/ui/character_select/selection_square.png");

    selection_rect = {0, 0, 128, 128};

    return true;
}

bool CharacterSelectScene::step_event (SDL_Event & e)
{
    switch (e.type)
    {
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                case SDLK_d:
                case SDLK_RIGHT:
                    selected++;
                    selected %= num_selections;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    selected--;
                    selected %= num_selections;
                    break;

                case SDLK_j:
                    if (p1 == -1)
                        p1 = selected;
                    else if (p2 == -1)
                        p2 = selected;
                    else
                    {
                        Player * tp1 = NULL;
                        Player * tp2 = NULL;

                        switch (p1)
                        {
                            case 0: tp1 = new PlayerJohnDebug (); break;
                            case 1: tp1 = new PlayerBilly (); break;
                            case 2: tp1 = new PlayerJosh (); break;
                            case 3: tp1 = new PlayerBonk (); break;
                            default: tp1 = new PlayerJohnDebug (); break;
                        }

                        switch (p2)
                        {
                            case 0: tp2 = new PlayerJohnDebug (); break;
                            case 1: tp2 = new PlayerBilly (); break;
                            case 2: tp2 = new PlayerJosh (); break;
                            case 3: tp2 = new PlayerBonk (); break;
                            default: tp1 = new PlayerJohnDebug (); break;
                        }

                        static_cast<Canvas *> (canvas)->set_scene (new GameScene (canvas, tp1, tp2));
                    }
                    break;
            }
            break;
        case SDL_JOYBUTTONUP:
            if (e.jbutton.which == 0)
            {
                // std::cout << (int) e.jbutton.button << "\n";
                switch ((int) e.jbutton.button)
                {
                    case 0:
                        if (p1 == -1)
                            p1 = selected;
                        else if (p2 == -1)
                            p2 = selected;
                        else
                        {
                            Player * tp1 = NULL;
                            Player * tp2 = NULL;

                            switch (p1)
                            {
                                case 0: tp1 = new PlayerJohnDebug (); break;
                                case 1: tp1 = new PlayerBilly (); break;
                                case 2: tp1 = new PlayerJosh (); break;
                                case 3: tp1 = new PlayerBonk (); break;
                                default: tp1 = new PlayerJohnDebug (); break;
                            }

                            switch (p2)
                            {
                                case 0: tp2 = new PlayerJohnDebug (); break;
                                case 1: tp2 = new PlayerBilly (); break;
                                case 2: tp2 = new PlayerJosh (); break;
                                case 3: tp2 = new PlayerBonk (); break;
                                default: tp1 = new PlayerJohnDebug (); break;
                            }

                            static_cast<Canvas *> (canvas)->set_scene (new GameScene (canvas, tp1, tp2));
                        }
                        break;         // x
                    case 1:            // circle
                    case 2:            // square
                    case 3:            // triangle
                    case 4:            // select
                    case 5:            // logo
                    case 6:            // start
                    case 7:            // l stick
                    case 8:            // r stick
                    case 9:            // l bumper
                    case 10: break;    // r bumper

                    case 14:
                        selected++;
                        selected %= num_selections;
                        break;    // right
                    case 13:
                        selected--;
                        selected %= num_selections;
                        break;    // left
                }
            }
            break;
    }

    return true;
}

void CharacterSelectScene::step_scene () { selection_rect.x = 0 + (128 * selected); }

void CharacterSelectScene::step_render (SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_RenderCopy (renderer, bg, NULL, NULL);

    SDL_Rect temp = {0, 0, 128, 128};
    SDL_RenderCopy (renderer, john_debug, NULL, &temp);
    temp.x += 128;
    SDL_RenderCopy (renderer, billy, NULL, &temp);
    temp.x += 128;
    SDL_RenderCopy (renderer, josh, NULL, &temp);
    temp.x += 128;
    SDL_RenderCopy (renderer, bonk, NULL, &temp);

    temp = {0, 256, 512, 512};
    if (p1 == 0)
        SDL_RenderCopy (renderer, john_debug, NULL, &temp);
    else if (p1 == 1)
        SDL_RenderCopy (renderer, billy, NULL, &temp);
    else if (p1 == 2)
        SDL_RenderCopy (renderer, josh, NULL, &temp);
    else if (p1 == 3)
        SDL_RenderCopy (renderer, bonk, NULL, &temp);

    temp = {512, 256, 512, 512};
    if (p2 == 0)
        SDL_RenderCopy (renderer, john_debug, NULL, &temp);
    else if (p2 == 1)
        SDL_RenderCopy (renderer, billy, NULL, &temp);
    else if (p2 == 2)
        SDL_RenderCopy (renderer, josh, NULL, &temp);
    else if (p2 == 3)
        SDL_RenderCopy (renderer, bonk, NULL, &temp);


    SDL_RenderCopy (renderer, selection_square, NULL, &selection_rect);
}

void CharacterSelectScene::close ()
{
    SDL_DestroyTexture (bg);
    SDL_DestroyTexture (john_debug);
    SDL_DestroyTexture (billy);
    SDL_DestroyTexture (josh);
    SDL_DestroyTexture (bonk);
    SDL_DestroyTexture (selection_square);
}
