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
    SERVO24_MODE_3BLOCK
};

enum SERVO24_LIST {

    SERVO24_SERVO0 = 1,
    SERVO24_SERVO1 = 2,
    SERVO24_SERVO2 = 4,
    SERVO24_SERVO3 = 8,
    SERVO24_SERVO4 = 16,
    SERVO24_SERVO5 = 32,
    SERVO24_SERVO6 = 64,
    SERVO24_SERVO7 = 128,

/*
    SERVO24_SERVO8,
    SERVO24_SERVO9,
    SERVO24_SERVO10,
    SERVO24_SERVO11,
    SERVO24_SERVO12,
    SERVO24_SERVO13,
    SERVO24_SERVO14,
    SERVO24_SERVO15,

    SERVO24_SERVO16,
    SERVO24_SERVO17,
    SERVO24_SERVO18,
    SERVO24_SERVO19,
    SERVO24_SERVO20,
    SERVO24_SERVO21,
    SERVO24_SERVO22,
    SERVO24_SERVO23,
*/
};

//struct Servo24 {
//};

template <class T>
class Servo24
{
private:
    T _serial;
    unsigned char _mode;
    char _limits[24 * 3];

public:
    Servo24(HardwareSerial &, unsigned char);
#ifdef ENABLE_NEW_SOFT_SERIAL
    Servo24(NewSoftSerial &, unsigned char);
#endif

    void setMode(SERVO24_MODE);

    void setLimits(char *);

    void command(char [], unsigned char);

    void block(char, char, char, char, char, char);
    
/*
    void init();
    void reset();
    int getResponse();
*/
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
    // Todo: Set the speed
};

#ifdef ENABLE_NEW_SOFT_SERIAL
template <class T>
Servo24<T>::Servo24(NewSoftSerial &serial, unsigned char mode) :
    _serial(serial),
    _mode(mode)
{
};
#endif

template <class T>
void Servo24<T>::setMode(SERVO24_MODE mode)
{
    _mode = mode;
};

template <class T>
void Servo24<T>::setLimits(char * limits)
{
    memcpy(limits, _limits, sizeof(limits));
}

template <class T>
void Servo24<T>::command(char data[], unsigned char size)
{
    int i = 0;
    _serial.print(255);

    for (; i < size; i++) {
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

#define CHECK_BIT(var, pos) (var & (1 << pos))

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
