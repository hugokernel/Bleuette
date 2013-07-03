#!/usr/bin/python
import serial, types, time, struct
from array import array

from Serial import Serial
from Servo import Servo
from Analog import Analog
import RPi.GPIO as GPIO

'''
import threading, copy
class MyThread ( threading.Thread ):

    def run ( self ):
        time.sleep(5)
        print 'Done'
'''

class BleuettePi:

    INTA = 26
    INTB = 15
    INTC = 22
    INTD = 12

    ser = None

    def __init__(self, mixed):
        self.com = Serial().connect(mixed)

        self.Servo = Servo(self.com)
        self.Analog = Analog(self.com)

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

        while True:
            time.sleep(20)

    def interrupt(self, index):
        if index == self.INTA:
            print "Interrupt from GPA"
        elif index == self.INTB:
            print "Interrupt from GPB"
        elif index == self.INTC:
            print "Interrupt from RTC"
        elif index == self.INTD:
            print "Interrupt from PIC !"
            print self.Analog.status()

    def getCurrent(self):
        total = 0;
        val = BPi.Analog.read(Analog.AN7)
        val += BPi.Analog.read(Analog.AN7)
        val += BPi.Analog.read(Analog.AN7)
        val += BPi.Analog.read(Analog.AN7)
        val += BPi.Analog.read(Analog.AN7)
        return val / 5

class Fake:
    @staticmethod
    def write(self):
        pass
    @staticmethod
    def read(self):
        pass

if __name__ == '__main__':

    BPi = BleuettePi('/dev/ttyAMA0')
    #BPi = BleuettePi(Fake)

    val = BPi.Analog.status()
    val = BPi.Analog.read(Analog.AN0)
    print BPi.Analog.convertInVolt(val)

    '''
    print "Current:", BPi.Analog.convertInVolt(BPi.Analog.readCurrent())

    value = chr(50)
    BPi.Servo.setValue(5, value)
    BPi.Servo.setValue(4, value)
    BPi.Servo.setValue(3, value)
#    BPi.Servo.setValue(13, chr(5))
    #BPi.Servo.setValue(5, chr(1))
    BPi.Servo.sendValues()
    '''

    print "Current:", BPi.Analog.convertInVolt(BPi.Analog.readCurrent())

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
