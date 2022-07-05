#pragma once

#include "SDL.h"
#include "canvas.hpp"
#include "players/billy.hpp"
#include "players/bonk.hpp"
#include "players/john_debug.hpp"
#include "players/josh.hpp"
#include "scene.hpp"
#include "tools/sdl_image_tools.hpp"

class CharacterSelectScene : public Scene
{
  protected:
    SDL_Texture * bg               = NULL;
    SDL_Texture * selection_square = NULL;

    SDL_Texture * john_debug = NULL;
    SDL_Texture * billy      = NULL;
    SDL_Texture * josh       = NULL;
    SDL_Texture * bonk       = NULL;

    SDL_Rect selection_rect;

    unsigned int selected       = 0;
    unsigned int num_selections = 4;

    int p1 = -1;
    int p2 = -1;

  public:
    CharacterSelectScene (SceneContainer * canvas);

    bool init (SDL_Renderer *);                         // init canvas and memory
    bool step_event (SDL_Event & e);                    // handle frames
    void step_scene ();                                 // update internal state
    void step_render (SDL_Window *, SDL_Renderer *);    // render frame
    void close ();                                      // free all memory
};