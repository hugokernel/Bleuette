
class BPi_Cmd:

    VERSION = 'V'   # @Todo: Get version

    # Servo related cmd
    INIT    = 'I'   # Init
    PAUSE   = 'P'   # Pause all servo
    RESUME  = 'R'   # Resume from pause
    CLEAR   = 'C'   # Clear all values
    SET     = 'S'   # Set consigne

    # Analog related
    CURRENT = 'A'   # Get last current value
    STATUS  = 'T'   # Get status
    SET_MAX = 'M'   # @Todo: Set current alarm value

    READAN0 = '0'   # Read analog value 0
    READAN1 = '1'   # Read analog value 1
    READAN2 = '2'   # Read analog value 2
    READAN3 = '3'   # Read analog value 3
    READAN4 = '4'   # Read analog value 4
    READAN5 = '5'   # Read analog value 5
    READAN6 = '6'   # Read analog value 6
    READAN7 = '7'   # Read analog value 7

    @staticmethod
    def isValid(name):
        return hasattr(BPi_Cmd, name)


DELAY_MIN = 0.2

__      = 230
PAUSE   = 231

FRONT   = 235
BACK    = 236

DOWN    = 240
UP      = 241

MID     = 245

