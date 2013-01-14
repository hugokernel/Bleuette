
#include <HardwareSerial.h>
#include <servo24.h>

extern HardwareSerial Serial1;

unsigned long servos = 0;
char values[16];

typedef struct motion_t {
    unsigned long servos;
    unsigned char count;        // Values count
    char values[14];
    unsigned long delay;
};

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
  

int runSequence(struct sequence_t seq) {

    Servo24 servo = Servo24(Serial1, 1);
    for (unsigned int i = 0; i < seq.count; i++) {

        servo.setValues(seq.motion[i].servos, seq.motion[i].values);
        
    servo.send();
        delay(seq.motion[i].delay);
    }

}

int led0 = 8;
int led1 = 9;

//char values[3];


char toto = 0;

void setup()
{
    pinMode(led0, OUTPUT);
    pinMode(led1, OUTPUT);

    memset(values, 0x000, sizeof(values));

    Servo24 servo = Servo24(Serial1, 1);
    //Serial1.begin(9600);
}

void loop() {

    digitalWrite(led0, HIGH);   // sets the LED on
    digitalWrite(led1, HIGH);   // sets the LED on
    //Serial.println("Paf!");

    //while (!Serial1) ;
    //Serial1.begin(9600);
   // Serial1.println("Coucou!");

    //runSequence(sequence_walk);
/*
    //servos = SERVO24_0 & SERVO24_1 & SERVO24_2 & SERVO24_12 & SERVO24_13;
    toto = 200;
    values[0] = 255;
    values[1] = 1;
    values[2] = toto;
    values[3] = toto;
    values[4] = toto;
    values[5] = toto;
    values[6] = toto;
    values[7] = toto;
    values[8] = toto;
    values[9] = toto;
    values[10] = toto;
    values[11] = toto;
    values[12] = toto;
    values[13] = toto;
    values[14] = toto;
    values[15] = toto;

    servo.setValues(0xFFFF, values);
    servo.send();

    delay(1000);
    //servos = SERVO24_0 & SERVO24_1 & SERVO24_2;
    //servos = SERVO24_0 & SERVO24_1 & SERVO24_2 & SERVO24_12 & SERVO24_13;
    toto = 10;
    values[0] = 255;
    values[1] = 1;
    values[2] = toto;
    values[3] = toto;
    values[4] = toto;
    values[5] = toto;
    values[6] = toto;
    values[7] = toto;
    values[8] = toto;
    values[9] = toto;
    values[10] = toto;
    values[11] = toto;
    values[12] = toto;
    values[13] = toto;
    values[14] = toto;
    values[15] = toto;

    servo.setValues(0xFFFF, values);
    servo.send();

    delay(1000);
    */

    toto = 200;
    values[0] = 255;
    values[1] = 1;
    values[2] = toto;
    values[3] = toto;
    values[4] = toto;
    values[5] = toto;
    values[6] = toto;
    values[7] = toto;
    values[8] = toto;
    values[9] = toto;
    values[10] = toto;
    values[11] = toto;
    values[12] = toto;
    values[13] = toto;
    values[14] = toto;
    values[15] = toto;

    for (char i = 0; i < sizeof(values); i++) {
        Serial1.write(values[i]);
        delay(2);
    }

    delay(400);

    toto = 10;
    values[0] = 255;
    values[1] = 1;
    values[2] = toto;
    values[3] = toto;
    values[4] = toto;
    values[5] = toto;
    values[6] = toto;
    values[7] = toto;
    values[8] = toto;
    values[9] = toto;
    values[10] = toto;
    values[11] = toto;
    values[12] = toto;
    values[13] = toto;
    values[14] = toto;
    values[15] = toto;

    for (char i = 0; i < sizeof(values); i++) {
        Serial1.write(values[i]);
        delay(2);
    }

    //toto++;
    //servo.setValues(servos, values);
    //servo.send();

    digitalWrite(led0, LOW);    // sets the LED off
    digitalWrite(led1, LOW);    // sets the LED off

    delay(400);
}

/*
int main() {



    while (1) {

        //runSequence(sequence_walk);

 

/*
        servos = SERVO24_0 & SERVO24_1 & SERVO24_2;
        values = {
            10, 60, 10
        };
*-/
        //servo.setValues(servos, values);
        //servo.send();

        //delay(3000);
    };
}
*/

