import sys, curses, os, time, copy, logging
sys.path.insert(0, os.path.abspath('lib'))

from Servo_SpeedStep import Servo_SpeedStep
from Servo_SpeedDelay import Servo_SpeedDelay
from Servo import Servo, Servo_Sequencer, SERVO_COUNT
from RainbowHandler import RainbowLoggingHandler
from Fake import SerialFake
from Serial import Serial
import config as Config

Log = None

class cursesHandler(logging.StreamHandler):
    def emit(self, record):
        global Log
        try:
            msg = self.format(record)
            if Log:
                Log.addLine(record)

                #Log.pad.addstr(self.pos, 2, msg, curses.A_BOLD)
                #self.pos += 1
                #time.sleep(0.5)

                #Log.pos[0] += 1
                #Log.pad.refresh(*Log.pos)#, 2, msg, curses.A_BOLD)
        except (KeyboardInterrupt, SystemExit):
            raise
        except:
            self.handleError(record)

logger = logging.getLogger()
#rainbowHandler = RainbowLoggingHandler(sys.stdout)
#formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
#rainbowHandler.setFormatter(formatter)
#logger.addHandler(rainbowHandler)
curseHandler = cursesHandler()
logger.addHandler(curseHandler)

logger.setLevel(logging.DEBUG)

class Setting:
    servo = 0
    setpoint = 128
    speed = 1

setting = Setting()

class Window:
    pad = None
    size = []
    pos = []

    def refresh(self):
        self.pad.refresh(*self.pos)

    def clear(self):
        y, x = self.pad.getmaxyx()
        s = ' ' * (x - 2)
        for l in range(1, y - 1):
            self.pad.addstr(l, 1, s)

class Control(Window):

    setting = None

    def __init__(self, height, width, pos):
        self.pos = pos
        self.size = [ height, width ]
        self.pad = curses.newpad(height, width)
        self.pad.border()
        self.pad.addstr(0, 2, "Control", curses.A_BOLD)
        self.pad.addstr(9, 2, "Left/Right : Select servo, Up/Down : Value, Page Up/Down : Speed, a: Toggle auto mode", curses.A_BOLD)

    def setSetting(setting):
        self.setting = setting

    def update(self, key = None):
        global logger, AUTO

        if key == curses.KEY_UP:
            setting.setpoint += 5
        elif key == curses.KEY_DOWN:
            setting.setpoint -= 5
        elif key == curses.KEY_RIGHT:
            setting.servo += 1
        elif key == curses.KEY_LEFT:
            setting.servo -= 1
        elif key == ord('\n'):
            #sspeed.setGlobalSpeed(setting.speed)
            sspeed.setSpeed(setting.servo, setting.speed)
            sspeed.setPosition(setting.servo, setting.setpoint)
        elif key == curses.KEY_NPAGE:
            setting.speed -= 10
            if setting.speed < 0:
                setting.speed = 0
        elif key == curses.KEY_PPAGE:
            setting.speed += 10
        elif key == ord('a'):
            if AUTO:
                logger.info("Mode auto : Off")
                [ sspeed.setPosition(i, 128) for i in range(0, 14) ]
                #[ sspeed.setPosition(i, sspeed.getPosition(i)) for i in range(0, 14) ]
                AUTO = False
            else:
                logger.info("Mode auto : On")
                [ sspeed.setPosition(i, 80) for i in range(0, 14) ]
                AUTO = True

        if setting.setpoint > 255:
            setting.setpoint = 0
        if setting.setpoint < 0:
            setting.setpoint = 255

        if setting.servo > 13:
            setting.servo = 0
        if setting.servo < 0:
            setting.servo = 13

        #values = sspeed.getCurrentValue()
        values = sspeed.getValues()

        self.pad.addstr(2, 3, 'Servo  : %i, Value: %i, Speed: %i         ' % (setting.servo, setting.setpoint, setting.speed))
        self.pad.addstr(4, 3, 'Servos : ' + ''.join([ ' %0.2i ' % i for i in range(0, len(values)) ]))
        self.pad.addstr(5, 3, 'Goal   : ' + ''.join([ ' %0.3i' % values[i].setpoint for i in range(0, len(values)) ]))
        self.pad.addstr(6, 3, 'Current: ' + ''.join([ ' %0.3i' % values[i].current for i in range(0, len(values)) ]))
        self.pad.addstr(7, 3, 'Speed  : ' + ''.join([ ' %0.3i' % values[i].speed for i in range(0, len(values)) ]))
        self.pad.refresh(*self.pos)

class LogPad(Window):

    buffer = []

    def __init__(self, height, width, pos):
        self.pos = pos
        self.size = [ height, width ]
        self.pad = curses.newpad(height, width)
        self.pad.border()
        self.pad.addstr(0, 2, "Logging", curses.A_BOLD)

    def update(self, key = None):
        self.pad.refresh(*self.pos)

    def draw(self):
        self.clear()
        y = 1
        for record in self.buffer:
            msg = "%s\t[%s]\t%s\t%s" % (record.relativeCreated, record.levelname, record.name, record.message)

            #msg += ' '.join([ '+' for i in range(0, self.size[1] - len(msg))])

            options = 0
            if record.levelno > logging.DEBUG:
                options = curses.A_BOLD

            self.pad.addstr(y, 2, msg, options)
            y += 1
            if y > self.size[0] - 2:
                break

    def addLine(self, record):
        if len(self.buffer) == self.size[0] - 2:
            self.buffer.pop(0)
        self.buffer.append(record)
        self.draw()

def main(stdscr):
    global control, Log

    controlSize = 12

    key = None
    #while key != ord('\n'):
    h, w = stdscr.getmaxyx()

    control = Control(controlSize, w, [ 0, 0, h - controlSize, 0, h, w ])
    control.update()

    Log = LogPad(h - controlSize, w, [ 0, 0, 0, 0, h, w ])

    stdscr.nodelay(True)
    while key != 27: #ord('\n'):
        key = stdscr.getch()

        control.update(key)
        Log.update()

        if key == curses.KEY_RESIZE:
            break

    sspeed.stop()

    stdscr.touchwin()
    stdscr.refresh()
    #stdscr.getch()

if Config.FAKE_MODE:
    obj = SerialFake
else:
    obj = '/dev/ttyAMA0'

serial = Serial()
serial.connect(obj)

servo = Servo(serial, fakemode = Config.FAKE_MODE)
servo.init()

#ServoSeq = Servo_Sequencer(servo)
#ServoSeq.daemon = True;
#ServoSeq.start()

#sspeed = Servo_SpeedStep(servo)
sspeed = Servo_SpeedDelay(servo)
sspeed.start()

[ sspeed.setSpeed(i, i * 7) for i in range(0, 14) ]

#values = sspeed.getCurrentValue()
#print values
#sys.exit()

AUTO = False

def test(index):
    global sspeed
    logger.info("Servo %i reached position (auto mode: %i)" % (index, AUTO))
    if AUTO:
        if sspeed.getPosition(index) == 160:
            sspeed.setPosition(index, 80)
        else:
            sspeed.setPosition(index, 160)

sspeed.setCallback(2, test)
sspeed.setGlobalSpeed(1)

[ sspeed.setPosition(i, 128) for i in range(0, 14) ]

'''
sspeed.setPosition(0, 1)
sspeed.setPosition(1, 5)
sspeed.setPosition(2, 10)
sspeed.setPosition(3, 20)
sspeed.setPosition(4, 50)
sspeed.setPosition(5, 70)
sspeed.setPosition(6, 90)
sspeed.setPosition(7, 110)
sspeed.setPosition(8, 150)
sspeed.setPosition(9, 180)
sspeed.setPosition(10, 200)
sspeed.setPosition(11, 230)
sspeed.setPosition(12, 240)
sspeed.setPosition(13, 250)
'''

curses.wrapper(main)

'''

sys.exit()

from random import randint
while True:
    sspeed.setPosition(randint(0, 5), randint(0, 255), randint(0, 100))
    time.sleep(5)

#sspeed.setPosition(0, 128, 100 / 2)
#sspeed.setPosition(0, 128, 100 / 10)
#sspeed.setPosition(0, 128, 1)

#    with ServoSeq:
#        ServoSeq.setPosition(data['servo'], data['value'])
'''

'''
Servo sspeed
s = Servo_SpeedStep(servo)
s.setPosition(0, 128)
'''
