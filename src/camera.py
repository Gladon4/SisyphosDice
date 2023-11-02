from entity import Entity
from level import Level

import pygame
import math

class Camera():
    def __init__(self, target: Entity, level: Level, screen_size, speed) -> None:
        self.target = target
        self.level = level
        self.position = (0,0)
        self.screen = pygame.Surface(level.level_data["size"])
        self.player = target
        self.screen_size = screen_size
        self.speed = speed

    def draw(self):
        self.screen.fill((0,0,0))
    
        self.screen.blit(self.level.level_image, (0,0))

        pygame.draw.circle(self.screen, (255,0,0), self.player.position, 10)


    def update(self, last_frame_time):
        center = (self.position[0] + self.screen_size[0] / 2, self.position[1] + self.screen_size[1] / 2)

        offset = ((self.target.position[0] - center[0]) * last_frame_time * self.speed, (self.target.position[1] - center[1]) * last_frame_time * self.speed)

        self.position = (self.position[0] + offset[0], self.position[1] + offset[1])

        render_x = center[0] - self.screen_size[0] / 2
        render_x = max(render_x, 0)
        render_x = min(render_x, self.level.level_data["size"][0] -  self.screen_size[0])

        render_y = center[1] - self.screen_size[1] / 2
        render_y = max(render_y, 0)
        render_y = min(render_y, self.level.level_data["size"][1] - self.screen_size[1])
        
        return (-render_x, -render_y)

    
    def update_and_draw(self, last_frame_time):
        render_position = self.update(last_frame_time)
        self.draw()
        return render_position