from PIL import Image
import os
import numpy as np
import pathlib
import pygame

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


def draw_hitbox(image_path, frame_number=0):
    img = Image.open(image_path)

    pygame.init()
    pygame.font.init()

    msfont = pygame.font.SysFont('Comic Sans MS', 30)

    text_surface = msfont.render("frame: " + str(frame_number), False, (255, 255, 255))

    size = img.width, img.height
    black = 40, 40, 40

    screen = pygame.display.set_mode(size)
    texture = pygame.image.load(image_path)
    info = pygame.image.load("./scripts/info.png")

    hitboxes = [[], []]
    hitbox_rect = [0, 0, 0, 0]

    is_drawing = False

    clock = pygame.time.Clock()

    hiurt = 0

    #return hitboxes

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                for hiurtbox in hitboxes:
                    for hitbox in hiurtbox:
                        hitbox[0] = (hitbox[0] - (img.width/2))
                        hitbox[1] = -(img.height - hitbox[1])
                        hitbox[2] = (hitbox[2])
                        hitbox[3] = hitbox[3]
                return hitboxes

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_d:
                    if hiurt == 0:
                        hiurt = 1
                        hitbox_rect = [0, 0, 0, 0]
                if event.key == pygame.K_s:
                    if frame_number != 0:
                        return "same as last"
                if event.key == pygame.K_f:
                    for hiurtbox in hitboxes:
                        for hitbox in hiurtbox:
                            hitbox[0] = (hitbox[0] - (img.width/2))
                            hitbox[1] = -(img.height - hitbox[1])
                            hitbox[2] = (hitbox[2])
                            hitbox[3] = hitbox[3]
                    return hitboxes

            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:  # 1 == left button
                    mouse_pos = pygame.mouse.get_pos()
                    hitbox_rect[0] = mouse_pos[0]
                    hitbox_rect[1] = mouse_pos[1]
                    is_drawing = True

            if event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:  # 1 == left button
                    is_drawing = False
                    hitboxes[hiurt].append(hitbox_rect.copy())
                    # print(
                    #     "Huitbox {" + str((hitbox_rect[0] - (img.width/2))/img.width) + ", -" + str((img.height - hitbox_rect[1])/img.height) + ",  " + str((hitbox_rect[2])/img.width) + ",  " + str(hitbox_rect[3]/img.height) + "},")

        if is_drawing:
            mouse_pos = pygame.mouse.get_pos()
            hitbox_rect[2] = mouse_pos[0] - hitbox_rect[0]
            hitbox_rect[3] = mouse_pos[1] - hitbox_rect[1]

        screen.fill(black)
        screen.blit(texture, texture.get_rect())
        screen.blit(info, info.get_rect())
        screen.blit(text_surface, (300,0))
        

        for h in hitboxes[0]:
            pygame.draw.rect(screen, (0, 120, 0), h, 2)
        for h in hitboxes[1]:
            pygame.draw.rect(screen, (120, 0, 0), h, 2)

        pygame.draw.rect(screen,  (0, 255, 0) if hiurt ==
                         0 else (255, 0, 0), hitbox_rect, 2)
        pygame.display.flip()
        clock.tick(30)


def generate_animation_atlas_and_hitbox(frames_dir, hitbox_path, hurtbox_path):
    frames = [frames_dir + "/" + x.name for x in os.scandir(frames_dir) if x.is_file()
              and pathlib.Path(x.name).suffix == ".png"]

    #print (pathlib.Path(frames_dir).parent)

    if len(frames) == 0:
        print(pathlib.Path(frames_dir).name,
              "not found, using missing texture")
        frames = ["res/missing_texture.png"]

    hitboxes = []
    hurtboxes = []

    for i, frame_path in enumerate(frames):
        frame = Image.open(frame_path)

        if frame.size != (500, 600):
            print("frame size needs to be 500x600,", frame.size, "was given.")
            return

        hiurtboxes = draw_hitbox(frame_path, i)

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
        [x for x in os.scandir("./res/characters/") if x.is_dir()
         and not x.name == "__pycache__"]

    for i, character in enumerate(characters):
        print("\t" + str(i+1) + ":", character.name)

    inp = input("-> ")
    character = characters[int(inp)-1].name

    print("Generate for which animation:")

    for animation in common_animations:
        if(not os.path.isdir("./res/characters/" + character + "/animations/frames/" + animation)):
            os.mkdir("./res/characters/" + character + "/animations/frames/" + animation)

    animations =\
        [x for x in os.scandir(
            "./res/characters/" + character + "/animations/frames/") if x.is_dir()]

    print("\t" + str(0) + ": all")

    for i, animation in enumerate(animations):
        print("\t" + str(i+1) + ":", animation.name)

    inp = input("-> ")

    if int(inp) == 0:
        for i in range(len(animations)):
            animation = animations[int(i)].name

            generate_animation_atlas_and_hitbox(
                "./res/characters/" + character + "/animations/frames/" + animation,
                "./res/characters/" + character + "/animations/frames/" + animation + "/" + "hitbox",
                "./res/characters/" + character + "/animations/frames/" + animation + "/" + "hurtbox"
            )

    else:
        animation = animations[int(inp)-1].name

        generate_animation_atlas_and_hitbox(
            "./res/characters/" + character + "/animations/frames/" + animation,
            "./res/characters/" + character + "/animations/frames/" + animation + "/" + "hitbox",
            "./res/characters/" + character + "/animations/frames/" + animation + "/" + "hurtbox"
        )


if __name__ == "__main__":
    main()
