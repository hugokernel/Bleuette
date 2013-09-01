
from Sequences import Sequences

class Drive():

    FORWARD = 1
    REVERSE = 2
    RIGHT   = 3
    LEFT    = 4

    sequencer = None

    def __init__(self, sequencer):
        self.sequencer = sequencer

    def update(self, cmd, callback):

        CMD = {
            self.FORWARD:   Sequences['walk'],
            self.REVERSE:   Sequences['walk'],
            self.RIGHT:     Sequences['pushup'],
            self.LEFT:      Sequences['pushup']
        }

        if callback:
            self.sequencer.setCallback(callback)

        self.sequencer.Buffer.push(CMD[cmd], 100)
        self.sequencer.Thread.start()

