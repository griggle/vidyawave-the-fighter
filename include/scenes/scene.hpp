#pragma once

#include <SDL.h>

class Scene
{
  protected:
    int  width, height;    // dimensions
    bool quit;             // Main loop flag

    // debug stuff
    int joystick_x = 0;
    int joystick_y = 0;

  public:
    Scene (int width, int height);

    virtual bool init (SDL_Renderer *)      = 0;    // init canvas and memory
    virtual bool step_event (SDL_Event & e) = 0;    // handle frames
    virtual void step_scene ()              = 0;    // update internal state
    virtual void step_render (SDL_Window *, SDL_Renderer *, int & width,
                              int & height) = 0;    // render frame
    virtual void close ()                   = 0;    // free all memory
};