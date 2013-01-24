/*
    ServoController.h - Bleuette servo controller
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

    unsigned char _values[SERVO_COUNT];

    void write(unsigned char);

public:
    ServoController();
    ServoController(HardwareSerial &);

    inline void setValue(unsigned char, unsigned char);

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

ServoController::ServoController() :
    _serial(Serial1)
{
    _last_status_code = 0;
    memset(&_values, 0x00, sizeof(_values));

    _serial.begin(9600);
}

ServoController::ServoController(HardwareSerial &serial) :
    _serial(serial)
{
    _last_status_code = 0;
};

inline bool ServoController::getLastStatus()
{
    return (_last_status_code == SERVO_ACK) ? true : false;
}

unsigned long ServoController::get(unsigned char servo)
{
    unsigned long servos[] = {
        SERVO_0,
        SERVO_1,
        SERVO_2,
        SERVO_3,
        SERVO_4,
        SERVO_5,
        SERVO_6,
        SERVO_7,
        SERVO_8,
        SERVO_9,
        SERVO_10,
        SERVO_11,
        SERVO_12,
        SERVO_13
    };

    return (servo < sizeof(servos) ? servos[servo] : 0);
}

int ServoController::getResponse()
{
    while (!_serial.available());
    return _serial.read();
}

inline void ServoController::write(unsigned char byte)
{
    _serial.write(byte);
    delay(SERVO_DELAY_BYTE);
}

inline bool ServoController::command(SERVO_CMD command)
{
    write(SERVO_HEADER);
    delay(SERVO_DELAY_BYTE);
    write(command);
    delay(SERVO_DELAY_BYTE);

    _last_status_code = getResponse();

    return getLastStatus();
}

bool ServoController::command(SERVO_CMD command, unsigned char data[], unsigned char size)
{
    unsigned char c = 0;
    unsigned char control = 0;
    unsigned char retry = SERVO_MAX_RETRY;

    do {
        write(SERVO_HEADER);
        write(command);

        control = 0;
        for (c = 0; c < size; c++) {
            write(data[c]);

            control += data[c];
        }

        write(control);
        delay(SERVO_DELAY_BYTE);

        _last_status_code = getResponse();
        if (_last_status_code == SERVO_ACK) {
            break;
        }

    } while (retry--);

    return getLastStatus();
}

bool ServoController::init()
{
    return command(SERVO_CMD_INIT);
}

/**
 *  Stop impulsion generation
 */
bool ServoController::pause()
{
    return command(SERVO_CMD_PAUSE);
}

/**
 *  Restart impulsion
 */
bool ServoController::resume()
{
    return command(SERVO_CMD_RESUME);
}

/**
 *  Set to 0 all consigne
 */
bool ServoController::clear()
{
    return command(SERVO_CMD_CLEAR);
}

inline void ServoController::setValue(unsigned char index, unsigned char value)
{
    _values[index] = value;
}

/**
 *  Set servo values
 *  servos is a bits field and values is the values only for bit set
 *
 *  Example:
 *  - Set all servo to 200 :
 *      servos : 0xFFFF, values[] = 200, 200, 200, 200, ...
 *  - Set servos 0 & 2 to 100 :
 *      servos : 0b00000000 00000101, values[] = 100, 100
 *
 */
void ServoController::setValues(unsigned long servos, unsigned char * values, unsigned char count)
{
    unsigned char b, v = 0;
    for (b = 0, v = 0; v < count; b++) {
        if (CHECK_BIT(servos, b)) {
            // Todo: replace with setValue
            _values[b] = values[v++];
            //setValue(b, v++);
        }
    }
}

void ServoController::sendValues()
{
    command(SERVO_CMD_SET, _values, sizeof(_values));
}

#endif
