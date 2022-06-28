#include "animations/h_animation.hpp"

HitAnimation::HitAnimation (std::string atlas_path, std::string hurtbox_path, std::string hitbox_path, int fps)
    : Animation (atlas_path, fps), hurtbox_path (hurtbox_path.c_str ()), hitbox_path (hitbox_path.c_str ())
{
    hitboxes  = load_hiurtboxes_from_file (hitbox_path);
    hurtboxes = load_hiurtboxes_from_file (hurtbox_path);
}

std::vector<SDL_Rect> HitAnimation::get_hitboxes (int frame)
{
    if (hitboxes.size () > 0)
        return hitboxes.at ((((int) (frame / (60.0 / fps))) % hitboxes.size ()));
    else
        return std::vector<SDL_Rect> {};
}

std::vector<SDL_Rect> HitAnimation::get_hurtboxes (int frame)
{
    if (hurtboxes.size () > 0)
        return hurtboxes.at ((((int) (frame / (60.0 / fps))) % hurtboxes.size ()));
    else
        return std::vector<SDL_Rect> {};
}

std::vector<std::vector<SDL_Rect>> HitAnimation::load_hiurtboxes_from_file (std::string path)
{
    std::ifstream hiurtbox_stream (path);
    std::string   hiurtbox_string;

    if (hiurtbox_stream.is_open ())
    {
        while (hiurtbox_stream) { std::getline (hiurtbox_stream, hiurtbox_string); }
    }
    else
    {
        std::cout << "Couldn't open file: " << hurtbox_path << "\n";
    }

    hiurtbox_stream.close ();

    auto nest_parse = [] (std::string input) {
        int                      counter = 0;
        std::string              buffer;
        std::vector<std::string> ret;

        for (auto & c : input)
        {
            if (c == ']')
            {
                counter--;
                if (counter == 0)
                {
                    ret.push_back (buffer);
                    buffer = "";
                }
            }

            if (counter > 0) buffer += c;

            if (c == '[')
            {
                counter++;
            }
        }

        return ret;
    };

    auto str_to_rect = [] (std::string str) {
        SDL_Rect ret = {0, 0, 0, 0};

        std::string buffer  = "";
        int         counter = 0;

        for (int i = 0; i < str.size (); i++)
        {
            if (str[i] == ',')
            {
                switch (counter)
                {
                    case 0: ret.x = std::stoi (buffer); break;
                    case 1: ret.y = std::stoi (buffer); break;
                    case 2: ret.w = std::stoi (buffer); break;
                }

                buffer = "";
                counter++;
            }
            else if (str[i] != ' ')
                buffer += str[i];
        }

        ret.h = std::stoi (buffer);

        return ret;
    };

    std::vector<std::vector<std::string>> string_hiurt_split;
    std::vector<std::vector<SDL_Rect>>    ret;

    //auto temp = nest_parse(nest_parse(hiurtbox_string).at(0));

    for (auto & frame : nest_parse (nest_parse(hiurtbox_string).at(0)))
    {
        //for (auto & asdf : frame) std::cout << asdf << "\n\n\n";
        string_hiurt_split.push_back (nest_parse (frame));
    }

    for (int i = 0; i < string_hiurt_split.size (); i++)
    {
        ret.push_back ({});
        for (auto & box : string_hiurt_split[i])
        {
            if (box != "") ret.at (i).push_back (str_to_rect (box));
        }
    }

    return ret;
}
