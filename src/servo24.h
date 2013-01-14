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
#include <Arduino.h>

#include "HardwareSerial.h"

#define CHECK_BIT(var, pos) (var & (1 << pos))

#define SERVO24_COUNT   14

#define SERVO24_ACK    'O'
#define SERVO24_NACK   'N'

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

class Servo24
{
private:
    HardwareSerial _serial;
    unsigned char _mode;
    int _last_status_code;

    unsigned char _values[14];

public:
    Servo24(HardwareSerial &, unsigned char);

    void setValues(unsigned long, char *);
    void send();

    bool getLastStatus();
    int getResponse();
    bool command(unsigned char [], unsigned char);

};

Servo24::Servo24(HardwareSerial &serial, unsigned char mode) :
    _serial(serial),
    _mode(mode)
{
    memset(&_values, 0x00, sizeof(_values));
    _last_status_code = 0;
    serial.begin(9600);
};

int Servo24::getResponse()
{
    while (!_serial.available());
    return _serial.read();
}

bool Servo24::getLastStatus()
{
    return (_last_status_code == SERVO24_ACK) ? true : false;
}

bool Servo24::command(unsigned char data[], unsigned char size)
{
    unsigned char i = 0;
    unsigned long c = 1;

    for (i = 0; i < size; i++) {
        _serial.write(data[i]);
        delay(2);

        Serial.print(data[i], DEC);
        Serial.print('\n');
    }

    _last_status_code = getResponse();

    Serial.print(_last_status_code);

    return getLastStatus();
};

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
void Servo24::setValues(unsigned long servos, char * values)
{
    unsigned char b = 0;
    for (b = 0; b < SERVO24_COUNT; b++) {
        if (CHECK_BIT(servos, b)) {
            _values[b] = values[b];
        }
    }
}

void Servo24::send()
{
    unsigned char i = 0;

    unsigned char data[] = {
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
        0
    };

    // Control byte
    for (i = 2; i < sizeof(data) - 1; i++) {
        data[sizeof(data) - 1] += data[i];
    }

    Serial.write("Control byte :");
    Serial.print(data[sizeof(data) - 1]);
    Serial.print('\n');

    command(data, sizeof(data));
}

#endif
