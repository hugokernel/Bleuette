
import os, sys
import curses
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
        self.pad.addstr(4, 3, "Odd  : %i %i %i" % (inputs[1], inputs[3], inputs[5]))
        self.pad.refresh(*self.pos)

class Accelerometer(Window):

    def update(self, key = None):
        self.pad.addstr(2, 3, "X : ?, Y : ?, Z : ?")
        self.pad.refresh(*self.pos)

class Compass(Window):

    def update(self, key = None):
        self.pad.addstr(2, 3, "Even : # # #")
        self.pad.refresh(*self.pos)

class Drive(Window):

    def update(self, key = None):
        self.pad.addstr(2, 3, "Press D to start ! %s")
        if key == ord('D'):
            self.run()
        self.pad.refresh(*self.pos)

    i = 0
    def test(self):
        self.i += 1
        self.pod.addstr(1, 2, 'callback ! %i' % self.i)
        self.pod.refresh(0, 0, 20, 20, 20 + self.oheight, 20 + self.owidth)

    pod = None
    oheight = 20
    owidth = 50
    def run(self):
        self.pod = curses.newpad(self.oheight, self.owidth)
        self.pod.border()
        self.pod.addstr(0, 2, 'Drive', curses.A_BOLD)


        self.pod.refresh(0, 0, 20, 20, 20 + self.oheight, 20 + self.owidth)

        key = None
        self.pod.keypad(1)
        self.pod.nodelay(True)
        while key != ord('\n'):
            key = self.pod.getch()

            self.pod.addstr(1, 2, 'Key: %s  ' % str(key))
            self.pod.refresh(0, 0, 20, 20, 20 + self.oheight, 20 + self.owidth)

            if key == ord('q'): break
            elif key == curses.KEY_RIGHT:   B.Drive.update(B.Drive.RIGHT, self.test)
            elif key == curses.KEY_LEFT:    B.Drive.update(B.Drive.LEFT)
            elif key == curses.KEY_UP:      B.Drive.update(B.Drive.FORWARD)
            elif key == curses.KEY_DOWN:    B.Drive.update(B.Drive.REVERSE)

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

def information(value):
    global info
    #info.set('%i' % string)
    info.content = '%i' % value
#    info.pad.addstr(1, 3, string.ljust(info.size[1] - 5))

def main(stdscr):
    global info

    key = None
    while key != ord('\n'):
        h, w = stdscr.getmaxyx()
        mh = int(h / 2)
        mw = int(w / 2)

        infoheight = 3
        groundsheight = 8
        compassheight = 20
        driveheight = 5

        info = Info(mw, infoheight, [ 0, 0, h - infoheight, mw, h, w ], "Information")
        info.update()

        gsens = GroundSensor(mw, groundsheight, [ 0, 0, 0, 0, mh, mw ], "Ground sensor")
        gsens.update()

        accel = Accelerometer(mw, groundsheight, [ 0, 0, 0, mw, mh, w ], "Accelerometer")
        accel.update()

        comp = Compass(w, compassheight, [ 0, 0, groundsheight, 0, h, w ], "Compass")
        comp.update()

        drive = Drive(mw, driveheight, [ 0, 0, compassheight + groundsheight, 0, mh, mw ], "Drive")
        drive.update()

        stdscr.nodelay(True)
        while key != ord('\n'):
            sleep(0.05)
            key = stdscr.getch()
            information(key)
            accel.update(key)
            gsens.update(key)
            comp.update(key)
            drive.update(key)
            info.update(key)

            if key == curses.KEY_RESIZE:
                break

#        if key: debug('key : %i, %c, %s' % (key, input.mode, type(key)))

    stdscr.touchwin()
    stdscr.refresh()
    #stdscr.getch()

curses.wrapper(main)

