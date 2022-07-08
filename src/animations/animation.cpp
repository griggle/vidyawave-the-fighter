#include "animations/animation.hpp"

Animation::Animation (std::vector<std::string> atlas, int fps) : atlas (atlas), fps (fps) {}

void Animation::load_texture (SDL_Renderer * renderer)
{
    for (auto & frame_path : atlas) { textures.push_back (IMG_LoadTexture (renderer, frame_path.c_str ())); }
}

void Animation::close_texture ()
{
    for (auto & tex : textures)
    {
        SDL_DestroyTexture (tex);
        tex = NULL;
    }
}

SDL_Texture * Animation::get_frame (int frame)
{
    return textures.at (((int) (frame / (60.0 / fps))) % textures.size ());
}
