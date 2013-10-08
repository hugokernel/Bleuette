
from Servo import Servo, Servo_Sequencer, SERVO_COUNT
import threading, logging, time
from Serial import Serial

MAX_SPEED = 1

class Servo_Values:
    current     = 128
    setpoint    = 0
    speed       = 1
    def __str__(self):
        status = ''
        if self.current == self.setpoint:
            status = '[done]'
        return "current: %i, setpoint: %i, speed: %i %s" % (self.current, self.setpoint, self.speed, status)

class Servo_SpeedDelay(threading.Thread):

    __values = [ Servo_Values() for i in range(0, SERVO_COUNT) ]

    __speed = MAX_SPEED
    __callback = None

    __stop = False

    def __init__(self, servo):
        self.__servo = servo

        threading.Thread.__init__(self)
        self.event = threading.Event()

        self.logger = logging.getLogger('Servo SpeedDelay')

    def run(self):

        counter = 0
        while self.__stop == False:

            newvalue = False

            for (i, val) in enumerate(self.__values):
                modified = False

                #if val.current != val.setpoint and val.speed == 0:
                if val.speed == 0:
                #    #self.logger.info('Current: %i, Setpoint: %i' % (val.current, val.setpoint))
                    if val.current != val.setpoint:
                        modified = newvalue = True

                    val.current = val.setpoint
                #    #self.logger.info('Current: %i, Setpoint: %i' % (val.current, val.setpoint))
                #    self.event.wait(1)

                if val.setpoint and val.speed and (counter % val.speed) == 0:
                    if val.current < val.setpoint:
                        modified = newvalue = True
                        val.current += 1

                    if val.current > val.setpoint:
                        modified = newvalue = True
                        val.current -= 1

                    #self.logger.info("PAF %i" % i)
                    #self.logger.debug("Servo %i : %s" % (i, val))
                   # self.logger.info(''.join([ ' %0.3i' % self.__values[i].current for i in range(0, len(self.__values))]))
                    #self.event.wait(1)

                self.__servo.setValue(i, int(val.current))

                # Test if setpoint is reached ?
                if modified == True and val.current == val.setpoint and self.__callback:
                    self.__callback(i)

                #self.logger.debug("Servo %i : %s" % (i, val))

            # Sent if new value
            if newvalue:
                #self.logger.info(''.join([ ' %0.3i' % self.__values[i].current for i in range(0, len(self.__values))]))
                self.__servo.sendValues()
                #self.event.wait(0.3)
                #self.event.wait(1)

            #self.logger.debug('Wait !')
            #self.event.wait(0.0001)

            counter += 1

    def setCallback(self, index, callback):
        self.__callback = callback

    def setGlobalSpeed(self, speed):
        self.__speed = speed

    def setSpeed(self, index, speed):
        self.__values[index].speed = speed
        self.logger.debug("Set speed for servo %i : %i" % (index, speed))

    def getCurrentValue(self):
        return [ self.__values[i].current for i in range(0, SERVO_COUNT) ]

    def getValues(self):
        return self.__values

    def stop(self):
        self.__stop = True

    def getPosition(self, index):
        return self.__values[index].current

    def setPosition(self, index, setpoint, speed = -1):
        self.__values[index].setpoint = setpoint

        if speed != -1:
            self.__values[index].speed = speed

        self.logger.debug("Servo %i : %s" % (index, self.__values[index]))

'''
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

    serial = Serial()
    serial.connect(SerialFake)

    servo = Servo(serial, fakemode = True)
    #Servo.init()

    #ServoSeq = Servo_Sequencer(servo)
    #ServoSeq.daemon = True;
    #ServoSeq.start()

    engine = Servo_SpeedDelay(servo)
    engine.start()

    def test(index):
        logger.info("Servo %i reached position !" % index)
        #time.sleep(0.5)

    engine.setCallback(2, test)

    engine.setSpeed(5)
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
    engine.setPosition(13, 250)

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
