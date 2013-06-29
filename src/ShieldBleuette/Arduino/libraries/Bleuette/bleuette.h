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

#define BLEUETTE_VOLTAGE    0
#define BLEUETTE_CURRENT    1

#define BLEUETTE_BUTTON0    10
#define BLEUETTE_BUTTON1    11

#define LED0    BLEUETTE_LED0
#define LED1    BLEUETTE_LED1
#define BUTTON0 BLEUETTE_BUTTON0
#define BUTTON1 BLEUETTE_BUTTON1

// Uout = R2 / (R1 + R2) x Alim
// Alim = R2 / (R1 + R2) x Uout
#define R1  10000
#define R2  10000

#define COEFF                   0.5     // (R2 / (R1 + R2))
#define VOLTAGE_LIPO_MAX        7.4
#define VOLTAGE_UC_MAX          5       // (Volt)
#define ADC_STEP                (double)VOLTAGE_UC_MAX / 1024

#define CURRENT_FULL_SCALE      2       // Current sensor / rsense configuration : 5A (Volt)
#define CURRENT_MAX             2.5     // Bleuette max current (Ampere)

#define CURRENT_ADC_MAX_VALUE   (double)1024 / VOLTAGE_UC_MAX * CURRENT_FULL_SCALE // MAX4173TESA : (full scale : 0 -> 2V max)
#define CURRENT_SENSE_MAX       5       // (Ampere)

#define CURRENT_COEFF           0.0122 //(double)CURRENT_SENSE_MAX / CURRENT_ADC_MAX_VALUE

#define VOLTAGE(value)          (double)value * ADC_STEP / COEFF 

/**
 *  Foot sensor
 */
#define BLEUETTE_FOOT_SENSOR_ADDR_A 2
#define BLEUETTE_FOOT_SENSOR_ADDR_B 3
#define BLEUETTE_FOOT_SENSOR_ADDR_C 4

#define BLEUETTE_FOOT_SENSOR_OUT    5

/*
#define BLEUETTE_FOOT_SENSOR_0  0
#define BLEUETTE_FOOT_SENSOR_1  1
#define BLEUETTE_FOOT_SENSOR_2  2
#define BLEUETTE_FOOT_SENSOR_3  3
#define BLEUETTE_FOOT_SENSOR_4  4
#define BLEUETTE_FOOT_SENSOR_5  5
*/

enum BLEUETTE_FOOT_SENSOR {
    BLEUETTE_FOOT_SENSOR_0,
    BLEUETTE_FOOT_SENSOR_1,
    BLEUETTE_FOOT_SENSOR_2,
    BLEUETTE_FOOT_SENSOR_3,
    BLEUETTE_FOOT_SENSOR_4,
    BLEUETTE_FOOT_SENSOR_5
};

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
//private:


public:
    ServoController servo;
    Sequencer sequencer;

    Bleuette();

    void init(void);

    void monitor();

    void ledOn(unsigned char);
    void ledOff(unsigned char);
    void ledToggle(unsigned char);

    bool getButtonState(unsigned char);

    unsigned int readADC(unsigned char);
    unsigned int readCurrent();

    double getVoltage();
    double getCurrent();
    unsigned char getCurrentPercent();

    // Foot sensor read
    unsigned char footSensorRead(unsigned char);
    unsigned char feetSensorRead();
};

#endif
