
//#include <HardwareSerial.h>
#include <bleuette.h>
#include <servocontroller.h>

extern HardwareSerial Serial1;

Bleuette bleuette;

motion_t motion_walk[] = {
/*
    {
        3000,
        SERVO_0 | SERVO_1 | SERVO_2,
        3,
        { 10, 200, 10 },
    },
    {
        3000,
        SERVO_0 | SERVO_1 | SERVO_2,
        3,
        { 200, 10, 200 },
    }
*/
    {
        1000,
        SERVO_0 | SERVO_1 | SERVO_2 | SERVO_3 | SERVO_4 | SERVO_5 | SERVO_6 | SERVO_7 | SERVO_8 | SERVO_9 | SERVO_10 | SERVO_11 | SERVO_12 | SERVO_13,
        14,
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    },
    {
        1000,
        SERVO_0 | SERVO_1 | SERVO_2 | SERVO_3 | SERVO_4 | SERVO_5 | SERVO_6 | SERVO_7 | SERVO_8 | SERVO_9 | SERVO_10 | SERVO_11 | SERVO_12 | SERVO_13,
        14,
        { 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230 },
    }
};

sequence_t sequence_walk = {
    true,
    sizeof(motion_walk) / sizeof(motion_walk[0]),
    motion_walk
};

void setup()
{
    //Serial.begin(9600);

    bleuette = Bleuette();
    bleuette.init();

Timer1init();
}

void Timer1init() {

    // Setup Timer2 overflow to fire every 8ms (125Hz)
    //   period [sec] = (1 / f_clock [sec]) * prescale * (255-count)
    //                  (1/16000000)  * 1024 * (255-130) = .008 sec


    TCCR1B = 0x00;        // Disable Timer2 while we set it up

    TCNT1  = 130;         // Reset Timer Count  (255-130) = execute ev 125-th T/C clock
    TIFR1  = 0x00;        // Timer2 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK1 = 0x01;        // Timer2 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR1A = 0x00;        // Timer2 Control Reg A: Wave Gen Mode normal
    TCCR1B = 0x07;        // Timer2 Control Reg B: Timer Prescaler set to 1024
}



/*** FUNC

Name:       Timer2 ISR

Function:   Handles the Timer2-overflow interrupt

Description:    Maintains the 7-segment display

Parameters: void

Returns:    void

FUNC ***/

ISR(TIMER2_OVF_vect) {
    static unsigned int led_state = 0; // LED state

    led_state = !led_state;         // toggles the LED state
    if (led_state) {
        bleuette.ledOn(LED1);
    } else {
        bleuette.ledOff(LED1);
    }

    TCNT1 = 130;     // reset timer ct to 130 out of 255
    TIFR1 = 0x00;    // timer2 int flag reg: clear timer overflow flag
};

void loop() {

//    while (!Serial) ;
//    Serial.print("Serial OK\n");
/*
    go();
    delay(1000);
    return;

    bleuette.ledOn(LED0);
    delay(500);
    //bleuette.init();
    Serial1.write(0xFF);
    //Serial1.write(B10101010);
    bleuette.ledOff(LED0);
    delay(500);
    return;
*/

/*
    Serial.print("Start loop !");
    Serial.print("Voltage :");
    Serial.print(bleuette.getVoltage());
    Serial.print("\n");

    Serial.print("Current :");
    Serial.print(bleuette.getCurrent());
    Serial.print("\n");
*/

    while (1) {
        bleuette.ledOn(LED0);
        //delay(1000);
        bleuette._servo.resume();

        bleuette.runSequence(sequence_walk);

        bleuette._servo.pause();
        delay(1000);

        //Serial.print("pause!");
        //delay(1000);
    //    Serial.print("resume!");

        //bleuette._servo.resume();
        //delay(1000);
        //bleuette._servo.resume();
        //bleuette._servo.clear();

        bleuette.ledOff(LED0);
        //delay(1000);
    }
}

void go() {
    unsigned long servos = 0;
    char values[18];
    unsigned char i = 0;

    unsigned char toto = 200;
    memset(&values, 0, sizeof(values));
    //values[0] = 255;
    //values[1] = 'S';
    values[0] = 'A';
    values[1] = 'B';
    values[2] = 'C';
    values[13] = 'Z';
    /*
    values[5] = 0x40;
    values[6] = 0x50;
    values[7] = 0x60;
    values[8] = 0x70;
    values[9] = 0x80;
    values[10] = 0x90;
    values[11] = 0xA0;
    values[12] = 0xB0;
    values[13] = 0xC0;
    values[14] = 0xD0;
    values[15] = 0xE0;
    values[16] = 0xF0;
    values[17] = 0x36;
    */

    while (1) {

        if (i % 2) {
            memset(&values, 1, sizeof(values));
        } else {
            memset(&values, 200, sizeof(values));
        }

        bleuette.ledOn(LED0);
        bleuette._servo.setValues(
            SERVO_0 | SERVO_1 | SERVO_2 | SERVO_8 | SERVO_9 | SERVO_10 | SERVO_13,
            values,
            7
        );

        bleuette._servo.sendValues();
        bleuette.ledOff(LED0);
/*
    for (char i = 0; i < sizeof(values); i++) {
        Serial1.write(values[i]);
        delay(2);
    }

    int o = bleuette._servo.getResponse();
    Serial.write(o);
*/
        delay(1000);
        i++;
    }
}

