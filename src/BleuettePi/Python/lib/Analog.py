
from Serial import Serial
from Define import BPi_Cmd
import types, struct

class Analog(Serial):

    AN0 = '0'
    AN1 = '1'
    AN2 = '2'
    AN3 = '3'
    AN4 = '4'
    AN5 = '5'
    AN6 = '6'
    AN7 = '7'

    CURRENT_IO = AN7
    VOLTAGE_IO = AN6

    VOLT_REF = 5.0
    RESOLUTION = 1024

    def __init__(self, serial):
        self.serial = serial

    def makeWord(self, h, l):
        return struct.unpack("<H", l + h)[0]

    # Read value
    def __read(self, cmd):
        retry = self.MAX_RETRY
        while True:
            self.serial.write(self.HEADER)
            self.serial.write(cmd)

            high = self.serial.read(1)
            low = self.serial.read(1)
            control = self.serial.read(1)

            if (control and ord(control) == (ord(high) + ord(low)) % 256):
                break

            retry -= 1
            if retry == 0:
                raise Exception('Unable to communicate !')

        return self.makeWord(high, low)

    def read(self, index):
        if BPi_Cmd.isValid('READAN' + index) == False:
            raise Exception('Invalid input !')

        return self.__read(index)

    def readCurrent(self):
        return self.__read(BPi_Cmd.CURRENT)

    def getCurrent(self):
        total = 0;
        val = self.read(Analog.AN7)
        val += self.read(Analog.AN7)
        val += self.read(Analog.AN7)
        val += self.read(Analog.AN7)
        val += self.read(Analog.AN7)
        return val / 5

    def convertInVolt(self, value):
        coeff = self.VOLT_REF / self.RESOLUTION
        return value * coeff

    def convertInAmpere(self, value):
        pass

