#include "SDL_net.h"
#include "canvas.hpp"

#include <chrono>
#include <iostream>
#include <queue>
#include <thread>

int main (int argc, char * args[])
{
    std::cout << "launched\n";

    // std::string mode = "";

    // std::cin >> mode;

    const int width  = 1920;
    const int height = 1080;

    Canvas canvas (width, height);

    canvas.init ();
    canvas.set_scene (new GameScene (1920, 1080));

    /*auto frame_time = std::chrono::microseconds ((int) (1000000.0f / 90));
    auto start      = std::chrono::system_clock::now ();*/

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


        // std::cout << 1000.f / (frame_time - duration) << "\n";


        // Cap to 60 FPS
        /*auto duration = (std::chrono::system_clock::now () - start);
        std::this_thread::sleep_for (frame_time - duration);
        start = std::chrono::system_clock::now ();
        std::cout << std::chrono::seconds (1) / (frame_time - duration) << "\n";*/
    }

    canvas.close ();



    //// client
    // if (mode == "0")
    //{
    //    std::cout << "client\n";

    //    UDPsocket   sd;
    //    IPaddress   srvadd;
    //    UDPpacket * p;
    //    int         quit;


    //    /* Initialize SDL_net */
    //    if (SDLNet_Init () < 0)
    //    {
    //        fprintf (stderr, "SDLNet_Init: %s\n", SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Open a socket on random port */
    //    if (!(sd = SDLNet_UDP_Open (0)))
    //    {
    //        fprintf (stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Resolve server name */
    //    if (SDLNet_ResolveHost (&srvadd, "127.0.0.1", 2000))
    //    {
    //        fprintf (stderr, "SDLNet_ResolveHost(%s %d): %s\n", "127.0.0.1", 2000, SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Allocate memory for the packet */
    //    if (!(p = SDLNet_AllocPacket (512)))
    //    {
    //        fprintf (stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Main loop */
    //    quit = 0;
    //    while (!quit)
    //    {
    //        printf ("Fill the buffer\n>");
    //        scanf ("%s", (char *) p->data);

    //        p->address.host = srvadd.host; /* Set the destination host */
    //        p->address.port = srvadd.port; /* And destination port */

    //        p->len = strlen ((char *) p->data) + 1;
    //        SDLNet_UDP_Send (sd, -1, p); /* This sets the p->channel */

    //        /* Quit if packet contains "quit" */
    //        if (!strcmp ((char *) p->data, "quit")) quit = 1;
    //    }

    //    SDLNet_FreePacket (p);
    //    SDLNet_Quit ();
    //}
    //// server
    // else
    //{
    //    std::cout << "server\n";

    //    UDPsocket   sd; /* Socket descriptor */
    //    UDPpacket * p;  /* Pointer to packet memory */
    //    int         quit;

    //    /* Initialize SDL_net */
    //    if (SDLNet_Init () < 0)
    //    {
    //        fprintf (stderr, "SDLNet_Init: %s\n", SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Open a socket */
    //    if (!(sd = SDLNet_UDP_Open (2000)))
    //    {
    //        fprintf (stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Make space for the packet */
    //    if (!(p = SDLNet_AllocPacket (512)))
    //    {
    //        fprintf (stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError ());
    //        exit (EXIT_FAILURE);
    //    }

    //    /* Main loop */ quit = 0;
    //    while (!quit)
    //    {
    //        /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
    //        if (SDLNet_UDP_Recv (sd, p))
    //        {
    //            printf ("UDP Packet incoming\n");
    //            printf ("\tChan: %d\n", p->channel);
    //            printf ("\tData: %s\n", (char *) p->data);
    //            printf ("\tLen: %d\n", p->len);
    //            printf ("\tMaxlen: %d\n", p->maxlen);
    //            printf ("\tStatus: %d\n", p->status);
    //            printf ("\tAddress: %x %x\n", p->address.host, p->address.port);

    //            /* Quit if packet contains "quit" */
    //            if (!strcmp ((char *) p->data, "quit")) quit = 1;
    //        }
    //    }

    //    /* Clean and exit */ SDLNet_FreePacket (p);
    //    SDLNet_Quit ();
    //}



    return 0;
}