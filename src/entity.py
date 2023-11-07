from level import Level

class Entity():
    def __init__(self, size: tuple, position: tuple, mass: float, drag: float) -> None:
        self.size = size
        self.mass = mass

        self.position = position
        self.velocity = [0, 0]
        self.acceleration = [0, 0]

        self.drag = drag

        self.level: Level
        self.level = None

        self.isAffectedByGravity = True
        self.on_ground = False


    def set_level(self, level: Level) -> None:
        self.level = level


    def setIsAffectedByGravity(self, isAffectedByGravity: bool) -> None:
        self.isAffectedByGravity = isAffectedByGravity


    def __apply_gravity(self, delta_time) -> None:
        if self.isAffectedByGravity and self.level:
            self.velocity[1] += self.level.gravity * delta_time * 60


    def move(self, delta_time) -> tuple:
        self.__apply_gravity(delta_time)

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

        self.on_ground = False

        solve = self.level.check_for_collisions_on_path(self, self.position, new_position)
        if solve != []:
            can_move_x = True
            can_move_y = True

            for collision in solve:
                hitbox = collision[0]
                collision_line = collision[1]
                collision_with_horizontal = collision_line[1][1] == 0

                if hitbox["y"] > self.position[1] and collision_with_horizontal:
                    self.on_ground = True

                if collision_with_horizontal:
                    can_move_y = False

                else:
                    can_move_x = False

            if can_move_x:
                self.position[0] = new_position[0]
            else:
                self.velocity[0] = 0
                self.acceleration[0] = 0
            
            if can_move_y:
                self.position[1] = new_position[1]
            else:
                self.velocity[1] = 0
                self.acceleration[1] = 0

        else:
            self.position = new_position

            
        # print(self.position)

        return self.position
