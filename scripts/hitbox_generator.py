#from hitbox_calculator import draw_hitbox
from PIL import Image
import os
import numpy as np
import pathlib
import ast

#os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"

common_animations = [
    "neutral",
    "crouch",
    "air",

    "neutral_to_crouch",
    "crouch_to_neutral",

    "air_to_neutral",
    "neutral_to_air",
    "neutral_to_forward_air",
    "neutral_to_backward_air",

    "block_neutral",
    "block_crouch",
    "block_air",

    "stun",
    "knocked_down",
    "grabbed",
]


def generate_for_animation(file_name, character_name, animation_name):
    generated_animation_string = "{" + '"' + animation_name + '"' + ",\n                    { "

    read_boxes = []

    if os.path.exists("./res/characters/" + character_name + "/animations/frames/" + animation_name + "/" + file_name):
        with open("./res/characters/" + character_name + "/animations/frames/" + animation_name + "/" + file_name, "r") as f:
            read_boxes = ast.literal_eval(f.readline())
    else:
        with os.scandir("res/characters/" + character_name + "/animations/frames/" + animation_name) as frames:
            for entry in frames:
                if(entry.is_file() and entry.name.endswith(".png")):
                    read_boxes.append([])

    for frame in read_boxes:
        if len(frame) > 0:
            generated_animation_string += "\n                        {"
            for box in frame:
                generated_animation_string += "SDL_Rect{"
                generated_animation_string += str(int(box[0])) + ", "
                generated_animation_string += str(int(box[1])) + ", "
                generated_animation_string += str(int(box[2])) + ", "
                generated_animation_string += str(int(box[3]))
                generated_animation_string += "}, "

            generated_animation_string = generated_animation_string[:-2]
            generated_animation_string += "},"
        else:
            generated_animation_string += "\n                        {" + "},"

    return generated_animation_string[:-1] + "\n                    }\n                }"

def generate_for_character(file_name, character_name):
    generated_character_string = "{"

    with os.scandir("./res/characters/" + character_name + "/animations/frames/") as dirs:
        for entry in dirs:
            if(entry.is_dir()):
                generated_character_string += "\n                " + generate_for_animation(file_name, character_name, entry.name) + ","

    return "{" + '"' + character_name + '"' + ", " + generated_character_string[:-1] + "}\n            }"

def generate_file(file_name):
    generated_file = """#include "players/states/state.hpp"

    namespace GeneratedStates
    {
        // indexed by [player_name][state_name]
        std::map<std::string, std::map<std::string, std::vector<std::vector<SDL_Rect>>>> """ + file_name + "_map = {"

    with os.scandir("./res/characters/") as dirs:
        for entry in dirs:
            if(entry.is_dir()):
                generated_file += "\n            " + generate_for_character(file_name, entry.name) + ","

    generated_file = generated_file[:-1]
    generated_file += "\n        };\n    }"

    #print(generated_file)

    if not os.path.exists('generated'):
        os.makedirs('generated')

    with open("generated/generated_state_" + file_name + "es.cpp", "w") as f:
        f.write(generated_file)

def main():
    #print("Generating character hitboxes...")
    generate_file("hitbox")
    #print("Generating character hurtboxes...")
    generate_file("hurtbox")
   

if __name__ == "__main__":
    main()
