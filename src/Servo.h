/*
    Servo24.h - Oled 4D System libray for Arduino
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

#define ENABLE_NEW_SOFT_SERIAL

#ifndef Servo24_h
#define Servo24_h

#include <wiring.h>

#include "HardwareSerial.h"
#ifdef ENABLE_NEW_SOFT_SERIAL
#include "NewSoftSerial.h"
#endif

#define SERVO24_MODE_UNIT   0
#define SERVO24_MODE_BLOCK  1
#define SERVO24_MODE_3BLOCK 2

enum SERVO24_LIST {

    SERVO24_SERVO0 = 0,
    SERVO24_SERVO1,
    SERVO24_SERVO2,
    SERVO24_SERVO3,

};

struct Servo24 {
};

template <class T>
class Servo24
{
private:
    T _serial;
    unsigned char mode;

public:
    Servo24(HardwareSerial &);
#ifdef ENABLE_NEW_SOFT_SERIAL
    Servo24(NewSoftSerial &);
#endif

    void init();
    void reset();
    int getResponse();
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
};

template <class T>
Servo24<T>::Servo24(NewSoftSerial &serial) :
    _serial(serial),
    _mode(mode)
{
};

template <class T>
void Servo24<T>::setMode(unsigned char mode) :
    _mode(mode)
{
}

#endif
