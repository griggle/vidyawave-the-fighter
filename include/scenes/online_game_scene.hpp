#pragma once

#include "game_scene.hpp"

class OnlineGameScene : public GameScene
{
  protected:
    UDPsocket   sd;
    IPaddress   srvadd;
    UDPpacket * p_out;
    UDPpacket * p_in;

    SDL_Texture * connecting_screen = NULL;

  public:
    OnlineGameScene (SceneContainer * canvas, Player * p1, Player * p2);

    bool init (SDL_Renderer * renderer);
    bool step_event (SDL_Event & e);    // run a frame
    void step_scene ();                 // update internal state
    void close ();
};