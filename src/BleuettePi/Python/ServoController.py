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

    HEADER = 255

    CMD_INIT      = 'I'
    CMD_PAUSE     = 'P'
    CMD_RESUME    = 'R'
    CMD_CLEAR     = 'C'
    CMD_SET       = 'S'

    DELAY_BYTE = 0.002

    def __init__(self):
        ser = serial.Serial('/dev/ttyAMA0', 9600, timeout = 1)
        ser.open()

    def getResponse():
        return serial.readline()

    # Send only one command
    def command(cmd):
        ser.write(HEADER)
        sleep(DELAY_BYTE)
        ser.write(cmd)
        sleep(DELAY_BYTE)

    def init():
        return command(CMD_INIT)

    # Pause
    def pause():
        return command(CMD_PAUSE)

    # Resume
    def resume():
        return command(CMD_RESUME)

    # Set to 0 all consigne
    def clear():
        return command(CMD_CLEAR)

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

    Servo = ServoController()
    print Servo.init()

