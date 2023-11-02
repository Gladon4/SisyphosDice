import json
import pygame

class Level():
    def __init__(self, file_path) -> None:
        file = open(file_path)
        self.level_data = json.load(file)
        file.close()

        self.level_image = pygame.image.load(self.level_data["texture"]).convert_alpha()
    
    