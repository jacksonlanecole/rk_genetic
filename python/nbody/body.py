class Body(object):
    def __init__(self, mass, position, velocity, name = None):
        if (name != None):
            self.name = name

        self.mass = mass
        self.position = position
        self.velocity = velocity

