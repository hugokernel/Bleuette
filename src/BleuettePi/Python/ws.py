#!/usr/bin/env python
import json
import threading
from tornado import websocket, web, ioloop

import os
import sys
import time, time
sys.path.insert(0, os.path.abspath('lib'))

from Bleuette import Bleuette
from Sequences import Sequences

cl = []

B = Bleuette()

class Affiche(threading.Thread):
    def __init__(self, nom = ''):
        threading.Thread.__init__(self)
        self.nom = nom
        self._stopevent = threading.Event( )
    def run(self):
        i = 0
        while not self._stopevent.isSet():
            #print self.nom, i

            compass = None
            acc = B.BPi.Accelerometer.get()
            ground = B.BPi.GroundSensor.get()

            current = B.BPi.Analog.getCurrent()

            data = {
                'sensors': {
                    'accelerometer': acc,
                    'compass': compass,
                    'ground': ground,
                    'current': current
                }
            }

            data = json.dumps(data)
            for c in cl:
                c.write_message(data)
            i += 100
            self._stopevent.wait(0.5)
        print "le thread "+self.nom +" s'est termine proprement"
    def stop(self):
        self._stopevent.set( )

a = Affiche('Thread A')
a.daemon = True;
a.start();

class IndexHandler(web.RequestHandler):
    def get(self):
        self.render("index.html")

class SocketHandler(websocket.WebSocketHandler):

    def open(self):
        if self not in cl:
            cl.append(self)

    def on_message(self, message):
        print("Message : " + message)

        if message == 'forward':
            B.Sequencer.forward(Sequences['walk'], 1)
        elif message == 'reverse':
            B.Sequencer.reverse(Sequences['walk'], 1)
        elif message == 'middle':
            B.Sequencer.reverse(Sequences['middle'], 1)
        elif message == 'pushup':
            B.Sequencer.reverse(Sequences['pushup'], 1)


    def on_close(self):
        if self in cl:
            cl.remove(self)

class ApiHandler(web.RequestHandler):

    @web.asynchronous
    def get(self, *args):
        self.finish()

        action = self.get_argument('action')
        count = self.get_argument('count')
        if action == 'walk':
            B.Sequencer.forward(Sequences['walk'], int(count))

        #id = self.get_argument("id")
        #value = self.get_argument("value")
        #data = {"id": id, "value" : value}
        #data = json.dumps(data)
        #for c in cl:
        #    c.write_message(data)

    @web.asynchronous
    def post(self):
        pass

app = web.Application([
    (r'/', IndexHandler),
    (r'/ws', SocketHandler),
    (r'/api', ApiHandler),
    (r'/(favicon.ico)', web.StaticFileHandler, {'path': '../'}),
    (r'/(rest_api_example.png)', web.StaticFileHandler, {'path': './'}),
])

if __name__ == '__main__':
    app.listen(8888)
    ioloop.IOLoop.instance().start()
