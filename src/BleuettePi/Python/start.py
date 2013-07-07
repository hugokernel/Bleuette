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
B.Sequencer.forward(Sequences['middle'])
B.BPi.Servo.pause()

#B.BPi.Accelerometer.realTime()
B.BPi.Compass.realTime()

sys.exit()
#B.Sequencer.forward(sequences['pushup'], 4)
#B.Sequencer.forward(Sequences['walk'], 4)

B.Sequencer.Buffer.push(Sequences['walk'])
B.Sequencer.Buffer.push(Sequences['walk'])
B.Sequencer.Buffer.push(Sequences['walk'])

#B.Sequencer.Buffer.forward()
B.Sequencer.Thread.start()

for i in range(0, 20):
    time.sleep(0.5)
    if i == 1:
        print 'Stop !'
        B.Sequencer.Thread.stop()

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

