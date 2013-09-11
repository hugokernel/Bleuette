
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

    def __init__(self, port=1, address=0x1E, gauss=1.3, declination=(0,0)):
        pass

    def axes(self):
        return (0, 0, 0)

    def heading(self):
        return (0, 0)

    def degrees(self, (degrees, minutes)):
        return ""

class ADXL345:
    
    def scaledAccelCal(self, a):
        return [0, 1, 2]

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
        return 1#return randint(0, 1)

class Analog:
    
    def __init__(self, a):
        pass

    def getCurrent(self):
        return 0

