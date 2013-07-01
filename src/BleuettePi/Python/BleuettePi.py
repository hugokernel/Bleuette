#!/usr/bin/python
import serial, types, time
from array import array

from Serial import Serial
from Servo import Servo
from Analog import Analog

'''
import threading, copy
class MyThread ( threading.Thread ):

    def run ( self ):
        time.sleep(5)
        print 'Done'
'''

class BleuettePi:

    def __init__(self, mixed):
        self.com = Serial().connect(mixed)

        self.Servo = Servo(self.com)
        self.Analog = Analog(self.com)


class Fake:
    @staticmethod
    def write(self):
        pass
    @staticmethod
    def read(self):
        pass

if __name__ == '__main__':

    #BPi = BleuettePi('/dev/ttyAMA0')
    BPi = BleuettePi(Fake)
    BPi.Servo.setValues(Servo._2 | Servo._4, [ chr(90), chr(120) ])
    BPi.Servo.clear()
    BPi.Servo.dump()


#BleuettePi.Servo.sendValues()
#BleuettePi.ADC.get(0)

'''
    MyThread().start()

BleuettePi
- Servos
- ADC 6 + 2
- GY-27
- Interruption
- 16 IO

 +-> BleuettePi
   +-> Serial
     +-> Servo
     +-> ADC
   +-> I2C
     +-> GY-27
     +-> IO
     +-> RTC
   +-> IO
     +-> Io / Interruption

'''
