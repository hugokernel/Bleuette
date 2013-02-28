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

#include "sequencer.h"

Sequencer::Sequencer() {
    memset(&_sequences, 0, sizeof(_sequences));
    _sequences_count = 0;
    _sequences_index = 0;

    _callback = NULL;
    _delay = 0;
}

Sequencer::Sequencer(ServoController servo) :
    _servo(servo)
{
    //memset(&_sequences, 0, sizeof(_sequences));
    _sequences_count = 0;
    _sequences_index = 0;

    _callback = NULL;
    _delay = 0;
}

/*
bool Sequencer::runLine(struct sequence_t seq, unsigned int i, unsigned int timeout)
{
    servo.setValues(seq.motion[i].servos, seq.motion[i].values, seq.motion[i].count);
    servo.sendValues();
    delay(seq.motion[i].delay + timeout);
}
*/

inline unsigned char Sequencer::getValue(unsigned char servo, unsigned char pos) {
    unsigned char min, max = 0;
    switch (pos) {
        case FRONT:
        case DOWN:
            return servo_limits[servo][0];
        case BACK:
        case UP:
            return servo_limits[servo][1];
        case MID:
            min = min(servo_limits[servo][0], servo_limits[servo][1]);
            max = max(servo_limits[servo][0], servo_limits[servo][1]);

            /*
            P("Min:");
            P(min);
            P("; Max:");
            P(max);
            P("; MID:");
            PLN((max - min) / 2 + min);
            */
            return ((max - min) / 2 + min);
        default:
            min = min(servo_limits[servo][0], servo_limits[servo][1]);
            max = max(servo_limits[servo][0], servo_limits[servo][1]);

            if (pos >= min && pos <= max) {
                return pos;
            }

            P("Min / Max error : ");
            P(pos);
            PLN(" !");
    }

    return 0;
}

void Sequencer::setCallback(Status (*function)(unsigned int)) {
    _callback = function;
}

void Sequencer::setDelay(unsigned long delay) {
    _delay = delay;
}

void Sequencer::add(sequence_t *seq) {
    //_sequences = seq;
}

void Sequencer::push(sequence_t *seq) {

//Serial.print(seq.name);

    //char * args = (char *)malloc(size + 3);
    _sequences[_sequences_count++] = seq;
    //Serial.print(_sequences[_sequences_count].name);

    if (_sequences_count > sizeof(_sequences)) {
        _sequences_count = 0;
    }
}

void Sequencer::clear() {
    _sequences_count = 0;
    _sequences_index = 0;
}

void Sequencer::start() {
    Serial.print("Sequence count :");
    Serial.println(_sequences_count);
    for (unsigned int i = 0; i < _sequences_count; i++) {
        //Serial.print(_sequences[i].name);
        //run(*_sequences[i++]);
    }
}

Status Sequencer::forward(struct sequence_t seq) {
    return forward(seq, 0, seq.count);
}

Status Sequencer::forward(struct sequence_t seq, unsigned int start, unsigned int end) {
    Status status = STATUS_OK;
    for (unsigned int index = start; index < end; index++) {
        status = play(seq, index);
        if (status != STATUS_OK) {
            break;
        }
    }

    return status;
}

Status Sequencer::backward(struct sequence_t seq) {
    return backward(seq, seq.count, 0);
}

Status Sequencer::backward(struct sequence_t seq, unsigned int start, unsigned int end) {
    Status status = STATUS_OK;
    for (unsigned int index = start; index > end; index--) {
        status = play(seq, index - 1);
        if (status != STATUS_OK) {
            break;
        }
    }

    return status;
}

/**
 *  Play one sequence line
 */
Status Sequencer::play(struct sequence_t seq, unsigned int index) {

    Status status = STATUS_OK;

    if (seq.motion[index].function) {
        seq.motion[index].function();
    }

    if (_callback != NULL) {
        status = _callback(index);
    }

    for (unsigned char i = 0; i < SEQUENCER_SERVO_COUNT; i++) {
        if (seq.motion[index].pos[i] != __) {
            _servo.setValue(i, getValue(i, seq.motion[index].pos[i]));
        }
    }

    _servo.sendValues();

    if (!seq.motion[index].delay) {
        delay(DELAY_MIN);
    } else {
        delay(seq.motion[index].delay);
    }

    delay(_delay);

    return status;
}

