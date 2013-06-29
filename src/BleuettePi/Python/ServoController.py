#!/usr/bin/python

import serial
from time import sleep

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

