/*
    Sequencer - Bleuette shield
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

#ifndef Sequencer_h
#define Sequencer_h

#include <servocontroller.h>

#define P   Serial.print
#define PLN Serial.println

#define SEQUENCER_SERVO_COUNT   SERVO_COUNT

#define __      230
#define PAUSE   231

#define FRONT   235
#define BACK    236

#define DOWN    240
#define UP      241

#define MID     245

#define HMIN    85
#define HMAX    170
#define HMID    HMIN + (HMAX - HMIN)  / 2

#define VMIN    90
#define VMAX    210
#define VMID    VMIN + (VMAX - VMIN)  / 2

const unsigned char servo_limits[][2] = {

    // Horizontal servos
    { HMIN, HMAX },
    { HMAX, HMIN },
    { HMIN, HMAX },
    { HMAX, HMIN },
    { HMIN, HMAX },
    { HMAX, HMIN },

    // Vertical servos
    { VMIN, VMAX },
    { VMAX, VMIN },
    { VMIN, VMAX },
    { VMAX, VMIN },
    { VMAX, VMIN },
    { VMIN, VMAX }

};

typedef struct motion_t {
    unsigned int delay;
    unsigned char pos[SEQUENCER_SERVO_COUNT];
    void (*function)();
};

typedef struct sequence_t {
    char * name;
    unsigned int count;
    motion_t *motion;
};

#define DELAY_MIN 150

class Sequencer
{
public:
    typedef enum Status { STATUS_OK, STATUS_ABORT };
   
private:
    sequence_t      *_sequences[10];
    unsigned char   _sequences_count;
    unsigned char   _sequences_index;

    Status (*_callback)(unsigned int);
    Status play(struct sequence_t, unsigned int);

    unsigned long _delay;
    ServoController _servo;
public:
    Sequencer();
    Sequencer(ServoController);

    inline unsigned char getValue(unsigned char, unsigned char);

    void setCallback(Status (*)(unsigned int));
    void setDelay(unsigned long);

    void add(sequence_t *);

    void push(sequence_t *);
    void clear();

    void start();

    Status forward(struct sequence_t);
    Status forward(struct sequence_t, unsigned int, unsigned int);
    Status backward(struct sequence_t);
    Status backward(struct sequence_t, unsigned int, unsigned int);
};

#endif

