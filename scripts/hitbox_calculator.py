import sys
import pygame
from PIL import Image


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
    info = pygame.image.load("info.png")

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


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("No input file.")
        print(draw_hitbox("../res/missing_texture.png"))

    else:
        draw_hitbox(sys.argv[1])
