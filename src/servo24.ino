#include <servo24.h>

//#include <NewSoftSerial.h>
#include <HardwareSerial.h>

extern HardwareSerial Serial;

unsigned long servos = 0;
unsigned char values[14];

typedef struct sequence_t {
    unsigned long servos;
    unsigned char count;        // Values count
    unsigned char values[14];
} sequence;

sequence_t seq_walk[2] = {
    {
        SERVO24_0 & SERVO24_1 & SERVO24_2,
        3,
        10, 60, 10
    },
    {
        SERVO24_0 & SERVO24_1 & SERVO24_2,
        3,
        60, 10, 60
    },
};

int main() {
  
    Servo24Hard servo = Servo24Hard(Serial, 1);

    while (1) {

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

