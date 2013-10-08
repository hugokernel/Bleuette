
import threading, time
import Sequences
import Define
import Servo

#from Define import __, PAUSE, FRONT, BACK, DOWN, UP, MID, DELAY_MIN

HMIN    = 85
HMAX    = 170
HMID    = HMIN + (HMAX - HMIN) / 2

VMIN    = 90
VMAX    = 210
VMID    = VMIN + (VMAX - VMIN) / 2

servo_limits = [

    # Horizontal servos
    [ HMIN, HMAX ],
    [ HMAX, HMIN ],
    [ HMIN, HMAX ],
    [ HMAX, HMIN ],
    [ HMIN, HMAX ],
    [ HMAX, HMIN ],

    # Vertical servos
    [ VMIN, VMAX ],
    [ VMAX, VMIN ],
    [ VMIN, VMAX ],
    [ VMAX, VMIN ],
    [ VMAX, VMIN ],
    [ VMIN, VMAX ]
]

class Sequencer_Thread(threading.Thread):

    sequencer = None
    ok = True

    def __init__(self, sequencer):
        self.sequencer = sequencer
        threading.Thread.__init__(self)
        self._stopevent = threading.Event()

    #def run(self, callback):
    def run(self):
        i = 0

        while True:
            while not self._stopevent.isSet():
                for line in self.sequencer.Buffer.buffer:
                    for s in line.sequence.sequence:
                        print "Play !"
                        if self._stopevent.isSet():
                            print "Stop event !"
                            break

                        self.sequencer.play(s)

            #if callback:
            #    callback()

                self._stopevent.wait(0.5)

        print "End of Thread"

    def stop(self):
        self._stopevent.set()

class Sequencer_Line:
    sequence = []
    callback = None
    reverse = False

class Sequencer_Buffer:

    sequencer = None

    buffer = []

    def __init__(self, sequencer):
        self.sequencer = sequencer

    def push(self, seq, count = 1, options = {}):

        sline = Sequencer_Line
        sline.sequence = seq

        if options:
            if 'callback' in options:
                sline.callback = options['callback'] 
            if 'reverse' in options:
                sline.reverse = options['reverse']

        for i in range(0, count):
            self.buffer.append(sline)

    def clear(self):
        self.buffer = []

    def play(self, count = 1):
        for i in range(0, count):
            for line in self.buffer.sequence:
                for s in line.sequence:
                    if s.reverse:
                        print "Reverse"
                        self.sequencer.reverse(s)
                    else:
                        print "Forward"
                        self.sequencer.forward(s)

'''
    def forward(self, count = 1):
        for i in range(0, count):
            for line in self.buffer:
                for s in line.sequence:
                    self.sequencer.play(s)
'''

class Sequencer:

    Servo = None
    ServoSeq = None

    Buffer = None
    Thread = None

    __callback = []

    DEBUG = True

    def __init__(self, servo):
        self.Servo = servo
        self.Buffer = Sequencer_Buffer(self)
        self.Thread = Sequencer_Thread(self)

        self.ServoSeq = Servo.Servo_Sequencer(self.Servo)
        self.ServoSeq.daemon = True;
        self.ServoSeq.start()

#    def test(self):
#        self.Thread = Sequencer_Thread(self)

#    def setTrim(self, servo, value, update = True):
#        self.Servo.trim.set(servo, value, update)

    def addCallback(self, callback):
        self.__callback.append(callback)

    def getValue(self, servo, pos):
        _min = 0
        _max = 0
        if pos == 0:
            return 0
        elif pos == Define.FRONT or pos == Define.DOWN:
            return servo_limits[servo][0]
        elif pos == Define.BACK or pos == Define.UP:
            return servo_limits[servo][1]
        elif pos == Define.MID:
            _min = min(servo_limits[servo][0], servo_limits[servo][1])
            _max = max(servo_limits[servo][0], servo_limits[servo][1])
            return ((_max - _min) / 2 + _min)
        else:
            _min = min(servo_limits[servo][0], servo_limits[servo][1])
            _max = max(servo_limits[servo][0], servo_limits[servo][1])

            if pos >= _min and pos <= _max:
                return pos

    def forward(self, seq, count = 1, start_at = 0, callback = None):
        start_at = min(start_at, len(seq.sequence) - 1)

        for i in range(0, count):
            #for s in seq.sequence:
            for p in range(start_at, len(seq.sequence)):

                #self.play(s)
                self.play(seq.sequence[p])

                # After callback
                if callback:
                    callback(p)

                if self.__callback:
                    for c in self.__callback:
                        c(p, seq.sequence)

    def reverse(self, seq, count = 1, start_at = 0, callback = None):
        for i in range(0, count):
            for s in range(len(seq.sequence) - 1, -1, -1):

                self.play(seq.sequence[s])

                # After callback
                if callback:
                    callback(s)

                if self.__callback:
                    for c in self.__callback:
                        c(s, seq.sequence)

    def play(self, seq):

        with self.ServoSeq as servoSeq:
            for i in range(0, len(seq[1])):
                if seq[1][i] != Define.__:
                    servoSeq.setPosition(i, self.getValue(i, seq[1][i]))

        #self.Servo.setValue(i, self.getValue(i, seq[1][i]))
        #self.Servo.sendValues()

        if seq[2]:
            seq[2]()

        if self.DEBUG:
            self.Servo.dump()

        if seq[0] == None:
            time.sleep(Define.DELAY_MIN)
        else:
            time.sleep(seq[0])

