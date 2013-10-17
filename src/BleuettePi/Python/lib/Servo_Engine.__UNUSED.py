
from Servo import Servo, Servo_Sequencer, SERVO_COUNT
import threading, logging, time
from Serial import Serial

MAX_SPEED = 1

class Servo_Values:
    current     = 128
    setpoint    = 0
    speed       = 100
    step        = 0
    def __str__(self):
        status = ''
        if self.current == self.setpoint:
            status = '[done]'
        return "current: %i, setpoint: %i, speed: %i, step: %f %s" % (self.current, self.setpoint, self.speed, self.step, status)

class Servo_Engine(threading.Thread):

    __values = [ Servo_Values() for i in range(0, SERVO_COUNT) ]

    __speed = MAX_SPEED
    __callback = None

    def __init__(self, servo):
        self.__servo = servo

        threading.Thread.__init__(self)
        self.event = threading.Event()

        self.logger = logging.getLogger('Servo engine')

    def run(self):

        while True:
            newvalue = False

            for (i, val) in enumerate(self.__values):
                modified = False

                if val.current < val.setpoint:
                    modified = newvalue = True
                    val.current = val.current + val.step
                    if val.current > val.setpoint:
                        val.current = val.setpoint

                if val.current > val.setpoint:
                    modified = newvalue = True
                    val.current = val.current + val.step
                    if val.current < val.setpoint:
                        val.current = val.setpoint

                self.__servo.setValue(i, int(val.current))

                # Test if setpoint is reached ?
                if modified == True and val.current == val.setpoint:
                    self.__callback(i)

                self.logger.debug("Servo %i : %s" % (i, val))

            # Sent if new value
            if newvalue:
                self.logger.debug('Send!')
                self.__servo.sendValues()

            self.logger.debug('Wait !')
            self.event.wait(0.1)

    def setCallback(self, index, callback):
        self.__callback = callback

    def setSpeed(self, speed):
        self.__speed = speed

    def setPosition(self, index, setpoint, speed = -1):
        self.__values[index].setpoint = setpoint

        if speed == -1:
            speed = self.__speed

        self.__values[index].speed = speed
        #self.__values[index].step = speed / float(100) * abs(self.__values[index].current - self.__values[index].setpoint)

        gap = self.__values[index].setpoint - self.__values[index].current
        speed = float(speed) / float(100)
        #self.logger.debug("Servo: %i, Gap: %i, speed: %f, result: %f, count: %f" % (index, gap, speed, speed * gap, setpoint / (speed * gap)))

        if speed * gap:
            self.__values[index].step = setpoint / (speed * gap)
        else:
            self.__values[index].step = setpoint

        self.logger.debug("Servo %i : %s" % (index, self.__values[index]))

if __name__ == '__main__':

    from RainbowHandler import RainbowLoggingHandler
    import sys
    from Fake import SerialFake

    logger = logging.getLogger()
    rainbowHandler = RainbowLoggingHandler(sys.stdout)
    formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
    rainbowHandler.setFormatter(formatter)
    logger.addHandler(rainbowHandler)

    logger.setLevel(logging.DEBUG)

    if True:
        obj = '/dev/ttyAMA0'
        fake = False
    else:
        obj = SerialFake
        fake = True

    serial = Serial()
    serial.connect(obj)

    servo = Servo(serial, fakemode = fake)
    #Servo.init()

    #ServoSeq = Servo_Sequencer(servo)
    #ServoSeq.daemon = True;
    #ServoSeq.start()

    engine = Servo_Engine(servo)
    engine.start()

    SPEED = 1
    CONSIGNE = 170
    def test(index):
        global CONSIGNE, SPEED
        SPEED += 5
        logger.info("Servo %i reached position !" % index)
        if CONSIGNE == 170:
            CONSIGNE = 70
        else:
            CONSIGNE = 170
        logger.info("Speed: %i" % SPEED)
        #time.sleep(0.5)

    engine.setCallback(2, test)

    engine.setSpeed(1)
    engine.setPosition(0, 128)

    time.sleep(5)

    while True:
        engine.setSpeed(SPEED)
        engine.setPosition(0, CONSIGNE)

    #engine.setPosition(0, 1)
    #engine.setPosition(1, 5)
    #engine.setPosition(2, 10)
    #engine.setPosition(3, 20)
    #engine.setPosition(4, 50)
    #engine.setPosition(5, 70)
    #engine.setPosition(6, 90)
    #engine.setPosition(7, 110)
    #engine.setPosition(8, 150)
    #engine.setPosition(9, 180)
    #engine.setPosition(10, 200)
    #engine.setPosition(11, 230)
    #engine.setPosition(12, 240)
    #engine.setPosition(13, 250)

    sys.exit()

    from random import randint
    while True:
        engine.setPosition(randint(0, 5), randint(0, 255), randint(0, 100))
        time.sleep(5)

    #engine.setPosition(0, 128, 100 / 2)
    #engine.setPosition(0, 128, 100 / 10)
    #engine.setPosition(0, 128, 1)

#    with ServoSeq:
#        ServoSeq.setPosition(data['servo'], data['value'])

'''
Servo engine
s = Servo_Engine(servo)
s.setPosition(0, 128)
'''

