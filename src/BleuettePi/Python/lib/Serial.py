
import serial, types, struct

class Serial:

    HEADER = chr(255)

    MAX_RETRY = 3

    DELAY_BYTE = 0.002

    ACK = 'O'
    NACK = 'N'

    serial = None

    def __init__(self):
        pass

    def connect(self, serialDev):
        if type(serialDev) == types.StringType:
            self.serial = serial.Serial(serialDev, 9600, timeout = 1)
        else:
            print 'Serial Object'
            self.serial = serialDev

        #ser.open()

        return self.serial

    def write(self, data):
        self.serial.write(data)

    def read(self, count):
        return self.serial.read(count)

    def getResponse(self):
        return self.serial.read(1)

    def getLastStatus(self):
        return (self.last_status_code == self.ACK)

    # Send only one command
    def command(self, cmd, param = None):
        self.serial.write(self.HEADER)
        self.serial.write(cmd)

        if param:
            for p in param:
                self.serial.write(p)

        self.last_status_code = self.getResponse()
        return self.getLastStatus()

