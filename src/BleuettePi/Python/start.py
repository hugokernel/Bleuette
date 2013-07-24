#!/usr/bin/env python

import os
import sys
import time, time

sys.path.insert(0, os.path.abspath('lib'))

from Bleuette import Bleuette

B = Bleuette()

'''
value = chr(99)
B.BPi.Servo.setValue(11, value)
B.BPi.Servo.sendValues()
'''

from Sequences import Sequences

B.BPi.Servo.resume()
B.Sequencer.forward(Sequences['middle'])
#B.BPi.Servo.pause()
#sys.exit()

#sys.exit(1)

'''
#B.BPi.Accelerometer.realTime()
#B.BPi.Compass.realTime()
B.BPi.GroundSensor.realTime()
sys.exit()
'''

'''
while True:
    val = B.BPi.Analog.read(B.BPi.Analog.AN6)

    val = B.BPi.Analog.convertInVolt(val)

    val0 = (val / (10.0 / (14.76 + 10.0)))

    print val, val0, val0 * (12 / 11.1)

    #4.8 / (10 / 15 + 10)

    #print val
    time.sleep(1)
'''

B.Sequencer.forward(Sequences['pushup'], 3)
B.BPi.Servo.pause()
#B.Sequencer.forward(Sequences['walk'], 2)
sys.exit()

B.Sequencer.Buffer.push(Sequences['walk'], 20)

#B.Sequencer.Buffer.forward()
B.Sequencer.Thread.start()

for i in range(0, 20):
    time.sleep(0.5)
    if i == 1:
        print 'Stop !'
        B.Sequencer.Thread.stop()

B.Sequencer.forward(Sequences['standby'])
B.BPi.Servo.pause()
sys.exit()

#time.sleep(2)
#B.Sequencer.forward(sequences['middle'])


#B.BPi.setCurrentAlarmLevel(1000)

#print 'End'
#while True:
#    print B.BPi.Analog.getCurrent()
#    time.sleep(2)

'''
print BPi.getStatus()
BPi.Servo.init()

val = BPi.Analog.readCurrent()
val = BPi.Analog.read(Analog.AN0)
'''

