
from BleuettePi import BleuettePi
from Sequencer import Sequencer
from Drive import Drive
from Analog import Analog
from Fake import SerialFake
import config as Config

class Bleuette():

    BPi = None
    Sequencer = None
    Drive = None

    def __init__(self):
        if Config.FAKE_MODE == False:
            self.BPi = BleuettePi(Config.SERIAL_DEV)
        else:
            self.BPi = BleuettePi(SerialFake)

        self.Sequencer = Sequencer(self.BPi.Servo)
        self.Drive = Drive(self.Sequencer)

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
