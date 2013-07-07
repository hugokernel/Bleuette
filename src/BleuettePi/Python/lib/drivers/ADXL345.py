#!/usr/bin/python
# coding=<UTF-8>

import time
from Adafruit_I2C import Adafruit_I2C

# ===========================================================================
# ADXL345 Class, written for Python 3.x Modified 5-3-13 by Static
# This is definitely a work in progress.  This code relies on the
# Adafruit_I2C.py code.  Please support the open source movement, and those
# dedicated to open source.  www.adafruit.com
# If you find this code useful or have suggestions, hit me up at:
# medicforlife.blogspot.com
# As it stands, the code isn't commented out.  A ton of revision still needs to happen
# The end goal of this code is to spit out acceleration readings on demand.
# Due to the constraints of the ADXL345, this will always be three readings, X,Y,Z
# Thanks to tldr for making me do the math correctly.
# ===========================================================================

class ADXL345 :
    i2c = None

    # Registers
    __ADXL345_DEVID = 0x00
    __ADXL345_THRESH_TAP = 0x1D     #8 bits
    __ADXL345_X_OFFSET = 0x1E     #8 bits
    __ADXL345_Y_OFFSET = 0x1F     #8 bits
    __ADXL345_Z_OFFSET = 0x20     #8 bits
    __ADXL345_TAP_DUR = 0x21     #8 bits
    __ADXL345_TAP_LATENCY = 0x22     #8 bits
    __ADXL345_TAP_WINDOW = 0x23     #8 bits
    __ADXL345_THRESH_ACT = 0x24     #8 bits
    __ADXL345_THRESH_INACT = 0x25     #8 bits
    __ADXL345_TIME_INACT = 0x26     #8 bits
    __ADXL345_ACT_INACT_CTL = 0x27
    __ADXL345_THRESH_FF = 0x28     #8 bits
    __ADXL345_TIME_FF = 0x29     #8 bits
    __ADXL345_TAP_AXES = 0x2A
    __ADXL345_ACT_TAP_STATUS = 0x2B
    __ADXL345_BW_RATE = 0x2C
    __ADXL345_PWR_CTL = 0x2D
    __ADXL345_INT_ENABLE = 0x2E
    __ADXL345_INT_MAP = 0x2F
    __ADXL345_INT_SOURCE = 0x30
    __ADXL345_DATA_FORMAT = 0x31
    __ADXL345_DATA_X0 = 0x32     #8 bits
    __ADXL345_DATA_X1 = 0x33     #8 bits
    __ADXL345_DATA_Y0 = 0x34     #8 bits
    __ADXL345_DATA_Y1 = 0x35     #8 bits
    __ADXL345_DATA_Z0 = 0x36     #8 bits
    __ADXL345_DATA_Z1 = 0x37     #8 bits
    __ADXL345_FIFO_CTL = 0x38
    __ADXL345_FIFO_STATUS = 0x39

    # Datarate controls using BW_RATE register 0x2c
    __ADXL345_DR_3200 = 0b00001111
    __ADXL345_DR_1600 = 0b00001110
    __ADXL345_DR_800 = 0b00001101
    __ADXL345_DR_400 = 0b00001100
    __ADXL345_DR_200 = 0b00001011
    __ADXL345_DR_100 = 0b00001010
    __ADXL345_DR_50 = 0b00001001
    __ADXL345_DR_25 = 0b00001000
    __ADXL345_DR_12_5 = 0b00000111
    __ADXL345_DR_6_25 = 0b00000110

    # Datarate controls using BW_RATE at low power
    __ADXL345_DR_400 = 0b00011100
    __ADXL345_DR_200 = 0b00011011
    __ADXL345_DR_100 = 0b00011010
    __ADXL345_DR_50 = 0b00011001
    __ADXL345_DR_25 = 0b00011000
    __ADXL345_DR_12_5 = 0b00010111

    # Data Format using DATA_FORMAT 0x31
    __ADXL345_SELFTEST =    0b10000000
    __ADXL345_SPI_BIT =     0b01000000
    __ADXL345_INT_INVERT =  0b00100000
    __ADXL345_FULL_RES =    0b00001000
    __ADXL345_JUSTIFY =     0b00000100
    __ADXL345_2G =          0b00000000
    __ADXL345_4G =          0b00000001
    __ADXL345_8G =          0b00000010
    __ADXL345_16G =         0b00000011

    # FIFO Control using FIFO_CTL 0x38
    __ADXL345_BYPASS =      0b00000000
    __ADXL345_FIFO =        0b01000000
    __ADXL345_STREAM =      0b10000000
    __ADXL345_TRIG_MODE =   0b11000000
    __ADXL345_TRIG_INT1 =   0b00000000
    __ADXL345_TRIG_INT2 =   0b00100000
    __ADXL345_SAMPLES31 =   0b00011111
    __ADXL345_SAMPLES16 =   0b00010000
    __ADXL345_SAMPLES10 =   0b00001010

    # Power control using PWR_CTL 0x2D
    __ADXL345_LINK_BIT    = 0b00100000
    __ADXL345_AUTO_SLEEP  = 0b00010000
    __ADXL345_MEASURE     = 0b00001000
    __ADXL345_STANDBY     = 0b00000000
    __ADXL345_SLEEP       = 0b00000100
    __ADXL345_WAKEUP8HZ   = 0b00000000
    __ADXL345_WAKEUP4HZ   = 0b00000001
    __ADXL345_WAKEUP2HZ   = 0b00000010
    __ADXL345_WAKEUP1HZ   = 0b00000011

    # Constructor
    def __init__(self, address = 0x53, busnum = -1, debug = False):
        self.i2c = Adafruit_I2C(address, busnum, debug)
        self.address = address
        self.debug = debug
        self.xcal = False
        self.ycal = False
        self.zcal = False
        if address == 0x53:
            self.__ADXL345_WRITE = 0xA6
            self.__ADXL345_READ = 0xA7
        elif address == 0x1D:
            self.__ADXL345_WRITE = 0x3A
            self.__ADXL345_READ = 0x3B
        else:
            if self.debug == True:
                print("Invalid I2C Address for ADXL345: ", self.address)
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_BW_RATE, self.__ADXL345_DR_100)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, (self.__ADXL345_2G + self.__ADXL345_FULL_RES))
        self.gScale = 2
        self.scale = 0.043
        self.i2c.write8(self.__ADXL345_FIFO_CTL, self.__ADXL345_STREAM)

    def setgScale2(self):
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, (self.__ADXL345_2G + self.__ADXL345_FULL_RES))
        self.gscale = 2
        self.scale = 0.043

    def setgScale4(self):
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, (self.__ADXL345_4G + self.__ADXL345_FULL_RES))
        self.gscale = 4
        self.scale = 2 * 0.043

    def setgScale8(self):
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, (self.__ADXL345_8G + self.__ADXL345_FULL_RES))
        self.gscale = 8
        self.scale = 4 * 0.043

    def setgScale16(self):
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, (self.__ADXL345_16G + self.__ADXL345_FULL_RES))
        self.gscale = 16
        self.scale = 8 * 0.043

    def getgScale(self):
        return self.gscale
    def getScale(self):
        return self.scale

    def gravTest(self):
        interval = 0.1
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, self.__ADXL345_2G)
        self.i2c.write8(self.__ADXL345_FIFO_CTL, self.__ADXL345_STREAM)
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_MEASURE)
        time.sleep(interval)
        print("2G self test values X,Y,Z",self.scaledAccel(0.0043))
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, self.__ADXL345_4G)
        self.i2c.write8(self.__ADXL345_FIFO_CTL, self.__ADXL345_STREAM)
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_MEASURE)
        time.sleep(interval)
        print("4G self test values X,Y,Z",self.scaledAccel((0.0043*2)))
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, self.__ADXL345_8G)
        self.i2c.write8(self.__ADXL345_FIFO_CTL, self.__ADXL345_STREAM)
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_MEASURE)
        time.sleep(interval)
        print("8G self test values X,Y,Z",self.scaledAccel((0.0043*4)))
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_STANDBY)
        self.i2c.write8(self.__ADXL345_DATA_FORMAT, self.__ADXL345_16G)
        self.i2c.write8(self.__ADXL345_FIFO_CTL, self.__ADXL345_STREAM)
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_MEASURE)
        time.sleep(interval)
        print("16G self test values X,Y,Z",self.scaledAccel((0.0043*8)))

    def quickCalibrate(self, cycles = 100):
        i = 0
        x,y,z,xrun,yrun,zrun,xavg,yavg,zavg = 0,0,0,0,0,0,0,0,0
        for i in range(cycles):
            x,y,z = self.readAccel()
            xrun = xrun+x
            yrun = yrun+y
            zrun = zrun+z
        xavg = xrun/cycles
        yavg = yrun/cycles
        zavg = zrun/cycles
        if self.debug == True:
            print(xavg,yavg,zavg)
        self.xcal = xavg
        self.ycal = yavg
        self.zcal = zavg
        #return self.xcal, self.ycal, self.zcal

    def readAccelCal(self):
        if self.xcal == False or self.ycal == False or self.zcal == False:
            self.quickCalibrate()
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_MEASURE) # Set power control to measure
        x0 = self.i2c.readU8(self.__ADXL345_DATA_X0)                    # Read X-Axis 0
        x1 = self.i2c.readU8(self.__ADXL345_DATA_X1)                    # Read X-Axis 1
        X = x0 + (x1 << 8)                                              # By default, add the two together (bit-shift x1)
        if (x1 > 15):                                                   # if x1 signifies a negative
            x1 = (x1 & 0b1111)                                            # don't count the first 4 bits
            X = (x0 + (x1 << 8))+1                                        # bit shift and add to x0, then add 1
            X = -X                                                        # then multiply by -1 (make this a negative value)

        y0 = self.i2c.readU8(self.__ADXL345_DATA_Y0)
        y1 = self.i2c.readU8(self.__ADXL345_DATA_Y1)
        Y = y0 + (y1 << 8)
        if (y1 > 15):
            y1 = (y1 & 0b1111)
            Y = (y0 + (y1 << 8))+1
            Y = -Y

        z0 = self.i2c.readU8(self.__ADXL345_DATA_Z0)
        z1 = self.i2c.readU8(self.__ADXL345_DATA_Z1)
        Z = z0 + (z1 << 8)
        if (z1 > 15):
            z1 = (z1 & 0b1111)
            Z = (z0 + (z1 << 8))+1
            Z = -Z

        if self.debug == True:
            print(X - self.xcal, Y - self.ycal, Z - self.zcal)
        return X - self.xcal, Y - self.ycal, Z - self.zcal              # subtract the quick calibrate average values and return

    def scaledAccel(self, ScaleFactor):
        x,y,z = self.readAccel()
        x = x * ScaleFactor
        y = y * ScaleFactor
        z = z * ScaleFactor
        return x,y,z

    def scaledAccelCal(self, ScaleFactor):
        x,y,z = self.readAccelCal()
        x = x * ScaleFactor
        y = y * ScaleFactor
        z = z * ScaleFactor
        return x,y,z

    def readAccel(self):
        self.i2c.write8(self.__ADXL345_PWR_CTL, self.__ADXL345_MEASURE) # Set power control to measure
        x0 = self.i2c.readU8(self.__ADXL345_DATA_X0)                    # Read X-Axis 0
        x1 = self.i2c.readU8(self.__ADXL345_DATA_X1)                    # Read X-Axis 1
        X = x0 + (x1 << 8)                                              # By default, add the two together (bit-shift x1)
        if (x1 > 15):                                                   # if x1 signifies a negative
            x1 = (x1 & 0b1111)                                            # don't count the first 4 bits
            X = (x0 + (x1 << 8))+1                                        # bit shift and add to x0, then add 1
            X = -X                                                        # then multiply by -1 (make this a negative value)

        y0 = self.i2c.readU8(self.__ADXL345_DATA_Y0)
        y1 = self.i2c.readU8(self.__ADXL345_DATA_Y1)
        Y = y0 + (y1 << 8)
        if (y1 > 15):
            y1 = (y1 & 0b1111)
            Y = (y0 + (y1 << 8))+1
            Y = -Y

        z0 = self.i2c.readU8(self.__ADXL345_DATA_Z0)
        z1 = self.i2c.readU8(self.__ADXL345_DATA_Z1)
        Z = z0 + (z1 << 8)
        if (z1 > 15):
            z1 = (z1 & 0b1111)
            Z = (z0 + (z1 << 8))+1
            Z = -Z

        if self.debug == True:
            print(X,Y,Z)

        return X,Y,Z

    def scaledAccel(self, ScaleFactor):
        x,y,z = self.readAccel()
        x = x * ScaleFactor
        y = y * ScaleFactor
        z = z * ScaleFactor
        return x,y,z

if __name__ == '__main__':
    import time
    a = ADXL345()#debug = True)
    #a.gravTest()
#    a.quickCalibrate(1000)
    while True:
        print a.scaledAccelCal(1)
        time.sleep(1)

