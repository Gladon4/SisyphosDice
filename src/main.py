#!/bin/python

from modules.particle_system import ParticleSystem
from modules.font import Font
from modules.image_sequence import ImageSequence
from state_manager import StateManager

from player import Player
from camera import Camera
from level import Level

import pygame
import time
import sys
import json

globalStateManager = StateManager()

def main() -> int:
    settings_path = "../data/settings.json"
    settings = load_settings(settings_path)

    globalStateManager.settings = settings

    window_width, window_height = settings["window"]["width"], settings["window"]["height"]

    mainClock = pygame.time.Clock()
    pygame.init()
    pygame.display.set_caption(settings["window"]["name"])
    screen = pygame.display.set_mode((window_width, window_height))

    globalStateManager.screen = screen

    player = Player((32, 32), (500, 500))
    load_level("../data/levels/testStage.json")
    main_camera = Camera(player, globalStateManager.level, (window_width, window_height), 30)

    gui_screen = pygame.Surface((window_width, window_height))
    gui_screen.set_colorkey(settings["colours"]["colour_key"])
    globalStateManager.font = Font(settings["paths"]["font"])

    particle_screen = pygame.Surface((window_width // 4, window_height // 4))
    globalStateManager.particle_system = ParticleSystem(particle_screen)

    # print(f"Settings: {settings}")

    last_fixed_step_time = 0
    last_frame_time = 0

    while globalStateManager.run:
        current_frame_time = time.time()

        update_content = {"main_camera": main_camera,
                          "gui_screen": gui_screen,
                          "particle_screen": particle_screen,
                          "player": player}
        
        update(screen, update_content, current_frame_time - last_frame_time)
        last_frame_time = current_frame_time

        if time.time() - last_fixed_step_time >= settings["game"]["fixed_step_size"]:
            fixed_update_content = {"player": player
                                    }
            fixed_update(fixed_update_content, current_frame_time - last_fixed_step_time)
            last_fixed_step_time = current_frame_time

        # mainClock.tick(60)


    pygame.quit()
    return 0


def update(screen, update_content, last_frame_time):
    screen.fill(globalStateManager.settings["colours"]["black"])

    render_position = update_content["main_camera"].update_and_draw(last_frame_time)
    screen.blit(update_content["main_camera"].screen, render_position)

    draw_particles(update_content["particle_screen"], last_frame_time)
    draw_gui_screen(update_content["gui_screen"], globalStateManager.font, last_frame_time)

    pygame.display.update()


def fixed_update(fixed_update_content, last_fixed_step_time):
    event_manager()

    mx, my = pygame.mouse.get_pos()
    mouse_press = pygame.mouse.get_pressed()
    
    pressed_keyboard_keys = pygame.key.get_pressed()

    if pressed_keyboard_keys[pygame.K_a]:
        fixed_update_content["player"].position = (fixed_update_content["player"].position[0] - 10, fixed_update_content["player"].position[1])
    if pressed_keyboard_keys[pygame.K_d]:
        fixed_update_content["player"].position = (fixed_update_content["player"].position[0] + 10, fixed_update_content["player"].position[1])
    if pressed_keyboard_keys[pygame.K_w]:
        fixed_update_content["player"].position = (fixed_update_content["player"].position[0], fixed_update_content["player"].position[1] - 10)
    if pressed_keyboard_keys[pygame.K_s]:
        fixed_update_content["player"].position = (fixed_update_content["player"].position[0], fixed_update_content["player"].position[1] + 10)


def load_settings(path: str) -> dict:
    file = open(path)
    data = json.load(file)
    file.close()

    return data


def load_level(path: str) -> None:
    globalStateManager.level = Level(path)


def event_manager():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            globalStateManager.run = False
        
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                globalStateManager.run = False

            if event.key == pygame.K_F1:
                globalStateManager.show_fps = not globalStateManager.show_fps
            if event.key == pygame.K_F2:
                globalStateManager.debug = not globalStateManager.debug
            if event.key == pygame.K_F3:
                globalStateManager.light_disable = not globalStateManager.light_disable


def draw_gui_screen(gui_screen, font, last_frame_time):
    gui_screen.fill(globalStateManager.settings["colours"]["colour_key"])

    if globalStateManager.show_fps:
        font.render(gui_screen, str(int(1 / last_frame_time)), (10, 10))
        globalStateManager.screen.blit(gui_screen, (0, 0))


def draw_particles(particle_screen, last_frame_time):
    settings = globalStateManager.settings

    particle_screen.fill(settings["colours"]["black"])
    particle_screen.set_colorkey(settings["colours"]["black"])

    globalStateManager.particle_system.update((0,0))
    globalStateManager.particle_system.draw()

    particle_scaled = pygame.transform.scale(particle_screen, (settings["window"]["width"], settings["window"]["height"]))
    globalStateManager.screen.blit(particle_scaled, (0, 0))


if __name__ == "__main__":
    exit_code = main()

    print(f"Exited with code: {exit_code}")
    sys.exit()