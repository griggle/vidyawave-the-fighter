#include "animations/animation.hpp"

Animation::Animation (std::string atlas_path, int fps) : atlas_path (atlas_path), fps (fps) {}

void Animation::load_texture (SDL_Renderer * renderer)
{
    std::ifstream atlas_stream (atlas_path);

    if (atlas_stream.is_open ())
    {
        while (atlas_stream)
        {
            std::string temp;
            std::getline (atlas_stream, temp);

            if (temp.size () > 5) textures.push_back (IMG_LoadTexture (renderer, temp.c_str ()));
        }
    }
    else
    {
        std::cout << "Couldn't open file: " << atlas_path << "\n";
    }

    atlas_stream.close ();


    frames = textures.size ();
}

void Animation::close_texture ()
{
    for (auto & tex : textures)
    {
        SDL_DestroyTexture (tex);
        tex = NULL;
    }
}

SDL_Texture * Animation::get_frame (int frame) { return textures.at (((int) (frame / (60.0 / fps))) % frames); }
