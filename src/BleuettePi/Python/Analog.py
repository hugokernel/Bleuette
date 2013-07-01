
import types
from Serial import Serial

class Analog(Serial):

    def __init__(self, mixed):
        self.com = mixed

    # Read value
    def read(self, index):
        self.com.write(self.HEADER)
        self.com.write(index)
        return self.getResponse()

'''
        if type(mixed) == types.StringType:
            self.com = serial.Serial(mixed, 9600, timeout = 1)
        else:
            print 'Serial Object'
            self.com = mixed
'''
