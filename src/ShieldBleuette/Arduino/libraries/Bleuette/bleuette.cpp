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

#include "SerialCommand.h"

Bleuette::Bleuette()
{
    pinMode(BLEUETTE_LED0, OUTPUT);
    pinMode(BLEUETTE_LED1, OUTPUT);

    // Foot sensor
    pinMode(BLEUETTE_FOOT_SENSOR_ADDR_A, OUTPUT);
    pinMode(BLEUETTE_FOOT_SENSOR_ADDR_B, OUTPUT);
    pinMode(BLEUETTE_FOOT_SENSOR_ADDR_C, OUTPUT);
    pinMode(BLEUETTE_FOOT_SENSOR_OUT,    INPUT);
}

void Bleuette::init(void)
{
    servo = ServoController();
    servo.init();

    sequencer = Sequencer(servo);
}

/**
 *  Monitor current and voltage
 */
void Bleuette::monitor() {
    unsigned char current = 0;

    ledToggle(LED1);

/*
    P((double)COEFF);
    P("; ");
    P(cvoltage);
    P(", value : ");
    P((double)cvoltage * ADC_STEP); 
    P("; ");
    PLN((double)cvoltage * ADC_STEP / COEFF); 
    if (ccurrent > current + 10) {
//        echo("! V=%i (%i); I=%i\n", cvoltage, R2 / (R1 + R2) * MAX_VOLTAGE / (1024 / cvoltage), ccurrent);

        //P("V=");P(cvoltage);P("; I=");PLN(ccurrent);
    }
*/

/*
    current = getCurrent();
    //if (current > 2.0) {
        P("Voltage: ");
        P(getVoltage());
        P("; Current: ");
        PLN((double)current);
    //}
*/

//    P(getCurrentPercent());
 //   PLN("%");

    current = getCurrentPercent();
    if (current > 80) {
        P("Current limit reached : ");
        P(current);
        PLN("% !");
    }

    if (current > 90) {
        PLN("Current protection on !");
        servo.pause();
    }
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

inline unsigned int Bleuette::readADC(unsigned char pin)
{
    return analogRead(pin);
}

/**
 *  Get voltage
 */
double Bleuette::getVoltage()
{
    /*
    static double x;
    static double k = 1;
    double measure = analogRead(0);
    x = x + k * (measure - x);
    return x;
    */
    //return analogRead(BLEUETTE_VOLTAGE);
    unsigned int voltage = readADC(BLEUETTE_VOLTAGE);
    return VOLTAGE(voltage);
}

/**
 *  Get current
 */
double Bleuette::getCurrent()
{
    unsigned long total = 0;
    unsigned int current, max = 0;

    for (int i = 0; i < 100; i++) {
        current = readADC(BLEUETTE_CURRENT);
        total += current;
        if (current > max) {
            max = current;
        }
    }

/*
    // Todo: calculate exact value !
    P("Current:");
    P((total / 100));
    P("; Max:");
    PLN(max);
*/
// MAXTESA+ Gain : 20

    return (double)(total / 100) * 0.0122;//CURRENT_COEFF;
}

unsigned char Bleuette::getCurrentPercent()
{
    unsigned long total = 0;
    unsigned int current, max = 0;

    for (int i = 0; i < 100; i++) {
        current = readADC(BLEUETTE_CURRENT);
        total += current;
        if (current > max) {
            max = current;
        }
    }

/*
    P("; Current:");
    P((total / 100));
    P("; Max:");
    PLN(max);
*/
    // CURRENT_SENSE_MAX / CURRENT_MAX = 2
    return min((total / 100) / 2, 100);
}

unsigned char Bleuette::footSensorRead(unsigned char sensor)
{
    digitalWrite(BLEUETTE_FOOT_SENSOR_ADDR_A, sensor & 0);
    digitalWrite(BLEUETTE_FOOT_SENSOR_ADDR_B, sensor & 1);
    digitalWrite(BLEUETTE_FOOT_SENSOR_ADDR_C, sensor & 2);

    return digitalRead(BLEUETTE_FOOT_SENSOR_OUT);
}

unsigned char Bleuette::feetSensorRead()
{
    unsigned char index, out = 0;

    for (index = 0; index < 6; index++) {
        if (footSensorRead(index)) {
            out |= 1 << index;
        }
    }

    return out;
}

