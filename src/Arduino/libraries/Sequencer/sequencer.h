
#ifndef Sequencer_h
#define Sequencer_h

#include <servocontroller.h>

#define SEQUENCER_SERVO_COUNT   SERVO_COUNT

#define __      230
#define PAUSE   231

#define FRONT   235
#define BACK    236

#define UP      240
#define DOWN    241

#define TESTO(a, b, c, d, e, f, g, h, i, j, k, l) { a, b, c, d, e, f, g, h, i, j, k,  l },

const unsigned char servo_limits[][2] = {

    // Horizontal servos
    { 85,   170 },
    { 170,  85 },
    { 85,   170 },
    { 170,  85 },
    { 85,   170 },
    { 170,  85 },

    // Vertical servos
    { 90,   210 },
    { 210,  90 },
    { 90,   210 },
    { 210,  90 },
    { 90,   210 },
    { 210,  90 }

};

typedef struct motionr_t {
    unsigned int delay;
    unsigned char pos[SEQUENCER_SERVO_COUNT];
    //void (*function)();
};

typedef struct sequencer_t {
    char * name;
    unsigned int count;
    motionr_t *motion;
};

motionr_t motion_up[] = {
    {
        0,
        {
            __,     __,     __,     __,     __,     __,
            UP,     UP,     UP,     UP,     UP,     UP
        }
    }
};

sequencer_t seq_up = {
    "Leg up !",
    sizeof(motion_up) / sizeof(motion_up[0]),
    motion_up
};

motionr_t motion_down[] = {
    {
        0,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
        }
    }
};

sequencer_t seq_down = {
    "Leg down !",
    sizeof(motion_down) / sizeof(motion_down[0]),
    motion_down
};

/*
motionr_t motion_pumpr[] = {
    {
        0,
        {
            __,     __,     __,     __,     __,     __,
            UP,     UP,     UP,     UP,     UP,     UP
        }
    },
    {
        0,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
        }
    }
};

sequencer_t seq_pumpr = {
    "Pump !",
    sizeof(motion_pumpr) / sizeof(motion_pumpr[0]),
    motion_pumpr
};
*/

sequencer_t s_test[] = {
    seq_down,
    seq_up,
    seq_down,
    seq_up
};

sequencer_t sequences[] = {
    seq_down,
    seq_up
//    sequence_pump
};

class Sequencer
{
private:
    ServoController _servo;

    sequencer_t     *_sequences;
    unsigned char   _sequences_count;
    unsigned char   _sequences_index;
public:
    Sequencer();
    Sequencer(ServoController);

    inline unsigned char getValue(unsigned char, unsigned char);

    void add(sequencer_t *);

    void push(sequencer_t &);
    void clear();

    void start();

    void run(struct sequencer_t);
};

Sequencer::Sequencer() {
    memset(&_sequences, 0, sizeof(_sequences));
    _sequences_count = 0;
    _sequences_index = 0;
}

Sequencer::Sequencer(ServoController servo) :
    _servo(servo)
{
    //memset(&_sequences, 0, sizeof(_sequences));
    _sequences_count = 0;
    _sequences_index = 0;
}

/*
bool Sequencer::runLine(struct sequencer_t seq, unsigned int i, unsigned int timeout)
{
    servo.setValues(seq.motion[i].servos, seq.motion[i].values, seq.motion[i].count);
    servo.sendValues();
    delay(seq.motion[i].delay + timeout);
}
*/

inline unsigned char Sequencer::getValue(unsigned char servo, unsigned char pos) {
    switch (pos) {
        case FRONT:
        case UP:
            return servo_limits[servo][0];
        case BACK:
        case DOWN:
            return servo_limits[servo][1];
    }
}

void Sequencer::add(sequencer_t *seq) {
    _sequences = seq;
}

void Sequencer::push(sequencer_t &seq) {

    //char * args = (char *)malloc(size + 3);
    _sequences[_sequences_count++] = seq;

    if (_sequences_count > sizeof(_sequences)) {
        _sequences_count = 0;
    }
}

void Sequencer::clear() {
    _sequences_count = 0;
    _sequences_index = 0;
}

void Sequencer::start() {
    for (unsigned int i = 0; i < _sequences_count; i++) {
        run(_sequences[i++]);
    }
}

void Sequencer::run(struct sequencer_t seq) {

    for (unsigned int index = 1; index < seq.count; index++) {
        // ToDo: Calculate delay if struct.delay = 0 with current value

        for (unsigned char i = 1; i < SEQUENCER_SERVO_COUNT; i++) {
            if (seq.motion[index].pos[i] != __) {
                _servo.setValue(i, getValue(i, seq.motion[index].pos[i]));
            }
        }

        _servo.sendValues();
        delay(seq.motion[index].delay);
    }
}

/*
bool Bleuette::runSequenceR(struct sequence_t seq, unsigned int timeout) {
    for (unsigned int i = 0; i < SEQUENCER_SERVO_COUNT; i++) {
        runLine(seq, i, timeout);
    }
}
*/

#endif

