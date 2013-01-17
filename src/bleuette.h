/*
    Bleuette.h Control Bleuette shield
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

#ifndef Bleuette_h
#define Bleuette_h

#include <wiring.h>
#include <Arduino.h>

//#include "HardwareSerial.h"
#include <servocontroller.h>

#define BLEUETTE_LED0   8
#define BLEUETTE_LED1   9

typedef struct motion_t {
    unsigned long servos;
    unsigned char count;        // Values count
    char values[14];
    unsigned long delay;
};

typedef struct sequence_t {
    bool loopable;
    unsigned int count;
    motion_t *motion;
};

class Bleuette
{
private:
    ServoController _servo;

public:
    Bleuette();

    void init(void);

    void ledOn(unsigned char);
    void ledOff(unsigned char);

    bool getButtonState(unsigned char);

    int getVoltage();
    int getCurrent();

    // Walk
    bool runSequence(struct sequence_t);
};

Bleuette::Bleuette()
{
    pinMode(BLEUETTE_LED0, OUTPUT);
    pinMode(BLEUETTE_LED1, OUTPUT);
}

void Bleuette::init(void)
{
    _servo = ServoController();
}

/**
 *  Led on !
 */
void Bleuette::ledOn(unsigned char led)
{
    digitalWrite(led, HIGH);
}

/**
 *  Led off !
 */
void Bleuette::ledOff(unsigned char led)
{
    digitalWrite(led, LOW);
}

/**
 *  Run walk sequence
 */
bool Bleuette::runSequence(struct sequence_t seq) {

/*
    if (_serial.available()) {
        return false;
    }
*/
    for (unsigned int i = 0; i < seq.count; i++) {

        _servo.setValues(seq.motion[i].servos, seq.motion[i].values);
        _servo.sendValues();

        delay(seq.motion[i].delay);
    }
}

#endif
