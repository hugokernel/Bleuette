#include <servo24.h>

//#include <NewSoftSerial.h>
#include <HardwareSerial.h>

extern HardwareSerial Serial;

unsigned long servos = 0;
unsigned char values[14];

typedef struct motion_t {
    unsigned long servos;
    unsigned char count;        // Values count
    unsigned char values[14];
    unsigned long delay;
}; // sequence;

typedef struct sequence_t {
    bool loopable;
    unsigned int count;
    motion_t *motion;
};

motion_t motion_walk[] = {
    {
        SERVO24_0 & SERVO24_1 & SERVO24_2,
        3,
        { 10, 60, 10 },
        3000
    },
    {
        SERVO24_0 & SERVO24_1 & SERVO24_2,
        3,
        { 60, 10, 60 },
        3000
    }
};

sequence_t sequence_walk = {
    true,
    sizeof(motion_walk),
    motion_walk
};


void runSequence(sequence_t seq) {

}

int main() {
  
    Servo24Hard servo = Servo24Hard(Serial, 1);

    while (1) {

        //runSequence(sequence_walk);

/*
        servos = SERVO24_0 & SERVO24_1 & SERVO24_2;
        values = {
            10, 60, 10
        };
*/
        //servo.setValues(servos, values);
        servo.send();

        delay(3000);
    };
}

