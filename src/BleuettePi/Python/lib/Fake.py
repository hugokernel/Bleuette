
from random import randint

class GPIO:

    BOARD = 1
    IN = 0
    RISING = 0
    BOTH = 1

    @staticmethod
    def setmode(a):
        pass

    @staticmethod
    def setup(a, b):
        pass

    @staticmethod
    def add_event_detect(a, b, callback, bouncetime):
        pass

class hmc5883l:
    pass

class ADXL345:
    pass

class MCP230XX:

    INPUT = True
    OUTPUT = False

    def __init__(self, a, b):
        pass

    def config(self, a, b):
        pass

    def pullup(self, a, b):
        pass

    def input(self, a):
        return randint(0, 1)

