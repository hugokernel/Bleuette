
from BleuettePi import BleuettePi
from Sequencer import Sequencer
from Analog import Analog
import config as Config

class Fake:
    @staticmethod
    def write(self):
        pass
    @staticmethod
    def read(self):
        return 'a'

class Bleuette:

    BPi = None
    Sequencer = None

    def __init__(self):
        if Config.SERIAL_DEV:
            self.BPi = BleuettePi(Config.SERIAL_DEV)
        else:
            self.BPi = BleuettePi(Fake)

        self.Sequencer = Sequencer(self.BPi.Servo)
    

