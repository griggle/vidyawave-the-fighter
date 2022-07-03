#include <canvas.hpp>

Canvas::Canvas (int width, int height) : width (width), height (height), quit (false), current_scene (nullptr) {}

bool Canvas::step ()
{
    // Handle events on queue
    while (SDL_PollEvent (&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_QUIT) { quit = true; }
        // Window resizing
        else if (e.type == SDL_WINDOWEVENT)
        {
            switch (e.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    width  = e.window.data1;
                    height = e.window.data2;
                    break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    width  = e.window.data1;
                    height = e.window.data2;
                    break;
            }
        }
        else
        {
            current_scene->step_event (e);
        }
    }

    // Update scene
    current_scene->step_scene ();

    // Clear screen
    SDL_SetRenderDrawColor (gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderClear (gRenderer);

    // Draw pixels
    current_scene->step_render (gWindow, gRenderer);

    // Update screen
    SDL_RenderPresent (gRenderer);

    return quit;
}

void Canvas::close ()
{
    // Destroy window
    SDL_DestroyRenderer (gRenderer);
    SDL_DestroyWindow (gWindow);
    gWindow   = NULL;
    gRenderer = NULL;

    // Close game controller
    SDL_JoystickClose (gGameController);
    gGameController = NULL;

    // Free the music
    Mix_FreeMusic (gMusic);
    gMusic = NULL;

    // Quit SDL subsystems
    SDL_Quit ();
    IMG_Quit ();
    Mix_Quit ();
    SDLNet_Quit ();
}

void Canvas::screen_shot ()
{
    SDL_Surface * sshot = SDL_CreateRGBSurface (0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels (gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP (sshot, "screenshot.bmp");
    SDL_FreeSurface (sshot);
}


bool Canvas::init ()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0)
    {
        printf ("SDL could not initialize! SDL Error: %s\n", SDL_GetError ());
        success = false;
    }
    else
    {
        // Check for joysticks
        if (SDL_NumJoysticks () < 1) { printf ("Warning: No joysticks connected!\n"); }
        else
        {
            // Load joystick
            gGameController = SDL_JoystickOpen (0);
            if (gGameController == NULL)
            { printf ("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError ()); }
        }

        // Initialize SDL_mixer
        if (Mix_Init (MIX_INIT_OGG) < 0)
        {
            printf ("SDL Mixer could not initialize! SDL Error: %s\n", Mix_GetError ());
            success = false;
        }

        if (Mix_OpenAudio (MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
        {
            printf ("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError ());
            success = false;
        }

        /* Initialize SDL_net */
        if (SDLNet_Init () < 0)
        {
            fprintf (stderr, "SDLNet_Init: %s\n", SDLNet_GetError ());
            exit (EXIT_FAILURE);
        }

        // Create window
        gWindow = SDL_CreateWindow ("Vidyawave, the Fighter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                                    height, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf ("Window could not be created! SDL Error: %s\n", SDL_GetError ());
            success = false;
        }
        else
        {
            // Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init (imgFlags) & imgFlags))
            {
                printf ("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError ());
                success = false;
            }
            else
            {
                // Create renderer for window
                gRenderer = SDL_CreateRenderer (gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL)
                {
                    printf ("Renderer could not be created! SDL Error: %s\n", SDL_GetError ());
                    success = false;
                }
                else
                {
                    // Initialize renderer color
                    SDL_SetRenderDrawColor (gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                }
            }
        }
    }

    // Load music
    gMusic = Mix_LoadMUS ("res/songs/test.mp3");
    if (gMusic == NULL)
    {
        printf ("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError ());
        success = false;
    }
    //Mix_PlayMusic (gMusic, -1);


    SDL_RenderSetScale (gRenderer, width / 1920.0, height / 1080.0);
    SDL_SetRenderDrawBlendMode (gRenderer, SDL_BLENDMODE_BLEND);

    width  = 1920;
    height = 1080;

    int num_joysticks = SDL_NumJoysticks ();
    int i;
    for (i = 0; i < num_joysticks; ++i)
    {
        SDL_Joystick * js = SDL_JoystickOpen (i);
        if (js)
        {
            SDL_JoystickGUID guid = SDL_JoystickGetGUID (js);
            char             guid_str[1024];
            SDL_JoystickGetGUIDString (guid, guid_str, sizeof (guid_str));
            const char * name = SDL_JoystickName (js);

            int num_axes    = SDL_JoystickNumAxes (js);
            int num_buttons = SDL_JoystickNumButtons (js);
            int num_hats    = SDL_JoystickNumHats (js);
            int num_balls   = SDL_JoystickNumBalls (js);

            printf ("%s \"%s\" axes:%d buttons:%d hats:%d balls:%d\n", guid_str, name, num_axes, num_buttons, num_hats,
                    num_balls);

            SDL_JoystickClose (js);
        }
    }

    return success;
}


bool Canvas::set_scene (Scene * new_scene)
{
    if (current_scene != nullptr)
    {
        current_scene->close ();
        delete current_scene;
    }
    current_scene = new_scene;
    return current_scene->init (gRenderer);
}