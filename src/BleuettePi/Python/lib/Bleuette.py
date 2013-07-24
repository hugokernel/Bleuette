
from BleuettePi import BleuettePi
from Sequencer import Sequencer
from Analog import Analog
import config as Config

class Fake:
    @staticmethod
    def write(self):
        pass
    @staticmethod
    def read(self):
        return 'a'

class Bleuette():

    BPi = None
    Sequencer = None

    def __init__(self):
        if Config.FAKE_MODE == False:
            self.BPi = BleuettePi(Config.SERIAL_DEV)
        else:
            self.BPi = BleuettePi(Fake)

        self.Sequencer = Sequencer(self.BPi.Servo)

if __name__ == '__main__':
    '''
   try:
      opts, args = getopt.getopt(sys.argv[1:],"hb:o:s:",["bank=","output=","state="])

      if not opts:
        usage()
        sys.exit(2)

   except getopt.GetoptError:
      usage()
      sys.exit(2)

   for opt, arg in opts:
      if opt == '-h':
         usage()
         sys.exit()
      elif opt in ("-b", "--bank"):
         bank = arg
      elif opt in ("-o", "--output"):
         output = int(arg)
      elif opt in ("-s", "--state"):
         state = arg
    '''
