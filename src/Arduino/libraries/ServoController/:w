


#ifndef Sequencer_h
#define Sequencer_h

#include <servocontroller.h>

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

typedef enum Status {
    STATUS_OK,
    STATUS_ABORT
};

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
    Serial.println(start);
    Serial.println(end);
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

#endif

