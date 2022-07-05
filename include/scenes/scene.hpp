#pragma once

#include "SDL.h"
#include "scene_container.hpp"

class Scene : public SceneContainer
{
  protected:
    SceneContainer * canvas;

  public:
    Scene (SceneContainer * canvas);

    virtual bool init (SDL_Renderer *)                      = 0;    // init canvas and memory
    virtual bool step_event (SDL_Event & e)                 = 0;    // handle frames
    virtual void step_scene ()                              = 0;    // update internal state
    virtual void step_render (SDL_Window *, SDL_Renderer *) = 0;    // render frame
    virtual void close ()                                   = 0;    // free all memory
};