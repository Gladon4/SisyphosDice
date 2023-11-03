import json
import pygame
import numpy as np

class Level():
    def __init__(self, file_path) -> None:
        file = open(file_path)
        level_data = json.load(file)
        file.close()

        self.hitboxes = level_data["hitboxes"]
        self.lights = level_data["lights"]
        self.brightness = level_data["brightness"]
        self.gravity = level_data["gravity"]

        self.level_image = pygame.image.load(level_data["texture"]).convert_alpha()
        self.size = level_data["size"]

        self.chunk_width = 250

        self.__make_chunks()


    def __make_chunks(self):
        chunks = self.size[0] // self.chunk_width
        self.collision_chunks = [[] for _ in range(chunks + 1)]

        for hitbox in self.hitboxes:
            start_index = hitbox["x"] // self.chunk_width
            end_index = (hitbox["x"] + hitbox["width"]) // self.chunk_width

            for i in range(start_index, end_index+1):
                self.collision_chunks[i].append(hitbox)


    
    def check_for_collisions_on_path(self, object, starting_position, end_position):
        start_chunk_index = starting_position // self.chunk_width
        end_chunk_index = end_position // self.chunk_width

        for chunk in self.collision_chunks[start_chunk_index:end_chunk_index+1]:
            for hitbox in chunk:
                pass

    
    def __collision_check_with_hitbox(self, hitbox, starting_position, end_position):
        hitbox_lines = []
        
        # https://numpy.org/doc/stable/reference/generated/numpy.linalg.solve.html
        # equatiobs = np.array([])
        # solutions = np.array([starting_position[0]-])