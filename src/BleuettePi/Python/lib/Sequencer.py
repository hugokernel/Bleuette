
import threading, time
import Sequences
import Define
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

    def __init__(self, sequencer):
        self.sequencer = sequencer
        threading.Thread.__init__(self)
        self._stopevent = threading.Event()

    def run(self):
        i = 0
        #while not self._stopevent.isSet():

        for line in self.sequencer.Buffer.buffer:
            for s in line.sequence:
                print "Play !"
                if self._stopevent.isSet():
                    print "Stop event !"
                    break
                self.sequencer.play(s)

        #   self._stopevent.wait(0.5)
        print "le thread s'est termine proprement"

    def stop(self):
        self._stopevent.set()

class Sequencer_Buffer:

    sequencer = None

    buffer = []

    def __init__(self, sequencer):
        self.sequencer = sequencer

    def push(self, seq, count = 1):
        for i in range(0, count):
            self.buffer.append(seq)

    def forward(self, count = 1):
        for i in range(0, count):
            for line in self.buffer:
                for s in line.sequence:
                    self.sequencer.play(s)

class Sequencer:

    servo = None
    Buffer = None
    Thread = None

    __callback = None

    DEBUG = True

    def __init__(self, servo):
        self.servo = servo
        self.Buffer = Sequencer_Buffer(self)
        self.Thread = Sequencer_Thread(self)

    def setCallback(self, callback):
        self.__callback = callback

    def getValue(self, servo, pos):
        _min = 0
        _max = 0
        if pos == Define.FRONT or pos == Define.DOWN:
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

    def forward(self, seq, count = 1):
        for i in range(0, count):
            for s in seq.sequence:
                print s
                self.play(s)

    def play(self, seq):
        for i in range(0, len(seq[1])):
            if seq[1][i] != Define.__:
                self.servo.setValue(i, self.getValue(i, seq[1][i]))

        self.servo.sendValues()

        if seq[2]:
            seq[2]()
       
        if self.DEBUG:
            self.servo.dump()
            print '--'

        if seq[0] == None:
            time.sleep(Define.DELAY_MIN)
        else:
            time.sleep(seq[0])

