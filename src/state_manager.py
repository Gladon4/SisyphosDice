from modules.particle_system import ParticleSystem
from modules.font import Font

from level import Level
from camera import Camera

import pygame

class StateManager():
    def __init__(self) -> None:
        self.run = True

        self.show_fps = False
        self.debug = False
        self.light_disable = False

        self.particle_system: ParticleSystem
        self.font: Font
        self.screen: pygame.Surface
        self.level: Level
        self.camera: Camera

        self.settings: dict