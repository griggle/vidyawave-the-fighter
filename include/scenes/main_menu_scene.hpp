#pragma once

#include "scene.hpp"
#include "character_select_scene.hpp"
#include "canvas.hpp"
#include "SDL.h"
#include "tools/sdl_image_tools.hpp"

class MainMenuScene : public Scene
{
  protected:
    SDL_Texture * bg = NULL;

    unsigned int selected = 0;

    SDL_Texture * training_fg = NULL;
    SDL_Texture * training_bg = NULL;
    SDL_Rect      training_rect;

    SDL_Texture * network_fg = NULL;
    SDL_Texture * network_bg = NULL;
    SDL_Rect      network_rect;

  public:
    MainMenuScene (SceneContainer * canvas);

    bool init (SDL_Renderer *);                         // init canvas and memory
    bool step_event (SDL_Event & e);                    // handle frames
    void step_scene ();                                 // update internal state
    void step_render (SDL_Window *, SDL_Renderer *);    // render frame
    void close ();                                      // free all memory
};