from PIL import Image
import os
import numpy as np

from hitbox_calculator import draw_hitbox


def main():
    chars = os.scandir("anim/")

    for entry in chars:
        # for each character
        if entry.is_dir():
            print("generating for", entry.name)

            frames = os.scandir("anim/" + entry.name)
            animations = {
                "neutral_idle": [],
                "crouch_idle": [],
                "air_idle": [],
                "neutral_to_crouch": [],
                "neutral_to_air": [],
                "crouch_to_neutral": [],
                "walk_forwards": [],
                "walk_backwards": [],
            }

            for frame in frames:
                if frame.is_file():
                    if frame.name[:-8] not in animations:
                        animations[frame.name[:-8]] = [frame.name]
                        print("\tfound animation", frame.name[:-8])
                    else:
                        animations[frame.name[:-8]].append(frame.name)

            for key, item in animations.items():
                if len(item) == 0:
                    if key == "crouch_to_neutral":
                        print("\tanimation", key,
                              "not found, reversing neutral_to_crouch")
                        animations["crouch_to_neutral"] = list(
                            reversed(animations["neutral_to_crouch"]))
                    else:
                        print("\tanimation", key, "not found")

            max_animation_length = max(
                [len(animations[x]) for x in animations])

            atlas = Image.new(
                'RGBA', (max_animation_length*500, len(animations)*600))

            key_count = 0
            standard_animations = ["neutral_idle", "crouch_idle", "air_idle",
                                   "walk_forwards", "walk_backwards",
                                   "neutral_to_crouch", "neutral_to_air",
                                   "crouch_to_neutral", 
                                   "a"]#, "b", "c", "d"]

            hurtboxes = []
            hitboxes = []

            for key in standard_animations:
                hurtboxes.append([])
                hitboxes.append([])
                for i in range(len(animations[key])):
                    out = draw_hitbox("./anim/" + entry.name +
                                      "/" + animations[key][i])

                    if out == "same as last":
                        if len(hurtboxes[-1]) > 0:
                            hurtboxes[-1].append(hurtboxes[-1][-1])
                            hitboxes[-1].append(hitboxes[-1][-1])
                        else:
                            hurtboxes[-1].append(hurtboxes[-2][-1])
                            hitboxes[-1].append(hitboxes[-2][-1])
                    else:
                        hurtboxes[-1].append(out[0])
                        hitboxes[-1].append(out[1])

                    atlas.paste(Image.open("./anim/" + entry.name +
                                "/" + animations[key][i]), (i * 500, key_count * 600))

                key_count += 1

            for key in animations:
                if key not in standard_animations:
                    print(key, "needs to be added to standard animations")

            #print(hurtboxes)

            atlas.save("atlas/" + entry.name + ".png")

            with open("config/debug/hiurtboxes.json", "w") as f:
                f.write('{"hurtboxes":')
                
                f.write(str(hurtboxes))

                f.write(',"hitboxes":')

                f.write(str(hitboxes))

                f.write('}')


if __name__ == "__main__":
    main()
