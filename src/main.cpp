#include "SDL_net.h"
#include "canvas.hpp"

#include <chrono>
#include <iostream>
#include <queue>
#include <thread>


int main (int argc, char * args[])
{
    std::cout << "launched\n";

    const int width  = 960;
    const int height = 540;

    Canvas canvas (width, height);

    canvas.init ();
    canvas.set_scene (new GameScene ());

    int   start      = SDL_GetTicks ();
    int   fps_start  = SDL_GetTicks ();
    int   fps        = 60;
    float frame_time = 1000.f / fps;

    int frame_counter = 0;

    while (true)
    {
        if (canvas.step ()) [[unlikely]] return 0;

        // Cap to 60 FPS
        int duration = SDL_GetTicks () - start;
        SDL_Delay ((0 > frame_time - duration) ? 0 : (frame_time - duration));
        start = SDL_GetTicks ();
        frame_counter++;

        // get average frame rate every second
        if (SDL_GetTicks () - fps_start > 1000) [[unlikely]]
            {
                std::cout << "fps: " << frame_counter << "\r";
                frame_counter = 0;
                fps_start     = SDL_GetTicks ();
            }
    }

    canvas.close ();

    return 0;
}