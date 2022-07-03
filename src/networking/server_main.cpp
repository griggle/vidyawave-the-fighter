#include "SDL.h"
#include "SDL_net.h"

#include <iostream>


int main (int argc, char * args[])
{
    std::cout << "server\n";

    UDPsocket   sd;    /* Socket descriptor */
    UDPpacket * p;     /* Pointer to packet memory */
    UDPpacket * p_out; /* Pointer to packet memory */
    IPaddress   player_1 = {0, 0};
    IPaddress   player_2 = {0, 0};
    int         quit;

    /* Initialize SDL_net */
    if (SDLNet_Init () < 0)
    {
        fprintf (stderr, "SDLNet_Init: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Open a socket */
    if (!(sd = SDLNet_UDP_Open (9600)))
    {
        fprintf (stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Make space for the packet */
    if (!(p = SDLNet_AllocPacket (512)))
    {
        fprintf (stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Make space for the packet */
    if (!(p_out = SDLNet_AllocPacket (512)))
    {
        fprintf (stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError ());
        exit (EXIT_FAILURE);
    }

    /* Main loop */ quit = 0;
    while (!quit)
    {
        /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
        if (SDLNet_UDP_Recv (sd, p))
        {
            //printf ("UDP Packet incoming\n");
            //printf ("\tChan: %d\n", p->channel);
            //printf ("\tData: %s\n", (char *) p->data);
            //printf ("\tLen: %d\n", p->len);
            //printf ("\tMaxlen: %d\n", p->maxlen);
            //printf ("\tStatus: %d\n", p->status);
            //printf ("\tAddress: %d.%d.%d.%d %d\n", p->address.host & 0xff, (p->address.host & 0xff00) >> 8,
            //        (p->address.host & 0xff0000) >> 16, (p->address.host & 0xff000000) >> 24, p->address.port);

            // SDLNet_UDP_Send (sd, -1, p);

            /* Quit if packet contains "quit" */
            if (strcmp (((char *) p->data), "sync_me") == 0)
            {
                printf ("got a sync request... ");

                if (player_1.host == 0)
                {
                    player_1 = p->address;
                    printf ("it was player 1 (%d.%d.%d.%d:%d)\n", player_1.host & 0xff, (player_1.host & 0xff00) >> 8,
                            (player_1.host & 0xff0000) >> 16, (player_1.host & 0xff000000) >> 24, player_1.port);
                }
                else if (player_2.host == 0)
                {
                    player_2 = p->address;
                    printf ("it was player 2 (%d.%d.%d.%d:%d)\n", player_2.host & 0xff, (player_2.host & 0xff00) >> 8,
                            (player_2.host & 0xff0000) >> 16, (player_2.host & 0xff000000) >> 24, player_2.port);

                    p_out->data    = (Uint8 *) "player_1";
                    p_out->len     = strlen ((char *) p_out->data) + 1;
                    p_out->address = player_1;
                    SDLNet_UDP_Send (sd, -1, p_out);
                    p_out->data    = (Uint8 *) "player_2";
                    p_out->len     = strlen ((char *) p_out->data) + 1;
                    p_out->address = player_2;
                    SDLNet_UDP_Send (sd, -1, p_out);
                }
            }
            else
            {
                //printf ("Forwarding... \n");

                if (p->address.host == player_1.host && p->address.port == player_1.port)
                {
                    p_out->address = player_2;
                    p_out->data    = p->data;
                    p_out->len     = p->len;
                    SDLNet_UDP_Send (sd, -1, p_out);
                }
                else if (p->address.host == player_2.host && p->address.port == player_2.port)
                {
                    p_out->address = player_1;
                    p_out->data    = p->data;
                    p_out->len     = p->len;
                    SDLNet_UDP_Send (sd, -1, p_out);
                }
            }
        }
    }

    /* Clean and exit */
    SDLNet_FreePacket (p);
    SDLNet_FreePacket (p_out);
    SDLNet_Quit ();
}