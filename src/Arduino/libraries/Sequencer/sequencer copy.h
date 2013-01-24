
#ifndef Sequencer_h
#define Sequencer_h

#include <bleuette.h>
#include <servocontroller.h>

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

#define SEQUENCER_SERVO_COUNT   SERVO_COUNT

enum SEQ_POSITION {
    __,
    FRONT,
    BACK,
    UP,
    DOWN
};

typedef struct motionr_t {
    SEQ_POSITION pos[SEQUENCER_SERVO_COUNT];
};

typedef struct sequencer_t {
    char * name;
    unsigned int count;
    motionr_t *motion;
};

motionr_t motion_pumpr[] = {
    {
        __,     __,     __,     __,     __,     __,
        UP,     UP,     UP,     UP,     UP,     UP
    },
    {
        __,     __,     __,     __,     __,     __,
        DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
    }
};

sequencer_t seq_pumpr[] = {
    "Pump !",
    sizeof(motion_pumpr),
    motion_pumpr
};

class Sequencer
{
private:
    ServoController _servo;
public:
    Sequencer(ServoController);

    unsigned char getValue(unsigned char, SEQ_POSITION);

    // Walk
    bool runLine(struct sequencer_t, unsigned int, unsigned int);
    void run(struct sequencer_t, unsigned int);
};

Sequencer::Sequencer(ServoController servo) :
    _servo(servo)
{

}

unsigned char servo_values[][2] = {

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

/*
bool Sequencer::runLine(struct sequencer_t seq, unsigned int i, unsigned int timeout)
{
    servo.setValues(seq.motion[i].servos, seq.motion[i].values, seq.motion[i].count);
    servo.sendValues();
    delay(seq.motion[i].delay + timeout);
}
*/

unsigned char Sequencer::getValue(unsigned char servo, SEQ_POSITION pos) {
    switch (pos) {
        case __:    break;
        case FRONT:
            return servo_values[servo][0];
        case BACK:
            return servo_values[servo][1];
        case UP:
            return servo_values[servo][0];
        case DOWN:
            return servo_values[servo][1];
    }
}

void Sequencer::run(struct sequencer_t seq, unsigned int count) {
    unsigned long servos;
    unsigned char values[SEQUENCER_SERVO_COUNT];

    for (unsigned int index = 0; index < seq.count; index++) {
        for (unsigned char i = 0; i < SEQUENCER_SERVO_COUNT; i++) {
            //_servo.setValue(i, getValue(i, seq.motion[i]));
            getValue(i, seq.motion[index][i]);
        }
    }


}

#endif

