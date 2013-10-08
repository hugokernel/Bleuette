
import serial, types, time, copy, logging, threading
from Serial import Serial
from Define import BPi_Cmd
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
            if args[2] > 0:
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

# Run in background and send target value when possible
class Servo_Sequencer(threading.Thread):

    BUFFER_SIZE = 100

    event = None

    __servo = None

    __values = []
    __buffer = []

    __contextManagerMode = False

    def __init__(self, servo):
        self.__servo = servo

        threading.Thread.__init__(self)
        self.event = threading.Event()

        self.logger = logging.getLogger('Servo sequencer')

    def run(self):
        i = 0

        while True: #not self.__pause:

            if len(self.__buffer):
                v = self.__buffer.pop()
                if v:
                    if len(v) == 2:
                        #self.logger.debug('Standard value')
                        self.__servo.setValue(v[0], v[1])
                        self.__servo.sendValues()
                    else:
                        #self.logger.debug('Transaction')
                        for i in range(0, len(v)):
                            self.__servo.setValue(i, v[i])
                        self.__servo.sendValues()

            self.event.wait(0.005)

    def __enter__(self):
        self.__contextManagerMode = True
        self.__values = self.__servo.getValues()
        return self

    def __exit__(self, exception_type, exception_value, traceback):
        self.__contextManagerMode = False
        self.__buffer.append(self.__values);

    def setPosition(self, index, value):
        if len(self.__buffer) <= self.BUFFER_SIZE:
            if self.__contextManagerMode:
                self.__values[index] = value
            else:
                self.__buffer.append([ index, value ]);
        else:
            self.logger.error('Buffer full : Skip value !')

    #def stop(self):
    #    self._stopevent.set()

'''
# Use :
Servo = Servo.Servo(Serial)
Servo.init()

ServoSeq = Servo_Sequencer(Servo)
ServoSeq.daemon = True;
ServoSeq.start()

with ServoSeq:
    ServoSeq.setPosition(data['servo'], data['value'])
'''

LOCK = False

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

    __values = array('h', [0] * COUNT)
    __sent_values = array('h', [0] * COUNT)

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
        global LOCK

        retry = self.MAX_RETRY

        if LOCK:
            raise ServoException('Lock error !')

        LOCK = True

        while True:
            values = []

            self.serial.write(self.HEADER)
            self.serial.write(BPi_Cmd.SET)

            control = 0
            for i in range(0, self.COUNT):
                value = self.writeValue(i, self.__values[i])
                values.append(value)
                control += value

            if self.__callback:
                self.__callback(values)

            self.serial.write(chr(control % 256))

            self.last_status_code = self.getResponse()

            if self.last_status_code == self.ACK or self.fakemode:
                if self.DEBUG:
                    self.logger.debug('Ack OK !')
                self.__sent_values = copy.copy(self.__values)
                break
            else:
                self.logger.warning("Ack KO !")

            retry -= 1
            if retry == 0:
                if self.fakemode:
                    break
                else:
                    raise ServoException('Unable to communicate !')

        LOCK = False

        return self.getLastStatus()

    def init(self):
        return self.serial.command(BPi_Cmd.INIT)

    # Pause
    def pause(self):
        return self.serial.command(BPi_Cmd.PAUSE)

    # Resume
    def resume(self):
        return self.serial.command(BPi_Cmd.RESUME)

    # Set to 0 all consigne
    def clear(self):
        if self.command(BPi_Cmd.CLEAR) == self.ACK:
            for i in xrange(0, self.COUNT):
                self.__sent_values[i] = 0

    def setValue(self, index, value):
        #if type(value) == types.IntType:
        #    value = chr(value)
        self.__values[index] = value

    def setValues(self, servos, values):
        v = 0
        for i in xrange(0, self.COUNT):
            if servos & (1 << i):
                self.setValue(i, values[v])
                v += 1

    def getValues(self):
        return self.__sent_values

    def dump(self, allval = False):
        self.logger.debug('Sent values: %s' % self.__sent_values)
        if allval:
            self.logger.debug('Buff values: %s' % self.__values)

