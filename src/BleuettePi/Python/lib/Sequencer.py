
import Sequences, time
from Define import __, PAUSE, FRONT, BACK, DOWN, UP, MID

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

sequences = {
    'standby':  Sequences.Seq_StandBy,
    'middle':   Sequences.Seq_Middle,
    'pushup':   Sequences.Seq_PushUp
}

class Sequencer:

    servo = None

    def __init__(self, servo):
        self.servo = servo

    def getValue(self, servo, pos):
        _min = 0
        _max = 0
        if pos == FRONT or pos == DOWN:
            return servo_limits[servo][0]
        elif pos == BACK or pos == UP:
            return servo_limits[servo][0]
        elif pos == MID:
            _min = min(servo_limits[servo][0], servo_limits[servo][1])
            _max = max(servo_limits[servo][0], servo_limits[servo][1])
            return ((_max - _min) / 2 + _min)
        else:
            _min = min(servo_limits[servo][0], servo_limits[servo][1])
            _max = max(servo_limits[servo][0], servo_limits[servo][1])

            if pos >= _min and pos <= _max:
                return pos

    def forward(self, seq):
        for s in seq.sequence:
            print s
            self.play(s)

    def play(self, seq):
        for i in range(0, len(seq[1])):
            if seq[1][i] != __:
                print self.getValue(i, seq[1][i])
                self.servo.setValue(i, self.getValue(i, seq[1][i]))

        self.servo.sendValues()
        self.servo.dump()
        time.sleep(3)

