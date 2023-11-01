import pygame
import random
import math
from pygame.locals import *


class Particle:
    def __init__(self, pos, abs_velocity, rel_velocity, size, amount, color, density, gravity, lifespan, trail, glow, screen_pos):
        self.size = size
        self.amount = amount
        self.density = density
        self.gravity = gravity
        self.color = color
        self.total_lifespan = lifespan
        self.remaining_lifespan = lifespan
        self.trail = trail
        self.glow = glow
        self.screen_pos = screen_pos
        self.pos_offset = [0, 0]

        self.parts = []
        for i in range(amount):
            posX = (random.random() * 2 - 1)
            posY = (random.random() * 2 - 1)

            if math.sqrt(posX ** 2 + posY ** 2) > 1:
                fac = random.randint(9, 11) / 10 / math.sqrt(posX ** 2 + posY ** 2)
                posX = posX * fac * (10 / density)
                posY = posY * fac * (10 / density)
            else:
                posX = posX * (10 / density)
                posY = posY * (10 / density)

            velX = abs_velocity[0] + (posX / 100) + rel_velocity / 30 * posX
            velY = abs_velocity[1] + (posY / 100) + rel_velocity / 30 * posY

            self.parts.append({"pos": [pos[0] + posX, pos[1] + posY], "velocity": [velX, velY], "lifespan": lifespan * random.random(), "trail": []})

    def draw(self, screen, delta_time, screen_pos):
        last_offset = self.pos_offset[:]
        pos_offset = [screen_pos[0] - self.screen_pos[0], self.screen_pos[1] - screen_pos[1]]
        self.pos_offset = pos_offset
        offset_to_last_frame = [pos_offset[0] - last_offset[0]]
        for part in self.parts:
            if part["lifespan"] > 0:
                position = [part["pos"][0] // 4 + pos_offset[0] // 4, part["pos"][1] // 4 + pos_offset[1] // 4]

                color = (self.color[0] * part["lifespan"] / self.total_lifespan, self.color[1] * part["lifespan"] / self.total_lifespan, self.color[2] * part["lifespan"] / self.total_lifespan)
                size = self.size * part["lifespan"] / self.total_lifespan
                pygame.draw.circle(screen, color, position, size)

                radius = self.glow * part["lifespan"] / self.total_lifespan
                glow = pygame.Surface((radius * 2, radius * 2))
                pygame.draw.circle(glow, color, (radius, radius), radius)

                glow.set_colorkey((0, 0, 0))

                screen.blit(glow, (position[0] - self.glow * part["lifespan"] / self.total_lifespan, position[1] - self.glow * part["lifespan"] / self.total_lifespan), special_flags=BLEND_RGB_ADD)

                if part["trail"] != []:
                    points = []
                    for i in range(len(part["trail"])):
                        factor = len(part["trail"]) - (i + 1)
                        points += [(int(part["trail"][i][0] + offset_to_last_frame[0] * factor / 4), int(part["trail"][i][1]))]
                    if len(points) >= 2:
                        pygame.draw.lines(screen, color, False, points, int(size))

                if self.trail != 0:
                    if len(part["trail"]) > self.trail * 0.016 / (delta_time + 0.0000001):
                        part["trail"] = part["trail"][1:]
                    part["trail"].append(position)

    def update(self, delta_time):
        self.remaining_lifespan -= delta_time

        for part in self.parts:
            part["velocity"][0] += self.gravity[0] * delta_time * 60
            part["velocity"][1] += self.gravity[1] * delta_time * 60

            part["pos"][0] += part["velocity"][0] * delta_time * 60
            part["pos"][1] += part["velocity"][1] * delta_time * 60

            part["lifespan"] -= delta_time
