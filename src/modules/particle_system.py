import time

from modules.particle import Particle


class ParticleSystem:
    def __init__(self, screen, frame_offset=1):
        self.frame_offset = frame_offset
        self.frame = 1
        self.start_time = time.time()
        self.delta_time = 0
        self.screen = screen
        self.particles = []
        self.paused = False
        self.pos = [0, 0]

    def update(self, pos):
        self.pos = pos[:]
        if not self.paused and self.frame % self.frame_offset == 0:
            self.delta_time = time.time() - self.start_time
            par = []
            old_par = self.particles

            # Update all particles
            for p in range(len(self.particles)):
                particle = self.particles[p]
                if particle.remaining_lifespan > 0:
                    par.append(particle)
                    particle.update(self.delta_time)

            self.particles = par[:]

            # Delete particles that are finished playing
            while len(old_par) > 0:
                if old_par[0] not in self.particles:
                    del old_par[0]
                else:
                    old_par = old_par[1:]

            self.start_time = time.time()

    def draw(self):
        if self.frame % self.frame_offset == 0:
            # self.screen.fill((0, 0, 0))
            self.frame = 1
            for particle in self.particles:
                particle.draw(self.screen, self.delta_time, self.pos)

        else:
            self.frame += 1

    def particle(self, pos, abs_velocity, rel_velocity, size, amount, color, density, gravity, lifespan, trail, glow):
        self.particles.append(Particle(pos, abs_velocity, rel_velocity, size, amount, color, density, gravity, lifespan, trail, glow, self.pos))

    def pause(self):
        self.start_time = time.time()
        self.paused = not self.paused

    def explosion(self, pos, strength, size):
        self.particle(pos, (0, -strength / 10), strength / 10, size * 10, size, (150, 150, 150), 0.3, (0, 0.01), 7, 0, 0)
        self.particle(pos, (0, -strength * 2), strength * 4, size, size * 20, (220, 40, 0), 0.5, (0, 0.1), 5, 6, 4)
        self.particle(pos, (0, -strength), strength, size * 5, size * 7, (150, 150, 150), 0.5, (0, 0.01), 7, 0, 0)
        self.particle(pos, (0, -strength * 1.5), strength * 3, size * 1.5, size * 5, (75, 50, 50), 0.5, (0, 0.3), 5, 1, 0)
