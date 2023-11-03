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

        self.position[0] += self.velocity[0] * delta_time * 60
        self.position[1] += self.velocity[1] * delta_time * 60

        # print(self.position)

        return self.position
