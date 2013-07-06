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


from Sequencer import sequences
B.Sequencer.forward(sequences['middle'])
B.Sequencer.forward(sequences['pushup'])

time.sleep(2)

B.Sequencer.forward(sequences['middle'])


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

