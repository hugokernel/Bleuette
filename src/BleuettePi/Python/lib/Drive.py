
import threading
from Sequences import Sequences
from Sequencer import SequencerExceptionStop

class Drive_Thread(threading.Thread):

    sequencer = None
    sequence = None
    reverse = False

    __pause = True

    def __init__(self, sequencer):
        self.sequencer = sequencer
        threading.Thread.__init__(self)
        self.event = threading.Event()

        self.sequencer.setCallback(self.testStatus)

    def testStatus(self):
        if self.__pause:
            raise SequencerExceptionStop

    def run(self):
        i = 0

        while True:
            while not self.__pause:
                print "Start sequence"

                try:
                    if self.reverse:
                        self.sequencer.reverse(self.sequence)
                    else:
                        self.sequencer.forward(self.sequence)
                except SequencerExceptionStop:
                    print "Catched !"

                self.event.wait(0.05)

        print "Thread stopped !"

    def play(self, seq, reverse = False):
        self.sequence = seq
        self.reverse = reverse
        self.resume()

    def pause(self):
        self.__pause = True

    def resume(self):
        self.__pause = False

class Drive:

    FORWARD = 1
    REVERSE = 2
    RIGHT   = 3
    LEFT    = 4

    sequencer = None

    thread = None

    def __init__(self, sequencer):
        self.sequencer = sequencer

        self.thread = Drive_Thread(sequencer)
        self.thread.start()

    def forward(self):
        self.begin(self.FORWARD)

    def reverse(self):
        self.begin(self.REVERSE)

    def right(self):
        self.begin(self.RIGHT)

    def left(self):
        self.begin(self.LEFT)

    def begin(self, cmd, callback = None):

        print "[Begin]"

        Commands = {
            # id : [ seq, reverse ]
            self.FORWARD:   [ Sequences['walk'], False ],
            self.REVERSE:   [ Sequences['walk'], True ],
            self.RIGHT:     [ Sequences['rightturn'], True ],
            self.LEFT:      [ Sequences['leftturn'], True ]
        }

        if callback:
            self.sequencer.setCallback(callback)

        self.thread.play(Commands[cmd][0], Commands[cmd][1])

    def end(self):
        print "[End]"
        self.thread.pause()

