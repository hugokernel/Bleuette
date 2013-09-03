
import threading
from Sequences import Sequences

class DriveThreadException(Exception):
    pass

class Drive_Thread(threading.Thread):

    sequencer = None
    sequence = None
    reverse = False

    __pause = True

    index = 0

    def __init__(self, sequencer):
        self.sequencer = sequencer
        threading.Thread.__init__(self)
        self.event = threading.Event()

        #self.sequencer.setCallback(self.testStatus)

    def testStatus(self, index):
        if self.index >= len(self.sequence.sequence):
            self.index = 0

        print "Index:", self.index
        if self.__pause:
            raise DriveThreadException

    def run(self):
        i = 0

        while True:
            while not self.__pause:
                print "Start sequence @", self.index

                try:
                    if self.reverse:
                        self.sequencer.reverse(self.sequence, start_at = self.index, callback = self.testStatus)
                    else:
                        self.sequencer.forward(self.sequence, start_at = self.index, callback = self.testStatus)
                    self.index = 0
                except DriveThreadException:
                    break

                self.event.wait(0.05)

    def play(self, seq, reverse = False):
        self.sequence = seq
        self.reverse = reverse
        self.resume()

    def pause(self):
        self.__pause = True
        self.index += 1

    def resume(self):
        self.__pause = False

class Drive:

    FORWARD = 1
    REVERSE = 2
    RIGHT   = 3
    LEFT    = 4

    sequencer = None

    thread = None

    index = 0
    __last_cmd = None

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

        print "--------------"
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

        if cmd != self.__last_cmd and self.__last_cmd != None:
            self.thread.index = 0
            print "Diff"

        self.thread.play(Commands[cmd][0], Commands[cmd][1])

        self.__last_cmd = cmd

    def end(self):
        print "[End]"
        self.thread.pause()

