
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

class Servo_SpeedStep(threading.Thread):

    __values = [ Servo_Values() for i in range(0, SERVO_COUNT) ]

    __speed = MAX_SPEED
    __callback = None

    __stop = False

    def __init__(self, servo):
        self.__servo = servo

        threading.Thread.__init__(self)
        self.event = threading.Event()

        self.logger = logging.getLogger('Servo SpeedStep')

    def run(self):

        while self.__stop == False:
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

    def getCurrentValue(self):
        return [ self.__values[i].current for i in range(0, SERVO_COUNT) ]

    def stop(self):
        self.__stop = True

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

