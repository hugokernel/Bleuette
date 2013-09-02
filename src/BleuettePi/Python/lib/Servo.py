
from Serial import Serial
from Define import BPi_Cmd
import serial, types, time, copy
from array import array

class ServoException(Exception):
    pass

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

    DEBUG = False
    fakemode = False

    last_status_code = ''

    #values = array('c', [chr(0)] * COUNT)
    #sent_values = array('c', [chr(0)] * COUNT)

    values = array('h', [0] * COUNT)
    sent_values = array('h', [0] * COUNT)

    def __init__(self, serial, fakemode = False):
        self.serial = serial

        self.fakemode = fakemode

    # Servo related method
    def sendValues(self):
        retry = self.MAX_RETRY

        while True:
            self.serial.write(self.HEADER)
            self.serial.write(BPi_Cmd.SET)

            control = 0
            for i in range(0, self.COUNT):
                self.serial.write(chr(self.values[i]))
                control += self.values[i]

            self.serial.write(chr(control % 256))

            self.last_status_code = self.getResponse()

            if (self.last_status_code == self.ACK):
                if self.DEBUG:
                    print "Ack Ok !"
                self.sent_values = copy.copy(self.values)
                break

            retry -= 1
            if retry == 0:
                if self.fakemode:
                    break
                else:
                    raise ServoException('Unable to communicate !')

        return self.getLastStatus()

    def init(self):
        return self.command(BPi_Cmd.INIT)

    # Pause
    def pause(self):
        return self.command(BPi_Cmd.PAUSE)

    # Resume
    def resume(self):
        return self.command(BPi_Cmd.RESUME)

    # Set to 0 all consigne
    def clear(self):
        if self.command(BPi_Cmd.CLEAR) == self.ACK:
            for i in xrange(0, self.COUNT):
                self.sent_values[i] = 0

    def setValue(self, index, value):
        #if type(value) == types.IntType:
        #    value = chr(value)
        self.values[index] = value

    def setValues(self, servos, values):
        v = 0
        for i in xrange(0, self.COUNT):
            if servos & (1 << i):
                self.setValue(i, values[v])
                v += 1

    def dump(self):
        print 'Buff values:', self.values
        print 'Sent values:', self.sent_values

