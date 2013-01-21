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

#define BLEUETTE_BUTTON0    10
#define BLEUETTE_BUTTON1    11

#define LED0    BLEUETTE_LED0
#define LED1    BLEUETTE_LED1
#define BUTTON0 BLEUETTE_BUTTON0
#define BUTTON1 BLEUETTE_BUTTON1

typedef struct motion_t {
    unsigned long delay;        // How much time
    unsigned long servos;       // Servo bits field
    unsigned char count;        // Values count
    unsigned char values[14];            // All values for each servo
};

typedef struct sequence_t {
    bool loopable;              // Sequence is loopable ?
    unsigned int count;         // Motion count
    motion_t *motion;           // Motion struct
    char * name;
};

class Bleuette
{
private:

public:
    ServoController servo;
    Bleuette();

    void init(void);

    void ledOn(unsigned char);
    void ledOff(unsigned char);

    bool getButtonState(unsigned char);

    int getVoltage();
    int getCurrent();

    // Walk
    bool runLine(struct sequence_t, unsigned int);
    bool runSequence(struct sequence_t);
};

Bleuette::Bleuette()
{
    pinMode(BLEUETTE_LED0, OUTPUT);
    pinMode(BLEUETTE_LED1, OUTPUT);
}

void Bleuette::init(void)
{
    servo = ServoController();
    servo.init();
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
 *  Get button status
 */
bool Bleuette::getButtonState(unsigned char button)
{
    return digitalRead(button);
}

/**
 *  Get voltage
 */
int Bleuette::getVoltage()
{
    return analogRead(0);
}

/**
 *  Get voltage
 */
int Bleuette::getCurrent()
{
    return analogRead(1);
}

bool Bleuette::runLine(struct sequence_t seq, unsigned int i)
{
    servo.setValues(seq.motion[i].servos, seq.motion[i].values, seq.motion[i].count);
    servo.sendValues();
    delay(seq.motion[i].delay);
}

/**
 *  Run walk sequence
 */
bool Bleuette::runSequence(struct sequence_t seq) {
    for (unsigned int i = 0; i < seq.count; i++) {
        runLine(seq, i);
    }
}

#endif
