
//#include <HardwareSerial.h>
#include <bleuette.h>
#include <servocontroller.h>

extern HardwareSerial Serial1;

unsigned long servos = 0;
char values[16];

motion_t motion_walk[] = {
    {
        SERVO_0 & SERVO_1 & SERVO_2,
        3,
        { 10, 60, 10 },
        3000
    },
    {
        SERVO_0 & SERVO_1 & SERVO_2,
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
  
/*
int runSequence(struct sequence_t seq) {

    ServoController servo = ServoController();
    for (unsigned int i = 0; i < seq.count; i++) {

        servo.setValues(seq.motion[i].servos, seq.motion[i].values);
        servo.send();

        delay(seq.motion[i].delay);
    }
}

int led0 = 8;
int led1 = 9;

//char values[3];
*/

char toto = 0;

void setup()
{
    /*
    pinMode(led0, OUTPUT);
    pinMode(led1, OUTPUT);
    */
    memset(values, 0x000, sizeof(values));

    //ServoController servo = ServoController();
    //Serial1.begin(9600);
}

void loop() {

    //Serial.println("Paf!");

    //while (!Serial1) ;
    //Serial1.begin(9600);
   // Serial1.println("Coucou!");

    //runSequence(sequence_walk);
/*
    //servos = SERVO_0 & SERVO_1 & SERVO_2 & SERVO_12 & SERVO_13;
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
    //servos = SERVO_0 & SERVO_1 & SERVO_2;
    //servos = SERVO_0 & SERVO_1 & SERVO_2 & SERVO_12 & SERVO_13;
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

    delay(400);
}

/*
int main() {



    while (1) {

        //runSequence(sequence_walk);

 

/*
        servos = SERVO_0 & SERVO_1 & SERVO_2;
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

