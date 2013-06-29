#!/usr/bin/python

import serial
from time import sleep
from array import array

SERVO_0 = 1
SERVO_1 = 2
SERVO_2 = 4
SERVO_3 = 8
SERVO_4 = 16
SERVO_5 = 32
SERVO_6 = 64
SERVO_7 = 128
SERVO_8 = 256
SERVO_9 = 512
SERVO_10 = 1024
SERVO_11 = 2048
SERVO_12 = 4096
SERVO_13 = 8192

class ServoController:

    SERVO_COUNT = 14

    HEADER = chr(255)

    CMD_INIT      = 'I'
    CMD_PAUSE     = 'P'
    CMD_RESUME    = 'R'
    CMD_CLEAR     = 'C'
    CMD_SET       = 'S'

    DELAY_BYTE = 0.002

    ACK = 'O'
    NACK = 'N'

    last_status_code = ''

    values = array('c', ['a'] * SERVO_COUNT)

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
#            print ord(self.values[i])

        sleep(self.DELAY_BYTE)
        #print control % 255
        print control % 255
        self.com.write(chr(78)) #chr(control % 255))
        
        sleep(self.DELAY_BYTE)

        self.last_status_code = self.getResponse()
        return self. getLastStatus()

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

    def setValue(self, index, value):
        values[index] = value 

'''
    ServoController();
    ServoController(HardwareSerial &);

    void setValue(unsigned char, unsigned char);

    unsigned char getPosition(unsigned char);
    void getPositions(unsigned char &);

    void setValues(unsigned long, unsigned char *, unsigned char);
    void sendValues();

    unsigned long get(unsigned char);

    inline bool getLastStatus();

    bool command(SERVO_CMD, unsigned char [], unsigned char);
'''

if __name__ == '__main__':

    Servo = ServoController('/dev/ttyAMA0')
    print Servo.init()
    print Servo.sendValues()
    print Servo.resume()

