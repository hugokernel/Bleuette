
import types, struct
from Serial import Serial

class Analog(Serial):

    AN0 = '0'
    AN1 = '1'
    AN2 = '2'
    AN3 = '3'
    AN4 = '4'
    AN5 = '5'
    AN6 = '6'
    AN7 = '7'

    VOLT_REF = 5.0
    RESOLUTION = 1024

    def __init__(self, mixed):
        self.com = mixed

    def makeWord(self, h, l):
        return struct.unpack("<H", l + h)[0]

    # Read value
    def read(self, index):

        retry = self.MAX_RETRY
        while True:
            self.com.write(self.HEADER)
            self.com.write(index)

            high = self.com.read(1)
            low = self.com.read(1)
            control = self.com.read(1)

            if (control and ord(control) == (ord(high) + ord(low)) % 256):
                break

            retry -= 1
            if retry == 0:
                raise Exception('Unable to communicate !')

        return self.makeWord(high, low)

    def status(self):
        self.com.write(self.HEADER)
        self.com.write(self.CMD_STATUS)
        print ord(self.com.read(1))
        return

    def readCurrent(self):

        retry = self.MAX_RETRY
        while True:
            self.com.write(self.HEADER)
            self.com.write('A')

            high = self.com.read(1)
            low = self.com.read(1)
            control = self.com.read(1)

            if (control and ord(control) == (ord(high) + ord(low)) % 256):
                break

            retry -= 1
            if retry == 0:
                raise Exception('Unable to communicate !')

        return self.makeWord(high, low)

    def convertInVolt(self, value):
        coeff = self.VOLT_REF / self.RESOLUTION
        return value * coeff

    def convertInAmpere(self, value):
        pass

