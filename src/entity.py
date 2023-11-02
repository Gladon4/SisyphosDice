class Entity():
    def __init__(self, size, position) -> None:
        self.size = size
        self.position = position
        self.level = None

    
    def set_level(self, level):
        self.level = level
