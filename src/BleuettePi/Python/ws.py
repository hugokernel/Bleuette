#!/usr/bin/env python
import json
import threading
from tornado import websocket, web, ioloop
from operator import eq

import os
import sys, time, copy
sys.path.insert(0, os.path.abspath('lib'))

from Bleuette import Bleuette
from Sequences import Sequences
import Drive

cl = []

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

            current = B.BPi.Analog.getCurrent()

            self.data = {
                'sensors': {
                    'accelerometer':    acc,
                    'compass':          compass,
                    'ground':           ground,
                    'current':          current
                }
            }

            if any(map(eq, self.data, self.data_last)) == False:
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

class IndexHandler(web.RequestHandler):
    def get(self):
        self.render("index.html")

class SocketHandler(websocket.WebSocketHandler):

    last_message = None

    def open(self):
        print "New client !"

        if self not in cl:
            cl.append(self)

        a.update()

    def on_message(self, message):
        if message != self.last_message or message == 'release':
            print("Message : " + message)
            if message == 'forward':
                B.Drive.forward()
                #B.Sequencer.forward(Sequences['walk'], 1)
            elif message == 'reverse':
                B.Drive.reverse()
                #B.Sequencer.reverse(Sequences['walk'], 1)
            elif message == 'left':
                B.Drive.left()
            elif message == 'right':
                B.Drive.right()
            elif message == 'middle':
                B.Sequencer.reverse(Sequences['middle'], 1)
            elif message == 'pushup':
                B.Sequencer.reverse(Sequences['pushup'], 1)
            elif message == 'release':
                B.Drive.end()

        self.last_message = message

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
    (r'/(favicon.ico)', web.StaticFileHandler, {'path': './'}),
    (r'/(ws.js)', web.StaticFileHandler, {'path': './'}),
])

if __name__ == '__main__':
    app.listen(8888)
    ioloop.IOLoop.instance().start()
