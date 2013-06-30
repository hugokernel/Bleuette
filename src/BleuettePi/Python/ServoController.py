#!/usr/bin/python

import serial
from time import sleep
from array import array

SERVO0 = 1
SERVO1 = 2
SERVO2 = 4
SERVO3 = 8
SERVO4 = 16
SERVO5 = 32
SERVO6 = 64
SERVO7 = 128
SERVO8 = 256
SERVO9 = 512
SERVO10 = 1024
SERVO11 = 2048
SERVO12 = 4096
SERVO13 = 8192

ANA0 = '0'
ANA1 = '1'
ANA2 = '2'
ANA3 = '3'
ANA4 = '4'
ANA5 = '5'

class ServoController:

    SERVO_COUNT = 14

    HEADER = chr(255)

    CMD_INIT    = 'I'   # Init
    CMD_PAUSE   = 'P'   # Pause all servo
    CMD_RESUME  = 'R'   # Resume from pause
    CMD_CLEAR   = 'C'   # Clear all values
    CMD_SET     = 'S'   # Set consigne

    CMD_READAN0 = '0'   # Read analog value
    CMD_READAN1 = '1'   # Read analog value
    CMD_READAN2 = '2'   # Read analog value
    CMD_READAN3 = '3'   # Read analog value
    CMD_READAN4 = '4'   # Read analog value
    CMD_READAN5 = '5'   # Read analog value

    DELAY_BYTE = 0.002

    ACK = 'O'
    NACK = 'N'

    last_status_code = ''

    values = array('c', [chr(200)] * SERVO_COUNT)

    def __init__(self, device):
        self.com = serial.Serial(device, 9600, timeout = 1)
        #ser.open()

    def getResponse(self):
        return self.com.read(1)

    def getLastStatus(self):
        return (self.last_status_code == self.ACK)

    # Send only one command
    def command(self, cmd):
        self.com.write(self.HEADER)
        sleep(self.DELAY_BYTE)
        self.com.write(cmd)
        self.last_status_code = self.getResponse()
        return self. getLastStatus()

    def sendValues(self):
        self.com.write(self.HEADER)
        sleep(self.DELAY_BYTE)
        self.com.write(self.CMD_SET)

        control = 0
        for i in range(0, self.SERVO_COUNT):
            self.com.write(self.values[i])

            control += ord(self.values[i])

        #sleep(self.DELAY_BYTE)
        #print control % 255
        #print control % 256
        #self.com.write(chr(78)) #chr(control % 255))
        self.com.write(chr(control % 256))
        
        #sleep(self.DELAY_BYTE)

        self.last_status_code = self.getResponse()
        return self.getLastStatus()

    def init(self):
        return self.command(self.CMD_INIT)

    # Pause
    def pause(self):
        return self.command(self.CMD_PAUSE)

    # Resume
    def resume(self):
        return self.command(self.CMD_RESUME)

    # Set to 0 all consigne
    def clear(self):
        return self.command(self.CMD_CLEAR)

    # Read analog value
    def readAnalog(self, index):
        self.com.write(self.HEADER)
        sleep(self.DELAY_BYTE)
        self.com.write(index)

        #print index, ord(index), chr(index)

        #self.last_status_code = self.getResponse()
        return self.getResponse()
        #return self.getLastStatus()

    def setValue(self, index, value):
        values[index] = value 

'''
    void setValue(unsigned char, unsigned char);

    unsigned char getPosition(unsigned char);
    void getPositions(unsigned char &);

    void setValues(unsigned long, unsigned char *, unsigned char);
    void sendValues();

    unsigned long get(unsigned char);

    bool command(SERVO_CMD, unsigned char [], unsigned char);
'''

if __name__ == '__main__':

    Servo = ServoController('/dev/ttyAMA0')
    print Servo.init()
    print Servo.sendValues()
    print Servo.resume()
    print Servo.readAnalog(ANA2)

