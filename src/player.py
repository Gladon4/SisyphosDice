from entity import Entity

class Player(Entity):
    def __init__(self, size, position) -> None:
        super().__init__(size, position)