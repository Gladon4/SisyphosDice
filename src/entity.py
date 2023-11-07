from level import Level

class Entity():
    def __init__(self, size, position, mass, drag) -> None:
        self.size = size
        self.mass = mass

        self.position = position
        self.velocity = [0, 0]
        self.acceleration = [0, 0]

        self.drag = drag

        self.level: Level
        self.level = None

        self.isAffectedByGravity = True


    def set_level(self, level: Level) -> None:
        self.level = level


    def setIsAffectedByGravity(self, isAffectedByGravity: bool) -> None:
        self.isAffectedByGravity = isAffectedByGravity


    def __gravity(self, delta_time) -> None:
        if self.isAffectedByGravity and self.level:
            self.velocity[1] += self.level.gravity * delta_time * 60


    def move(self, delta_time) -> tuple:
        self.__gravity(delta_time)

        drag_froce = (-self.velocity[0] * self.drag, -self.velocity[1] * self.drag)

        self.velocity[0] += (self.acceleration[0] + drag_froce[0]) * delta_time * 60
        self.velocity[1] += (self.acceleration[1] + drag_froce[1]) * delta_time * 60

        if -0.01 <= self.velocity[0] <= 0.01:
            self.velocity[0] = 0

        # self.position[0] += self.velocity[0] * delta_time * 60
        # self.position[1] += self.velocity[1] * delta_time * 60

        new_position = self.position[:]
        new_position[0] += self.velocity[0] * delta_time * 60
        new_position[1] += self.velocity[1] * delta_time * 60

        has_moved = new_position[0]-self.position[0] != 0 or new_position[1]-self.position[1] != 0

        if has_moved:
            solve = self.level.check_for_collisions_on_path(self, self.position, new_position)
            if solve != None:
                hitbox = solve[0]
                collision_line = solve[1]
                collision_with_horizontal = collision_line[1][1] == 0

                if collision_with_horizontal:
                    self.position[0] = new_position[0]
                    self.velocity[1] = 0
                    self.acceleration[1] = 0

                else:
                    self.position[1] = new_position[1]
                    self.velocity[0] = 0
                    self.acceleration[0] = 0

            else:
                self.position = new_position

            
        # print(self.position)

        return self.position
