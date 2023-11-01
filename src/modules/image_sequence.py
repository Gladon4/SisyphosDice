import pygame
import os


class ImageSequence:
    def __init__(self, screen, pos, directory, anim_speed=1):
        self.screen = screen
        self.pos = pos
        self.current_frame = 0
        self.images = []
        self.anim_speed = anim_speed

        image_list = sorted([i for i in os.listdir(directory)])

        for image in image_list:
            self.images.append(pygame.image.load(directory + "/" + image).convert_alpha())

    def draw_next_frame(self):
        self.screen.blit(self.images[self.current_frame], self.pos)
        self.current_frame = (self.current_frame + self.anim_speed) % len(self.images)

    def set_pos(self, pos):
        self.pos = pos

    def set_anim_speed(self, anim_speed):
        self.anim_speed = anim_speed
