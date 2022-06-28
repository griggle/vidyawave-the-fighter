from hitbox_calculator import draw_hitbox
from PIL import Image
import os
import numpy as np
import pathlib

os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"

common_animations = [
    "neutral",
    "crouch",
    "air",
    "walk_forward",
    "walk_backward",
    "sprint",
    "backdash",
    "neutral_to_crouch",
    "neutral_to_air",
    "neutral_to_forward_air",
    "neutral_to_backward_air",
    "crouch_to_neutral",
]


def generate_animation_atlas_and_hitbox(frames_dir, atlas_path, hitbox_path, hurtbox_path):
    frames = [frames_dir + "/" + x.name for x in os.scandir(frames_dir) if x.is_file()
              and pathlib.Path(x.name).suffix == ".png"]

    #print (pathlib.Path(frames_dir).parent)

    if len(frames) == 0:
        print(pathlib.Path(frames_dir).name,
              "not found, using missing texture")
        frames = ["missing_texture.png"]

    hitboxes = []
    hurtboxes = []

    with open(atlas_path, "w") as f:
        f.write("")

    for i, frame_path in enumerate(frames):
        frame = Image.open(frame_path)

        if frame.size != (500, 600):
            print("frame size needs to be 500x600,", frame.size, "was given.")
            return

        with open(atlas_path, "a") as f:
            f.write("res/characters/" + frame_path[2:] + "\n")

        hiurtboxes = [[], []]

        if hiurtboxes == "same as last":
            if len(hitboxes) > 0:
                hitboxes.append(hitboxes[-1].copy())
                hurtboxes.append(hurtboxes[-1].copy())
            else:
                hitboxes.append([])
                hurtboxes.append([])
        else:
            hitboxes.append(hiurtboxes[1])
            hurtboxes.append(hiurtboxes[0])

    with open(hitbox_path, "w", encoding="utf8") as f:
        f.write(str(hitboxes))

    with open(hurtbox_path, "w", encoding="utf8") as f:
        f.write(str(hurtboxes))


def main():
    print("Generate for which character:")

    characters =\
        [x for x in os.scandir("./") if x.is_dir()
         and not x.name == "__pycache__"]

    for i, character in enumerate(characters):
        print("\t" + str(i+1) + ":", character.name)

    inp = input("-> ")
    character = characters[int(inp)-1].name

    print("Generate for which animation:")

    for animation in common_animations:
        if(not os.path.isdir("./" + character + "/animations/frames/" + animation)):
            os.mkdir("./" + character + "/animations/frames/" + animation)

    animations =\
        [x for x in os.scandir(
            "./" + character + "/animations/frames/") if x.is_dir()]

    print("\t" + str(0) + ": all")

    for i, animation in enumerate(animations):
        print("\t" + str(i+1) + ":", animation.name)

    inp = input("-> ")

    if int(inp) == 0:
        for i in range(len(animations)):
            animation = animations[int(i)].name

            generate_animation_atlas_and_hitbox(
                "./" + character + "/animations/frames/" + animation,
                "./" + character + "/animations/" + animation + ".atlas",
                "./" + character + "/animations/" + animation + ".hitboxes",
                "./" + character + "/animations/" + animation + ".hurtboxes"
            )

    else:
        animation = animations[int(inp)-1].name

        generate_animation_atlas_and_hitbox(
            "./" + character + "/animations/frames/" + animation,
            "./" + character + "/animations/" + animation + ".atlas",
            "./" + character + "/animations/" + animation + ".hitboxes",
            "./" + character + "/animations/" + animation + ".hurtboxes"
        )


if __name__ == "__main__":
    main()
