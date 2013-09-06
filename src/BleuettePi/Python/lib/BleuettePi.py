#!/usr/bin/python
import sys, serial, types, time, struct, logging

from array  import array
from Serial import Serial
import Servo
from Define import BPi_Cmd

import config as Config
from Data import Data

if Config.FAKE_MODE:
    from Fake import GPIO, hmc5883l, ADXL345, MCP230XX, Analog
else:
    from drivers.hmc5883l import hmc5883l
    from drivers.ADXL345 import ADXL345
    import RPi.GPIO as GPIO
    from drivers.Adafruit_MCP230xx import Adafruit_MCP230XX as MCP230XX
    from Analog import Analog

class BleuettePi_Compass:

    # Declination, ~Paris : 0.7
    # http://magnetic-declination.com/
    DECLINATION = (0, 7)

    def get(self):
        compass = hmc5883l(gauss = 4.7, declination = self.DECLINATION)
        return compass.degrees(compass.heading())

    def realTime(self):
        compass = hmc5883l(gauss = 4.7, declination = self.DECLINATION)
        while True:
            sys.stdout.write("\rHeading: " + compass.degrees(compass.heading()) + "     ")
            sys.stdout.flush()
            time.sleep(0.5)

class BleuettePi_Accelerometer:

    def get(self):
        a = ADXL345();
        return a.scaledAccelCal(10);

    def realTime(self):
        a = ADXL345()
        while True:
            x, y, z = a.scaledAccelCal(10)
            sys.stdout.write('\rx:%d, y:%d, z:%d            ' % (x, y, x))
            sys.stdout.flush()
            time.sleep(0.5)

class BleuettePi_GroundSensor:

    mcp = None

    def __init__(self, mcp):
        self.mcp = mcp
        for i in range(0, 7):
            self.mcp.config(i, MCP230XX.INPUT)
            self.mcp.pullup(i, True)

    def get(self):
        inputs = [ 0, 0, 0, 0, 0, 0, 0 ]
        for i in range(0, 7):
            inputs[i] = self.mcp.input(i)
        return inputs

    def realTime(self):
        inputs = [ 0, 0, 0, 0, 0, 0, 0 ]
        while True:
            for i in range(0, 7):
                inputs[i] = 'HIGH' if mcp.input(i) else 'LOW '
            sys.stdout.write('\r0:%s, 1:%s, 2:%s, 3:%s, 4:%s, 5:%s, 6:%s' % (inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]))
            sys.stdout.flush()
            time.sleep(0.5)

class BleuettePi(Serial):

    INTA = 26
    INTB = 15
    INTC = 22
    INTD = 12

    STATUS_MAX_CURRENT_REACHED = 0

    MCP_ADDRESS = 0x20

    serial = None
    mcp = None

    Servo = None
    Compass = None
    Accelerometer = None
    GroundSensor = None

    def __init__(self, mixed):
        self.logger = logging.getLogger('BleuettePi')

        self.serial = Serial()
        self.serial.connect(mixed)

        self.mcp = MCP230XX(self.MCP_ADDRESS, 16)

        self.Servo = Servo.Servo(self.serial, fakemode = Config.FAKE_MODE)
        self.Servo.init()
        Servo.Servo_Trim.values = Data.Instance().get(['servo', 'trims'])
        Servo.Servo_Limit.values = Data.Instance().get(['servo', 'limits'])

        self.Analog = Analog(self.serial)

        self.Compass = BleuettePi_Compass()
        self.Accelerometer = BleuettePi_Accelerometer()
        self.GroundSensor = BleuettePi_GroundSensor(self.mcp)

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
            self.logger.info("Interrupt from GPA")
        elif index == self.INTB:
            self.logger.info("Interrupt from GPB")
        elif index == self.INTC:
            self.logger.info("Interrupt from RTC")
        elif index == self.INTD:
            self.logger.info("Interrupt from PIC")
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

    def reset():
        # Todo !
        pass

    def getStatus(self):
        self.serial.write(self.HEADER)
        self.serial.write(BPi_Cmd.STATUS)
        status = self.serial.read(1)

        if ord(status) & (1 << self.STATUS_MAX_CURRENT_REACHED):
            self.logger.warning("Max current reached !")

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
