#include "scenes/main_menu_scene.hpp"

MainMenuScene::MainMenuScene (SceneContainer * canvas) : Scene (canvas) {}

bool MainMenuScene::init (SDL_Renderer * renderer)
{
    bg          = IMG_LoadTexture (renderer, "./res/ui/main_menu/bg.png");
    training_fg = IMG_LoadTexture (renderer, "./res/ui/main_menu/training_fg.png");
    training_bg = IMG_LoadTexture (renderer, "./res/ui/main_menu/training_bg.png");
    network_fg  = IMG_LoadTexture (renderer, "./res/ui/main_menu/network_fg.png");
    network_bg  = IMG_LoadTexture (renderer, "./res/ui/main_menu/network_bg.png");

    training_rect = {410, 390, 394, 93};
    network_rect  = {410, 550, 394, 93};

    return true;
}

bool MainMenuScene::step_event (SDL_Event & e)
{
    switch (e.type)
    {
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                case SDLK_UP:
                    selected++;
                    selected %= 2;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    selected--;
                    selected %= 2;
                    break;

                case SDLK_j:
                    if (selected == 0)
                        static_cast<Canvas *>(canvas)->set_scene (new GameScene (canvas));
                    else if (selected == 1)
                        static_cast<Canvas *> (canvas)->set_scene (new OnlineGameScene (canvas));
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
                        if (selected == 0)
                            static_cast<Canvas *> (canvas)->set_scene (new GameScene (canvas));
                        else if (selected == 1)
                            static_cast<Canvas *> (canvas)->set_scene (new OnlineGameScene (canvas));
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

                    case 11:
                        selected++;
                        selected %= 2;
                        break;    // up
                    case 12:
                        selected--;
                        selected %= 2;
                        break;    // down
                }
            }
            break;
    }

    return true;
}

void MainMenuScene::step_scene () {}

void MainMenuScene::step_render (SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_RenderCopy (renderer, bg, NULL, NULL);

    if (selected == 0)
        SDL_RenderCopy (renderer, training_bg, NULL, &training_rect);
    else
        SDL_RenderCopy (renderer, network_bg, NULL, &network_rect);

    SDL_RenderCopy (renderer, training_fg, NULL, &training_rect);
    SDL_RenderCopy (renderer, network_fg, NULL, &network_rect);
}

void MainMenuScene::close ()
{
    SDL_DestroyTexture (bg);
    SDL_DestroyTexture (training_fg);
    SDL_DestroyTexture (training_bg);
    SDL_DestroyTexture (network_fg);
    SDL_DestroyTexture (network_bg);
}
