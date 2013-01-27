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

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

//#include "HardwareSerial.h"
#include <sequencer.h>
#include <servocontroller.h>

#define BLEUETTE_SERVO_COUNT    SERVO_COUNT

#define BLEUETTE_LED0   8
#define BLEUETTE_LED1   9

#define BLEUETTE VOLTAGE    0
#define BLEUETTE_CURRENT    1

#define BLEUETTE_BUTTON0    10
#define BLEUETTE_BUTTON1    11

#define LED0    BLEUETTE_LED0
#define LED1    BLEUETTE_LED1
#define BUTTON0 BLEUETTE_BUTTON0
#define BUTTON1 BLEUETTE_BUTTON1

// Uout = R2 / (R1 + R2) x Alim
// Alim = R2 / (R1 + R2) x Uout
#define R1  3300
#define R2  10000
#define MAX_VOLTAGE 7.4
#define VOLTAGE(voltage) R2 / (R1 + R2) * MAX_VOLTAGE / (1024 / voltage) 

/*
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
*/

class Bleuette
{
private:

public:
    ServoController servo;
    Sequencer sequencer;

    Bleuette();

    void init(void);

    void ledOn(unsigned char);
    void ledOff(unsigned char);
    void ledToggle(unsigned char);

    bool getButtonState(unsigned char);

    unsigned int getVoltage();
    unsigned int getCurrent();

    // Walk
    /*
    bool runLine(struct sequence_t, unsigned int, unsigned int);
    bool runSequence(struct sequence_t);
    bool runSequence(struct sequence_t, unsigned int);
    bool runSequenceR(struct sequence_t, unsigned int);
    */
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

    sequencer = Sequencer(servo);
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

void Bleuette::ledToggle(unsigned char led)
{
    if (digitalRead(led)) {
        ledOff(led);
    } else {
        ledOn(led);
    }
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
unsigned int Bleuette::getVoltage()
{
    /*
    static double x;
    static double k = 1;
    double measure = analogRead(0);
    x = x + k * (measure - x);
    return x;
    */
    return analogRead(0);
}

/**
 *  Get voltage
 */
unsigned int Bleuette::getCurrent()
{
    return analogRead(BLEUETTE_CURRENT);
}

/*
bool Bleuette::runLine(struct sequence_t seq, unsigned int i, unsigned int timeout)
{
    servo.setValues(seq.motion[i].servos, seq.motion[i].values, seq.motion[i].count);
    servo.sendValues();
    delay(seq.motion[i].delay + timeout);
}
bool Bleuette::runSequence(struct sequence_t seq) {
    for (unsigned int i = 0; i < seq.count; i++) {
        runLine(seq, i, 0);
    }
}

bool Bleuette::runSequence(struct sequence_t seq, unsigned int timeout) {
    for (unsigned int i = 0; i < seq.count; i++) {
        runLine(seq, i, timeout);
    }
}
 */

#endif
