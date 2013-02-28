/*
    ServoController - Bleuette servo controller
    Copyright (c) 2013 Charles Rincheval.  All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    The latest version of this library can always be found at
    http://www.digitalspirit.org/
*/

//#define ENABLE_NEW_SOFT_SERIAL

#ifndef ServoController_h
#define ServoController_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "HardwareSerial.h"

#define SERVO_DEBUG 0

extern HardwareSerial Serial1;

#define CHECK_BIT(var, pos) (var & (1 << pos))

#define SERVO_COUNT   14

#define SERVO_ACK    'O'
#define SERVO_NACK   'N'

#define SERVO_HEADER 255

#define SERVO_MAX_RETRY     3
#define SERVO_DELAY_BYTE    2

enum SERVO_CMD {
    SERVO_CMD_INIT      = 'I',
    SERVO_CMD_PAUSE     = 'P',
    SERVO_CMD_RESUME    = 'R',
    SERVO_CMD_CLEAR     = 'C',
    SERVO_CMD_SET       = 'S',
};

enum SERVO_LIST {

    SERVO_0 = 1,      // RA0
    SERVO_1 = 2,      // RA1

    SERVO_2 = 4,      // RA2
    SERVO_3 = 8,      // RA3

    SERVO_4 = 16,     // RA4
    SERVO_5 = 32,     // RC0

    SERVO_6 = 64,     // RB0
    SERVO_7 = 128,    // RB1

    SERVO_8 = 256,    // RB2
    SERVO_9 = 512,    // RB3

    SERVO_10 = 1024,  // RB4
    SERVO_11 = 2048,  // RB5

    SERVO_12 = 4096,  // RD0
    SERVO_13 = 8192,  // RD1

};

class ServoController
{
private:
    HardwareSerial _serial;
    int _last_status_code;

    void write(unsigned char);

    unsigned char _buffer[SERVO_COUNT];
    unsigned char _current_values[SERVO_COUNT];

public:
    ServoController();
    ServoController(HardwareSerial &);

    void setValue(unsigned char, unsigned char);

    unsigned char getPosition(unsigned char);
    void getPositions(unsigned char &);

    void setValues(unsigned long, unsigned char *, unsigned char);
    void sendValues();

    unsigned long get(unsigned char);

    inline bool getLastStatus();
    int getResponse();

    inline bool command(SERVO_CMD);
    bool command(SERVO_CMD, unsigned char [], unsigned char);

    bool init();
    bool pause();
    bool resume();
    bool clear();
};

#endif
