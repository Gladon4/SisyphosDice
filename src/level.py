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
        start_chunk_index = int(starting_position[0] // self.chunk_width)
        end_chunk_index = int(end_position[0] // self.chunk_width)

        for chunk in self.collision_chunks[start_chunk_index:end_chunk_index+1]:
            for hitbox in chunk:
                solve = self.__collision_check_with_hitbox(hitbox, starting_position, end_position)
                if solve != None:
                    return solve

    
    def __collision_check_with_hitbox(self, hitbox, starting_position, end_position):
        object_line = (starting_position, (end_position[0] - starting_position[0], end_position[1] - starting_position[1]))

        # hitbox_lines = [((origin_x, origin_y), (x, y))]
        hitbox_lines = [((hitbox["x"], hitbox["y"]), (hitbox["width"], 0)),
                        ((hitbox["x"], hitbox["y"]), (0, hitbox["height"])),
                        ((hitbox["x"], hitbox["y"]+hitbox["height"]), (hitbox["width"], 0)),
                        ((hitbox["x"]+hitbox["width"], hitbox["y"]), (0, hitbox["height"]))]
    

        for line in hitbox_lines:
            if object_line[1][0] == line[1][0] or object_line[1][1] == line[1][1]:
                continue

            equations = np.array([[-object_line[1][0], line[1][0]], 
                                  [-object_line[1][1], line[1][1]]])
            
            solutions = np.array([object_line[0][0]-line[0][0], 
                                  object_line[0][1]-line[0][1]])
            
            solve = np.linalg.solve(equations, solutions).tolist()
            

            if 0 <= solve[0] <= 1 and 0 <= solve[1] <= 1:
                return (line[0][0] + line[1][0] * solve[1], line[0][1] + line[1][1] * solve[1])
            
        return None

        # https://numpy.org/doc/stable/reference/generated/numpy.linalg.solve.html

        """
            P + x_0 * p = H + x_1 * h
            P-H = x_1*h - x_0*p

            P(x)-H(x) = x_1*h(x) - x_0*p(x)
            P(y)-H(y) = x_1*h(y) - x_0*p(y)
        """