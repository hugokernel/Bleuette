#!/usr/bin/python
import serial, types, time, struct

from array  import array
from Serial import Serial
from Servo  import Servo
from Analog import Analog
from Define import BPi_Cmd
import RPi.GPIO as GPIO

class BleuettePi(Serial):

    INTA = 26
    INTB = 15
    INTC = 22
    INTD = 12

    STATUS_MAX_CURRENT_REACHED = 0

    serial = None

    def __init__(self, mixed):
        self.serial = Serial()
        self.serial.connect(mixed)

        self.Servo = Servo(self.serial)
        self.Analog = Analog(self.serial)

        # Init mode
        GPIO.setmode(GPIO.BOARD)

        GPIO.setup(self.INTA, GPIO.IN)
        GPIO.setup(self.INTB, GPIO.IN)
        GPIO.setup(self.INTC, GPIO.IN)
        GPIO.setup(self.INTD, GPIO.IN)

        GPIO.add_event_detect(self.INTA, GPIO.RISING, callback = self.interrupt, bouncetime = 300)
        GPIO.add_event_detect(self.INTB, GPIO.RISING, callback = self.interrupt, bouncetime = 300)
        GPIO.add_event_detect(self.INTC, GPIO.RISING, callback = self.interrupt, bouncetime = 300)
        GPIO.add_event_detect(self.INTD, GPIO.RISING, callback = self.interrupt, bouncetime = 300)

    def interrupt(self, index):
        if index == self.INTA:
            print "Interrupt from GPA"
        elif index == self.INTB:
            print "Interrupt from GPB"
        elif index == self.INTC:
            print "Interrupt from RTC"
        elif index == self.INTD:
            print "Interrupt from PIC !"
            self.getStatus()

    '''
    def setCurrentAlarmLevel(self, level):
        print "Level", level

        if level >= 1024:
            raise Exception('Max current alarm level is 1024 !')

        data = struct.pack("@i", level)
        level = [ data[0], data[1] ]

        return self.command(BPi_Cmd.SET_MAX, level)
    '''

    def getStatus(self):
        self.serial.write(self.HEADER)
        self.serial.write(BPi_Cmd.STATUS)
        status = self.serial.read(1)

        if ord(status) & (1 << self.STATUS_MAX_CURRENT_REACHED):
            print "Max current reached !"

        return status



class Fake:
    @staticmethod
    def write(self):
        pass
    @staticmethod
    def read(self):
        return 'a'

if __name__ == '__main__':

    #BPi = BleuettePi('/dev/ttyAMA0')
    BPi = BleuettePi(Fake)

    #print BPi.getStatus()
    #BPi.Servo.init()

    val = BPi.Analog.readCurrent()
    val = BPi.Analog.read(Analog.AN0)
#    print BPi.Analog.convertInVolt(val)

    print "Current:", BPi.Analog.convertInVolt(BPi.Analog.readCurrent())

#    print "Current:", BPi.Analog.convertInVolt(BPi.Analog.readCurrent())

    #while True:
    #    print BPi.getCurrent()
    #    time.sleep(1)

    #print BPi.monitor()

'''
    print "Set pos"
    BPi.Servo.setValues(Servo._2 | Servo._4, [ chr(90), chr(120) ])
    BPi.Servo.sendValues()
    #BPi.Servo.clear()
    #BPi.Servo.dump()

    time.sleep(2)

    print "Next"
    BPi.Servo.setValues(Servo._2 | Servo._4, [ chr(200), chr(20) ])
    BPi.Servo.sendValues()

#BleuettePi.Servo.sendValues()
#BleuettePi.ADC.get(0)
'''
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
