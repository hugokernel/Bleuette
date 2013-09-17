#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json
import threading
from tornado import websocket, web, ioloop, template
from operator import eq

import os, sys, time, copy, logging
sys.path.insert(0, os.path.abspath('lib'))

from Bleuette import Bleuette
from Data import Data
from Sequences import Sequences
import Drive
import config as Config
from RainbowHandler import RainbowLoggingHandler

cl = []


logger = logging.getLogger()

def getLevel(st):
    if st == 'debug':
        return logging.DEBUG
    elif st == 'info':
        return logging.INFO
    elif st == 'warning':
        return logging.WARNING
    elif st == 'error':
        return logging.ERROR
    elif st == 'critical':
        return logging.CRITICAL

rainbowHandler = RainbowLoggingHandler(sys.stdout)
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
rainbowHandler.setFormatter(formatter)
logger.addHandler(rainbowHandler)
#logger.setLevel(getLevel(Data.Instance().get(['log', 'default'])))


class JsHandler(logging.Handler):
    def __init__(self):
        logging.Handler.__init__(self)
        self.propagate = True

    def emit(self, record):

        data = {
            'type': 'log',
            'data': {
                'name':         record.name,
                'levelno':      record.levelno,
                'levelname':    record.levelname,
                'pathname':     record.pathname,
                'lineno':       record.lineno,
                'args':         record.args,
                'msg':          record.msg
            }
        }

        for c in cl:
            c.write_message(json.dumps(data))

jsHandler = JsHandler()
jsHandler.setLevel(logging.DEBUG)
logger.addHandler(jsHandler)
logger.setLevel(logging.DEBUG)
#ogger.setLevel(getLevel(Data.Instance().get(['log', 'js'])))



B = Bleuette()

class Affiche(threading.Thread):

    data = {}
    data_last = {}

    def __init__(self, nom = ''):
        threading.Thread.__init__(self)
        self.nom = nom
        self._stopevent = threading.Event( )

    def run(self):
        while not self._stopevent.isSet():

            compass = 0

            acc = B.BPi.Accelerometer.get()
            ground = B.BPi.GroundSensor.get()
            compass = B.BPi.Compass.get()
            #current = B.BPi.Analog.getCurrent()
            current = 0

            self.data = {
                'type': 'sensors',
                'sensors': {
                    'accelerometer': acc,
                    'compass': compass,
                    'ground': ground,
                    'current': current
                }
            }

            #print self.data

            #if any(map(eq, self.data, self.data_last))== False:
            self.update()

            self._stopevent.wait(0.5)

        print "End of thread !"

    def update(self):
        for c in cl:
            c.write_message(json.dumps(self.data))

        self.data_last = copy.copy(self.data)

    def stop(self):
        self._stopevent.set( )

a = Affiche('Thread A')
a.daemon = True;
a.start();

speed = 16
import Define
def delay(index, sequence):
    global speed
    time.sleep(Define.DELAY_MIN * (16 - speed))
B.Sequencer.addCallback(delay)

class IndexHandler(web.RequestHandler):
    def get(self):

        var = dict(
            Sequences = Sequences,
            Config = Config
        )

        self.render("www/index.html", **var)

class SocketHandler(websocket.WebSocketHandler):

    last_message = None

    def livemode(self, values):
        data = {
            'type': 'position',
            'data': {
                'servos': values
            }
        }
        self.write(json.dumps(data))

    def open(self):
        if self not in cl:
            cl.append(self)

    def write(self, data):
        for c in cl:
            c.write_message(data)

    def on_message(self, message):
        global logger
        logger.info("Message : %s" % message)

        data = json.loads(message)

        import Servo

        try:
            if data['cmd'] == 'set':
                if data['type'] == 'trim':
                    Servo.Servo_Trim.values[data['servo']] = data['value'];
                    B.Sequencer.Servo.sendValues()
                elif data['type'] == 'limit':
                    Servo.Servo_Limit.values[data['servo']] = [ data['min'], data['max'] ];
                    B.Sequencer.Servo.sendValues()
                elif data['type'] == 'position':
                    B.Sequencer.Servo.setValue(data['servo'], data['value'])
                    B.Sequencer.Servo.sendValues()
                elif data['type'] == 'speed':
                    global speed
                    speed = data['value']
                elif data['type'] == 'livemode':
                    if data['status']:
                        B.Sequencer.Servo.setCallback(self.livemode)
                    else:
                        B.Sequencer.Servo.removeCallback()
                elif data['type'] == 'loglevel':

                    if data['which'] == 'js':
                        _logger = jsHandler
                        #Data.Instance().set(['log', 'js'], data['level'])
                    else:
                        _logger = rainbowHandler
                        #Data.Instance().set(['log', 'default'], data['level'])
                    print(data);
                    #Data.Instance().save()

                    if data['level'] == 'debug':
                        _logger.setLevel(logging.DEBUG)
                    elif data['level'] == 'info':
                        _logger.setLevel(logging.INFO)
                    elif data['level'] == 'warning':
                        _logger.setLevel(logging.WARNING)
                    elif data['level'] == 'error':
                        _logger.setLevel(logging.ERROR)
                    elif data['level'] == 'warning':
                        _logger.setLevel(logging.CRITICAL)

                #print logger.getEffectiveLevel()

            elif data['cmd'] == 'drive':

                if data['status'] == 'begin':
                    if data['direction'] == 'forward':
                        B.Drive.forward()
                    elif data['direction'] == 'reverse':
                        B.Drive.reverse()
                    elif data['direction']== 'left':
                        B.Drive.left()
                    elif data['direction']== 'right':
                        B.Drive.right()
                elif data['status'] == 'end':
                    B.Drive.end()

            elif data['cmd'] == 'config':
                if data['action'] == 'save':
                    Data.Instance().set(['servo', 'trims'], Servo.Servo_Trim.values)
                    Data.Instance().set(['servo', 'limits'], Servo.Servo_Limit.values)
                    Data.Instance().save()
                elif data['action'] == 'get':
                    config = {
                        'type': 'config',
                        'data': {
                            'trims':    Data.Instance().get(['servo', 'trims']),
                            'limits':   Data.Instance().get(['servo', 'limits'])
                        }
                    }
                    self.write(json.dumps(config))

            elif data['cmd'] == 'control':

                if data['action'] == 'reset':
                    B.BPi.reset()

            elif data['cmd'] == 'sequence':

                if data['name'] == 'middle':
                    B.Sequencer.forward(Sequences['middle'], 1)
                elif data['name'] == 'pushup':
                    B.Sequencer.forward(Sequences['pushup'], 1)
                elif data['name'] == 'standby':
                    B.Sequencer.forward(Sequences['standby'], 1)
                elif data['name'] == 'release':
                    B.Sequencer.forward(Sequences['release'], 1)

            elif data['cmd'] == 'servo':

                if data['name'] == 'init':
                    B.Sequencer.Servo.init()
                elif data['name'] == 'pause':
                    B.Sequencer.Servo.pause()
                elif data['name'] == 'resume':
                    B.Sequencer.Servo.resume()

            else:
                logger.warning("Message : %s" % message)

        except Exception, e:
            logger.critical("Exception ! " + str(e));

    def on_close(self):
        if self in cl:
            cl.remove(self)

class ApiHandler(web.RequestHandler):

    @web.asynchronous
    def get(self, *args):
        self.finish()

    @web.asynchronous
    def post(self):
        pass

app = web.Application([
    (r'/', IndexHandler),
    (r'/ws', SocketHandler),
    (r'/api', ApiHandler),
    (r'/static/(.*)', web.StaticFileHandler, {'path': './www/'}),
    #(r'/(favicon.ico)', web.StaticFileHandler, {'path': './www/'}),
])

if __name__ == '__main__':
    app.listen(8888)
    try:
        ioloop.IOLoop.instance().start()
    except KeyboardInterrupt:
        print "Stop!"
        ioloop.IOLoop.instance().stop()

