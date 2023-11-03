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

    player = Player((32, 32), [500, 500], 3, 0.1)
    load_level("../data/levels/testStage.json")
    player.set_level(globalStateManager.level)
    player.setIsAffectedByGravity(False)
    main_camera = Camera(player, globalStateManager.level, (window_width, window_height), 5)
    globalStateManager.camera = main_camera

    gui_screen = pygame.Surface((window_width, window_height))
    gui_screen.set_colorkey(settings["colours"]["colour_key"])
    globalStateManager.font = Font(settings["paths"]["font"])

    particle_screen = pygame.Surface((window_width // 4, window_height // 4))
    globalStateManager.particle_system = ParticleSystem(particle_screen)

    # print(f"Settings: {settings}")

    last_collision = None

    last_frame_time = time.time()

    while globalStateManager.run:
        current_frame_time = time.time()
        delta_time = current_frame_time - last_frame_time

        event_manager()

        mx, my = pygame.mouse.get_pos()
        mouse_press = pygame.mouse.get_pressed()
        
        pressed_keyboard_keys = pygame.key.get_pressed()
                    

        if pressed_keyboard_keys[pygame.K_a]:
            if player.velocity[0] > -10:
                player.velocity[0] = -10
        if pressed_keyboard_keys[pygame.K_d]:
            if player.velocity[0] < 10:
                player.velocity[0] = 10

        if pressed_keyboard_keys[pygame.K_SPACE]:
            player.velocity[1] -= 5

    
        old_player_position = player.position[:]
        new_player_position = player.move(delta_time)

        if new_player_position[0]-old_player_position[0] != 0:
            solve = globalStateManager.level.check_for_collisions_on_path(player, old_player_position, new_player_position)
            if solve != None:
                last_collision = solve


        screen.fill(globalStateManager.settings["colours"]["black"])

        render_position = main_camera.update_and_draw(delta_time)
        player.draw(main_camera.screen)
        if globalStateManager.debug: draw_debug_info()
        if last_collision != None: pygame.draw.circle(main_camera.screen, (200, 0, 0), last_collision, 5)
        screen.blit(main_camera.screen, render_position)

        draw_particles(particle_screen, delta_time)
        draw_gui_screen(gui_screen, globalStateManager.font, delta_time)


        last_frame_time = current_frame_time
        pygame.display.update()
        # mainClock.tick(60)


    pygame.quit()
    return 0


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


def draw_gui_screen(gui_screen: pygame.Surface, font: Font, last_frame_time):
    gui_screen.fill(globalStateManager.settings["colours"]["colour_key"])

    if globalStateManager.show_fps:
        font.render(gui_screen, str(int(1 / last_frame_time)), (10, 10))
        globalStateManager.screen.blit(gui_screen, (0, 0))


def draw_particles(particle_screen: pygame.Surface, delta_time):
    settings = globalStateManager.settings

    particle_screen.fill(settings["colours"]["black"])
    particle_screen.set_colorkey(settings["colours"]["black"])

    globalStateManager.particle_system.update((0,0))
    globalStateManager.particle_system.draw()

    particle_scaled = pygame.transform.scale(particle_screen, (settings["window"]["width"], settings["window"]["height"]))
    globalStateManager.screen.blit(particle_scaled, (0, 0))


def draw_debug_info():
    for box in globalStateManager.level.hitboxes:
        pygame.draw.rect(globalStateManager.camera.screen, (0, 200, 0), (box["x"], box["y"], box["width"], box["height"]), 3)

    for i in range(len(globalStateManager.level.collision_chunks)):
        pygame.draw.line(globalStateManager.camera.screen, (0, 0, 200), (i*globalStateManager.level.chunk_width, 0), (i*globalStateManager.level.chunk_width, globalStateManager.settings["window"]["height"]))


if __name__ == "__main__":
    exit_code = main()

    print(f"Exited with code: {exit_code}")
    sys.exit()