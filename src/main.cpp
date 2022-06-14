#include "canvas.hpp"

#include <iostream>

int main (int argc, char * args[])
{
    std::cout << "launched\n";

    const int width  = 1920;
    const int height = 1080;

    Canvas canvas (width, height);

    canvas.init ();

    canvas.set_scene (new GameScene(width, height));

    while (!canvas.step ()) {}
    
    canvas.close ();

    return 0;
}