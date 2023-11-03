from entity import Entity

import pygame

class Player(Entity):
    def __init__(self, size, position, mass, drag) -> None:
        super().__init__(size, position, mass, drag)

    
    def draw(self, screen: pygame.Surface):
        pygame.draw.rect(screen, (255, 0, 0), (self.position[0] - self.size[0]/2, self.position[1] - self.size[1]/2, self.size[0], self.size[1]))