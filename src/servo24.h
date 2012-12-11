/*
    Servo24.h - @4 servos Arduino drivers
    Copyright (c) 2009 Charles Rincheval.  All rights reserved.

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
#define SERVO24_MODE_IS_BLEUETTE

#ifndef Servo24_h
#define Servo24_h

#include <wiring.h>

#include "HardwareSerial.h"
#ifdef ENABLE_NEW_SOFT_SERIAL
#include "NewSoftSerial.h"
#endif

enum SERVO24_MODE {
    SERVO24_MODE_UNIT = 0,
    SERVO24_MODE_BLOCK,
    SERVO24_MODE_3BLOCK,

    SERVO24_MODE_BLEUETTE
};

#define CHECK_BIT(var, pos) (var & (1 << pos))

#ifdef SERVO24_MODE_IS_BLEUETTE
#define SERVO24_COUNT   14

enum SERVO24_LIST {

    SERVO24_0 = 1,      // RA0
    SERVO24_1 = 2,      // RA1

    SERVO24_2 = 4,      // RA2
    SERVO24_3 = 8,      // RA3

    SERVO24_4 = 16,     // RA4
    SERVO24_5 = 32,     // RC0

    SERVO24_6 = 64,     // RB0
    SERVO24_7 = 128,    // RB1

    SERVO24_8 = 256,    // RB2
    SERVO24_9 = 512,    // RB3

    SERVO24_10 = 1024,  // RB4
    SERVO24_11 = 2048,  // RB5

    SERVO24_12 = 4096,  // RD0
    SERVO24_13 = 8192,  // RD1

};
#endif

template <class T>
class Servo24
{
private:
    T _serial;
    unsigned char _mode;

#ifdef SERVO24_MODE_IS_BLEUETTE
    char _values[14];
#else
    char _limits[24 * 3];
#endif

public:
    Servo24(HardwareSerial &, unsigned char);
#ifdef ENABLE_NEW_SOFT_SERIAL
    Servo24(NewSoftSerial &, unsigned char);
#endif

    void setMode(SERVO24_MODE);

#ifdef SERVO24_MODE_UNIT
    void set(unsigned char, unsigned char);
#endif

#ifdef SERVO24_MODE_IS_BLEUETTE
    void setValues(unsigned long, char *);
    void send();
#else
    // Limits
    void setLimit(char, unsigned char, unsigned char) {
    void setLimits(char *);
    void getLimits(unsigned char, unsigned char *)

    void block(char, char, char, char, char, char);
#endif

    void command(char [], unsigned char);

};

#define Servo24Hard Servo24<HardwareSerial>

#ifdef ENABLE_NEW_SOFT_SERIAL
#define Servo24Soft Servo24<NewSoftSerial>
#endif

template <class T>
Servo24<T>::Servo24(HardwareSerial &serial, unsigned char mode) :
    _serial(serial),
    _mode(mode)
{
#ifdef SERVO24_MODE_IS_BLEUETTE
    memset(&_values, 0x00, sizeof(_values));
#endif
    // Todo: Set the speed
};

#ifdef ENABLE_NEW_SOFT_SERIAL
template <class T>
Servo24<T>::Servo24(NewSoftSerial &serial, unsigned char mode) :
    _serial(serial),
    _mode(mode)
{
#ifdef SERVO24_MODE_IS_BLEUETTE
    memset(&_values, 0x00, sizeof(_values));
#endif
};
#endif

template <class T>
void Servo24<T>::setMode(SERVO24_MODE mode)
{
    _mode = mode;
};

#ifdef SERVO24_MODE_UNIT
template <class T>
void Servo24<T>::set(unsigned char servo, unsigned char position)
{
    char data[2];
    data[0] = servo;
    data[1] = position;
    command(data, 2);
}
#endif

#ifndef SERVO24_MODE_IS_BLEUETTE
template <class T>
void Servo24<T>::setLimit(char servo, unsigned char min, unsigned char max)
{
    unsigned char c = 0;

    // Test limits
    for (c = 0; c < 8; c++) {
        if (CHECK_BIT(servo, c)) {
            _limits[c * 2] = min;
            _limits[c * 2 + 1] = max;
        }
    }
}

template <class T>
void Servo24<T>::setLimits(unsigned char * limits)
{
    memcpy(_limits, limits, sizeof(_limits));
}

template <class T>
void Servo24<T>::getLimits(unsigned char servo, unsigned char * limits)
{
    limits[0] = _limits[servo * 3];
    limits[1] = _limits[servo * 3 + 1];
}

template <class T>
void Servo24<T>::testLimits(char * data) {
    char c = 0;

    // Test limits
    for (c = 0; c < 8; c++) {
        if (CHECK_BIT(val0[0], c)) {
            getLimits(c, &limits);

            // Too low !
            if (val0[1] < limits[0]) {
                val0[1] = limits[0];
            }

            // Too big !
            if (val0[1] > limits[1]) {
                val0[1] = limits[1];
            }
        }
    }
}

template <class T>
//void Servo24<T>::block(char servos0, char servos1, char servos2, char values0, char values1, char values2)
/*
 *  block([servo0 & servo1, 255], ...
 */
void Servo24<T>::block(char val0[], char val1[], char val2[])
{
    unsigned char c = 0;
    char data[6];
    char limit[2];

    testLimits(&val0);
    testLimits(&val1);
    testLimits(&val2);

    /*
    data = {
        servos2, servos1, servos0,
        values2, values1, values0
    };
    */

    command(data, 6);
};
#endif

template <class T>
void Servo24<T>::command(char data[], unsigned char size)
{
    unsigned char i = 0;

    for (i = 0; i < size; i++) {
        _serial.print(data[i]);
    }

/*
    if (str) {
        do {
            _serial.print(*str, BYTE);
            str++;
        } while (*str);
        _serial.print(0x00, BYTE);
    }

    delay(20);
    _last_status_code = getResponse();
*/
};

#ifdef SERVO24_MODE_IS_BLEUETTE
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
template <class T>
void Servo24<T>::setValues(unsigned long servos, char * values)
{
    unsigned char b, c = 0;
    for (b = 0; b < SERVO24_COUNT; b++) {
        if (CHECK_BIT(servos, b)) {
            _values[b] = values[c];
        }
    }
}

template <class T>
void Servo24<T>::send()
{
    char data[] = {
        255,            // Header
        1,              // Mode : 15 values
        _values[0],     // First servo
        _values[1],     // Etc...
        _values[2],
        _values[3],
        _values[4],
        _values[5],
        _values[6],
        _values[7],
        _values[8],
        _values[9],
        _values[10],
        _values[11],
        _values[12],
        _values[13],
        _values[14],
    };

    command(data, 2);
}
#endif


#endif
