
from Singleton import Singleton
import json

@Singleton
class Data:

    FILE = 'data.json'

    __data = {}

    def __init__(self):
        self.load()

    def load(self, file = None):
        try:
            data = open((file if file else self.FILE), 'r').read()
            self.__data = json.loads(data)
        except IOError:
            pass

    def get(self, key):
        if len(key) == 2:
            return self.__data[key[0]][key[1]]
        else:
            return self.__data[key[0]]

    def set(self, key, data):
        if len(key) == 2:
            self.__data[key[0]][key[1]] = data
        else:
            self.__data[key[0]] = data

    def save(self, file = None):
        with open((file if file else self.FILE), 'w') as jfile:
            jfile.write(json.dumps(self.__data))

