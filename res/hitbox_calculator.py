import sys
import pygame
from PIL import Image


def main(image_path):
    img = Image.open(image_path)

    pygame.init()

    size = img.width, img.height
    black = 0, 0, 0

    screen = pygame.display.set_mode(size)

    texture = pygame.image.load(image_path)

    hitboxes = []
    hitbox_rect = [0, 0, 0, 0]
    is_drawing = False

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:  # 1 == left button
                    mouse_pos = pygame.mouse.get_pos()
                    hitbox_rect[0] = mouse_pos[0]
                    hitbox_rect[1] = mouse_pos[1]
                    is_drawing = True

            if event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:  # 1 == left button
                    is_drawing = False
                    hitboxes.append(hitbox_rect.copy())
                    print(
                        "SDL_Rect {(int)(" + str(hitbox_rect[0] - (img.width/2)) + "), (int)(-" + str(img.height - hitbox_rect[1]) + "), (int) " + str(hitbox_rect[2]) + ", (int) " + str(hitbox_rect[3]) + "},")

        if is_drawing:
            mouse_pos = pygame.mouse.get_pos()
            hitbox_rect[2] = mouse_pos[0] - hitbox_rect[0]
            hitbox_rect[3] = mouse_pos[1] - hitbox_rect[1]

        screen.fill(black)
        screen.blit(texture, texture.get_rect())
        
        for h in hitboxes:
            pygame.draw.rect(screen, (0, 120, 0), h, 2)

        pygame.draw.rect(screen, (0, 255, 0), hitbox_rect, 2)
        pygame.display.flip()


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("No input file.")
        main("debug/debug_neutral.png")

    main(sys.argv[1])
