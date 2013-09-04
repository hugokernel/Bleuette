
from Serial import Serial
from Define import BPi_Cmd
import serial, types, time, copy, json
from array import array

class ServoException(Exception):
    pass

class Servo_Trim:

    __values = []

    def __init__(self, trims = []):
        self.__values = [ 0 ] * Servo.COUNT

        if trims:
            for i in range(0, Servo.COUNT):
                self.__values[i] = trims[i]

    def dump(self):
        return self.__values

    def get(self, servo):
        return self.__values[servo]

    def set(self, servo, value):
        self.__values[servo] = value

class Servo_Limit:

    __limits = []

    def __init__(self, limits = []):
        self.__limits = [ [ -1 ] * 2 ] * Servo.COUNT

        if limits:
            for i in range(0, Servo.COUNT):
                self.__limits[i] = limits[i]

    def dump(self):
        return self.__limits

    def get(self, servo):
        return self.__limits[servo]

    def set(self, servo, min, max):
        self.__limits[servo] = [ min, max ]

class Servo(Serial):

    FILE = 'servo.json'

    Trim = None
    Limit = None

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

        data = self.load()

        if 'trims' in data:
            self.Trim = Servo_Trim(data['trims'])

        if 'limits' in data:
            self.Limit = Servo_Limit(data['limits'])

        self.fakemode = fakemode

    def load(self, file = None):
        try:
            data = open((file if file else self.FILE), 'r').read()
            return json.loads(data)
        except IOError:
            pass

    def save(self, file = None):

        data = {
            'trims': self.Trim.dump(),
            'limits': self.Limit.dump()
        }

        with open((file if file else self.FILE), 'w') as jfile:
            jfile.write(json.dumps(data))

    # Servo related method
    def sendValues(self):
        retry = self.MAX_RETRY

        while True:
            self.serial.write(self.HEADER)
            self.serial.write(BPi_Cmd.SET)

            control = 0
            for i in range(0, self.COUNT):
                value = self.values[i] + self.Trim.get(i)
                if value < 0:
                    value = 0

                print "Want to send ", value
                if value > 0:
                    _min, _max = self.Limit.get(i)

                    if _min > -1:
                        value = max(value, _min)

                    if _max > -1:
                        value = min(value, _max)
                print "Send ", value

                self.serial.write(chr(value))
                control += value

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

