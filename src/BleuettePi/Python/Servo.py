#!/usr/bin/python
from Serial import Serial
import serial, types, time, copy
from array import array

class Servo(Serial):

    _0 = 1
    _1 = 2
    _2 = 4
    _3 = 8
    _4 = 16
    _5 = 32
    _6 = 64
    _7 = 128
    _8 = 256
    _9 = 512
    _10 = 1024
    _11 = 2048
    _12 = 4096
    _13 = 8192

    COUNT = 14

    DELAY_BYTE = 0.002

    ACK = 'O'
    NACK = 'N'

    DEBUG = True

    last_status_code = ''

    values = array('c', [chr(200)] * COUNT)
    #values = ['0'] * COUNT
    onboard_values = array('c', [chr(200)] * COUNT)
    #onboard_values = array('h', [200] * COUNT)

    def __init__(self, mixed):
        self.com = mixed

    def getResponse(self):
        return self.com.read(1)

    def getLastStatus(self):
        return (self.last_status_code == self.ACK)

    # Send only one command
    def command(self, cmd):
        self.com.write(self.HEADER)
        self.com.write(cmd)
        self.last_status_code = self.getResponse()
        return self. getLastStatus()

    # Servo related method
    def sendValues(self):
        retry = self.MAX_RETRY

        while True:
            self.com.write(self.HEADER)
            self.com.write(self.CMD_SET)

            control = 0
            for i in range(0, self.COUNT):
                self.com.write(self.values[i])
                control += ord(self.values[i])

            self.com.write(chr(control % 256))

            self.last_status_code = self.getResponse()

            if (self.last_status_code == self.ACK):
                if self.DEBUG:
                    print "Ack Ok !"
                self.onboard_values = copy.copy(self.values)
                break

            retry -= 1
            if retry == 0:
                raise Exception('Unable to communicate !')

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
        if self.command(self.CMD_CLEAR) == self.ACK:
            for i in xrange(0, self.COUNT):
                self.onboard_values[i] = 0

    def setValue(self, index, value):
        self.values[index] = value

    def setValues(self, servos, values):
        v = 0
        for i in xrange(0, self.COUNT):
            if servos & (1 << i):
                self.setValue(i, values[v])
                v += 1

    def dump(self):
        print self.values

