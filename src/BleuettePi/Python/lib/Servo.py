
from Serial import Serial
from Define import BPi_Cmd
import serial, types, time, copy, logging
from array import array
from Data import Data
from Singleton import Singleton

SERVO_COUNT = 14

class ServoException(Exception):
    pass

class Servo_Trim:
    values = [ 0 ] * SERVO_COUNT
    def __call__(self, fn):
        def wrapper(*args, **kwargs):
            args = list(args)
            args[2] += Servo_Trim.values[args[1]]
            if args[2] < 0:
                args[2] = 0
            return fn(*args)
        return wrapper

class Servo_Limit:
    values = [ [ - 1 ] * 2 ] * SERVO_COUNT
    def __call__(self, fn):
        def wrapper(*args, **kwargs):
            args = list(args)
            if args[2] > 0:
                _min, _max = Servo_Limit.values[args[1]]
                if _min > -1:
                    args[2] = max(args[2], _min)
                if _max > -1:
                    args[2] = min(args[2], _max)
            return fn(*args)
        return wrapper

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

    COUNT = SERVO_COUNT

    DEBUG = False
    fakemode = False

    last_status_code = ''

    #values = array('c', [chr(0)] * COUNT)
    #sent_values = array('c', [chr(0)] * COUNT)

    values = array('h', [0] * COUNT)
    sent_values = array('h', [0] * COUNT)

    __callback = []

    def __init__(self, serial, fakemode = False):
        self.logger = logging.getLogger('Servo')

        self.serial = serial
        self.fakemode = fakemode

    def setCallback(self, callback):
        self.__callback = callback

    def removeCallback(self):
        self.__callback = None

    @Servo_Limit()
    @Servo_Trim()
    def writeValue(self, index, value):
        self.serial.write(chr(value))
        return value

    # Servo related method
    def sendValues(self):
        retry = self.MAX_RETRY

        while True:
            values = []

            self.serial.write(self.HEADER)
            self.serial.write(BPi_Cmd.SET)

            control = 0
            for i in range(0, self.COUNT):
                #self.serial.write(chr(value))
                value = self.writeValue(i, self.values[i])
                values.append(value)
                control += value

            if self.__callback:
                self.__callback(values)

            self.serial.write(chr(control % 256))

            self.last_status_code = self.getResponse()

            if (self.last_status_code == self.ACK):
                if self.DEBUG:
                    self.logger.debug('Ack OK !')
                self.sent_values = copy.copy(self.values)
                break
            else:
                self.logger.warning("Ack KO !")

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
        self.logger.debug('Buff values: %s' % self.values)
        self.logger.debug('Sent values: %s' % self.sent_values)
        #print 'Buff values:', self.values
        #print 'Sent values:', self.sent_values

