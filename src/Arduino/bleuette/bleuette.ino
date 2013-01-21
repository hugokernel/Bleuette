
#include "sequences.h"
#include <bleuette.h>
#include <servocontroller.h>
#include <SerialCommand.h>

extern HardwareSerial Serial1;

Bleuette bleuette;

extern struct sequence_t sequence_neutral;
extern struct sequence_t sequence_down;
extern struct sequence_t sequence_up;
extern struct sequence_t sequence_walk;
extern struct sequence_t sequence_pump;

SerialCommand sCmd;     // The demo SerialCommand object


void setup()
{
    //Serial.begin(9600);

    bleuette = Bleuette();
    bleuette.init();

    // initialize timer1 
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;

    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
    TCCR1B |= (1 << CS12);    // 256 prescaler 
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts


    // Setup callbacks for SerialCommand commands
    sCmd.addCommand("init",     cmd_init);
    sCmd.addCommand("resume",   cmd_resume);
    sCmd.addCommand("pause",    cmd_pause);
    sCmd.addCommand("set",      cmd_set);
    sCmd.addCommand("seq",      cmd_seq);
    sCmd.setDefaultHandler(unrecognized);
    
    while (!Serial) ;
    Serial.println("Ready");
}

void LED_on() {
    bleuette.ledOn(LED0);
    bleuette.ledOn(LED1);
}

void LED_off() {
    bleuette.ledOff(LED0);
    bleuette.ledOff(LED1);
}


ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
    static unsigned int led_state = 0; // LED state
    static unsigned int current = 0;
    unsigned int ccurrent = 0;

    led_state = !led_state;         // toggles the LED state
    if (led_state) {
        bleuette.ledOn(LED1);
    } else {
        bleuette.ledOff(LED1);
    }

/*
    Serial.print("V:");
    Serial.print(bleuette.getVoltage());
    Serial.print(";");
*/

    ccurrent = bleuette.getCurrent();
    if (ccurrent > current) {
        //Serial.println("CURRENT!");
    }

    current = ccurrent;

    //TCNT1 = 0;     // reset timer ct to 130 out of 255
    //TCNT1 = 34286;            // preload timer
    TCNT1 = 0;            // preload timer
    //TIFR1 = 0x100;    // timer2 int flag reg: clear timer overflow flag
}

char values[18];

void loop() {

 sCmd.readSerial();     // We don't do much, just process serial commands
   return; 

    while (1) {
        bleuette.ledOn(LED0);
        //delay(1000);
        bleuette.servo.resume();

        bleuette.runSequence(sequence_walk);
        //delay(2000);

        //bleuette.servo.pause();
        //delay(2000);

        //Serial.print("pause!");
        //delay(1000);
    //    Serial.print("resume!");

        //bleuette.servo.resume();
        //delay(1000);
        //bleuette.servo.resume();
        //bleuette.servo.clear();

        bleuette.ledOff(LED0);
        //delay(1000);
    }
}

void cmd_init() {
    Serial.println("Init");
    bleuette.init();
}

void cmd_pause() {
    Serial.println("Pause");
    bleuette.servo.pause();
}

void cmd_resume() {
    Serial.println("Resume");
    bleuette.servo.resume();
}

void cmd_set() {
    unsigned int servo, position;
    char *arg;
    unsigned char value[1];

    Serial.println("Set");
    arg = sCmd.next();
    if (arg != NULL) {
        servo = atoi(arg);
    } else {
        Serial.println("No arguments");
        return;
    }

    arg = sCmd.next();
    if (arg != NULL) {
        value[0] = atol(arg);
    } else {
        Serial.println("No second argument");
        return;
    }

    Serial.print("Servo ");
    Serial.print(servo);
    Serial.print(" (");
    Serial.print(bleuette.servo.get(servo));
    Serial.print("), position ");
    Serial.println((int)value[0]);

    bleuette.servo.setValues(
        bleuette.servo.get(servo),
        value,
        1
    );

    bleuette.servo.sendValues();
}

void cmd_seq() {
    char *arg;
    unsigned char seq = 0;
    unsigned int i, count, index = 0;
    sequence_t seqs[] = {
        sequence_neutral,
        sequence_down,
        sequence_up,
        sequence_walk,
        sequence_pump
    };

    Serial.print("Seq ");
    arg = sCmd.next();
    if (arg != NULL) {
        seq = atoi(arg);
    } else {
        Serial.println("No arguments");
        return;
    }

    arg = sCmd.next();
    if (arg != NULL) {
        count = atoi(arg);
    } else {
        Serial.println("No arguments");
        return;
    }

    arg = sCmd.next();
    if (arg != NULL) {
        index = atoi(arg);
    }

    Serial.print(seq);
    Serial.print(", ");
    Serial.print(count);
    Serial.println(" time(s)");

    Serial.println(seqs[seq].name);

    if (index) {
        Serial.print("Index :");
        Serial.println(index);
        bleuette.runLine(seqs[seq], index);
    } else {
        for (i = 0; i < count; i++) {
            bleuette.runSequence(seqs[seq]);
        }
    }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("What?");
}

void go() {
    unsigned long servos = 0;
    unsigned char values[18];
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
        bleuette.servo.setValues(
            SERVO_0 | SERVO_1 | SERVO_2 | SERVO_8 | SERVO_9 | SERVO_10 | SERVO_13,
            values,
            7
        );

        bleuette.servo.sendValues();
        bleuette.ledOff(LED0);
/*
    for (char i = 0; i < sizeof(values); i++) {
        Serial1.write(values[i]);
        delay(2);
    }

    int o = bleuette.servo.getResponse();
    Serial.write(o);
*/
        delay(1000);
        i++;
    }
}

