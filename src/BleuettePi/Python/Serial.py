
import serial, types

class Serial:

    com = None

    HEADER = chr(255)

    MAX_RETRY = 3

    CMD_INIT    = 'I'   # Init
    CMD_PAUSE   = 'P'   # Pause all servo
    CMD_RESUME  = 'R'   # Resume from pause
    CMD_CLEAR   = 'C'   # Clear all values
    CMD_SET     = 'S'   # Set consigne

    CMD_CURRENT = 'A'   # Get last current value
    CMD_STATUS  = 'T'   # Get status

    CMD_READAN0 = '0'   # Read analog value 0
    CMD_READAN1 = '1'   # Read analog value 1
    CMD_READAN2 = '2'   # Read analog value 2
    CMD_READAN3 = '3'   # Read analog value 3
    CMD_READAN4 = '4'   # Read analog value 4
    CMD_READAN5 = '5'   # Read analog value 5
    CMD_READAN6 = '6'   # Read analog value 6
    CMD_READAN7 = '7'   # Read analog value 7

    def connect(self, mixed):
        if type(mixed) == types.StringType:
            self.com = serial.Serial(mixed, 9600, timeout = 1)
        else:
            print 'Serial Object'
            self.com = mixed

        #ser.open()

        return self.com

