
import os, sys
import curses, random
from copy import copy
from time import sleep

sys.path.insert(0, os.path.abspath('lib'))
from Bleuette import Bleuette

info = None

B = Bleuette()

def fillwin(w, c):
    y, x = w.getmaxyx()
    s = c * (x - 1)
    for l in range(y):
        w.addstr(l, 0, s)

def debug(string):
    db = curses.newwin(3, len(string) + 2, 5, 5)
    db.border()
    db.addstr(1, 1, string)
    db.touchwin()
    db.refresh()
    #db.getch()
    sleep(1)

class Window:
    pad = None
    size = []
    pos = []

    def __init__(self, width, height, pos, title):
        self.pos = pos
        self.pad = curses.newpad(height, width)
        self.pad.border()
        self.pad.addstr(0, 2, title, curses.A_BOLD)

class GroundSensor(Window):

    def update(self, key = None):
        global B
        inputs = B.BPi.GroundSensor.get()
        self.pad.addstr(2, 3, "Even : %i %i %i" % (inputs[0], inputs[2], inputs[4]))
        self.pad.addstr(6, 3, "Odd  : %i %i %i" % (inputs[1], inputs[3], inputs[5]))
        self.pad.refresh(*self.pos)

class Compass(Window):

    def update(self, key = None):
        self.pad.addstr(2, 3, "Even : # # #")
        self.pad.refresh(*self.pos)

class Accelerometer(Window):

    def update(self, key = None):
        self.pad.addstr(2, 3, "Even : # # #")
        self.pad.refresh(*self.pos)

class Info(Window):

    content = 'Running'
    count = 0

    def set(self, content):
        self.pad.addstr(1, 3, content)

    def update(self, key = None):
        status = 'OK'

        if self.content == 'Running':
            content = self.content
            for i in range(0, 3):
                content += '.' if self.count > i else ' '

            self.count += 1

            if self.count > 3:
                self.count = 0
        else:
            content = self.content

        self.set(content)
        self.pad.refresh(*self.pos)
        self.pad.refresh(*self.pos)

def information(string):
    global info
    info.pad.addstr(1, 3, string.ljust(info.size[1] - 5))

def main(stdscr):
    global info

    key = None
    while key != ord('\n'):
        h, w = stdscr.getmaxyx()
        mh = int(h / 2)
        mw = int(w / 2)

        info = Info(mw, 3, [ 0, 0, h - 3, mw, h, w ], "Information")
        info.update()

        gsens = GroundSensor(mw, mh, [ 0, 0, 0, 0, mh, mw ], "Ground sensor")
        gsens.update()

        comp = Compass(w, mh - 3, [ 0, 0, mh, 0, h, w ], "Compass")
        comp.update()

        accel = Accelerometer(mw, mh, [ 0, 0, 0, mw, mh, w ], "Accelerometer")
        accel.update()

        stdscr.nodelay(True)
        while key != ord('\n'):
            sleep(0.5)
            key = stdscr.getch()

            accel.update(key)
            gsens.update(key)
            comp.update(key)
            info.update(key)

            if key == curses.KEY_RESIZE:
                break

#        if key: debug('key : %i, %c, %s' % (key, input.mode, type(key)))

    stdscr.touchwin()
    stdscr.refresh()
    #stdscr.getch()

curses.wrapper(main)

