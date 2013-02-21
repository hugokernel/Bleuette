
#define P   Serial.print
#define PLN Serial.println

#include <bleuette.h>
#include <sequencer.h>
#include "sequences.h"
#include <servocontroller.h>
#include <SerialCommand.h>

//#include <Streaming.h>

extern HardwareSerial Serial1;

Bleuette bleuette;

SerialCommand sCmd;

#define ARG_ERROR   PLN("Argument error !"); return;

/*
#include <stdarg.h>
#include <stdio.h>

void echo(char *fmt, ... ) {
    char tmp[128];
    va_list args;
    va_start (args, fmt );
    vsnprintf(tmp, 128, fmt, args);
    va_end (args);
    Serial.print(tmp);
}
*/

/*
void echo(char *str) {
    Serial.print(str);
}
//#define echo println
*/

/*
void println(char *str) {
    PLN(str);
}
*/


Status waitSerial(unsigned int i) {
    P("Waiting for serial, sequence ");
    P(i);
    P(" ...");
    while (Serial.read() != '\n');
    PLN("[ok]");

    return STATUS_OK;
}

Status waitButton(unsigned int i) {
    P("Waiting for button, sequence ");
    P(i);
    P(" ...");
    while (!bleuette.getButtonState(BLEUETTE_BUTTON1));
    PLN("[ok]");

    return STATUS_OK;
}

void setup()
{
    Serial.begin(9600);

    bleuette = Bleuette();
    bleuette.init();

    //Sequencer sequencer = Sequencer(bleuette.servo);


    // Setup callbacks for SerialCommand commands
    sCmd.setDefaultHandler(unrecognized);
    sCmd.addCommand("init",     cmd_init);
    sCmd.addCommand("info",     cmd_info);
    sCmd.addCommand("debug",    cmd_debug);

    sCmd.addCommand("resume",   cmd_resume);
    sCmd.addCommand("pause",    cmd_pause);
    sCmd.addCommand("set",      cmd_set);

    // Sequencer
    sCmd.addCommand("seq",      cmd_seq);
    sCmd.addCommand("push",     cmd_push);
    sCmd.addCommand("start",    cmd_start);
    sCmd.addCommand("clear",    cmd_clear);

    sCmd.addCommand("pos",      cmd_pos);

    // initialize timer1
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;

    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts

//    while (!Serial) ;
//    PLN("Ready !");
}

ISR(TIMER1_OVF_vect)
{
    bleuette.monitor();

    //TCNT1 = 0;     // reset timer ct to 130 out of 255
    //TCNT1 = 34286;            // preload timer
    TCNT1 = 0;            // preload timer
    //TIFR1 = 0x100;    // timer2 int flag reg: clear timer overflow flag
}

void loop() {
    bleuette.ledToggle(LED0);
    sCmd.readSerial();
    delay(1);
}

/**
 *  Init
 */
void cmd_init() {
    PLN("Init");
    bleuette.init();
}

/**
 *  Info
 */
void cmd_info() {
    unsigned char index = 0;

    PLN("Info !!");

    P("Voltage : ");
    PLN(bleuette.getVoltage());
    P("Current : ");
    P(bleuette.getCurrentPercent());
    PLN("%");

    P("Button 0 ");
    if (!bleuette.getButtonState(BLEUETTE_BUTTON0)) {
        PLN("pressed !");
    } else {
        PLN("released !");
    }

    P("Button 1 ");
    if (!bleuette.getButtonState(BLEUETTE_BUTTON1)) {
        PLN("pressed !");
    } else {
        PLN("released !");
    }

    PLN("Feet");
    for (index = 0; index < 6; index++) {
        P(index);
        P(":");
        if (bleuette.footSensorRead(index)) {
            P("1");
        } else {
            P("0");
        }
        P(", ");
    }
    PLN();
}

void cmd_debug() {
    static bool debug = false;
    char *arg;

    arg = sCmd.next();
    if (arg == NULL) {
        ARG_ERROR
    }

    debug = atoi(arg);

    if (debug) {
        bleuette.sequencer.setCallback(waitSerial);
        PLN("Debug enabled !");
    } else {
        bleuette.sequencer.setCallback(NULL);
        PLN("Debug disabled !");
    }
}

/**
 *  Pause
 */
void cmd_pause() {
    PLN("Pause");
    bleuette.servo.pause();
}

/**
 *  Resume
 */
void cmd_resume() {
    PLN("Resume");
    bleuette.servo.resume();
}

/**
 *  Set position
 */
void cmd_set() {
    unsigned int servo, position;
    char *arg;
    unsigned char value[1];

    arg = sCmd.next();
    if (arg == NULL) {
        ARG_ERROR
    }

    servo = atoi(arg);

    arg = sCmd.next();
    if (arg == NULL) {
        ARG_ERROR
    }

    value[0] = atol(arg);

    //echo("Servo %i servo (%i) go to position !\n", servo, bleuette.servo.get(servo));//, value[0]);

    P("Set servo : ");
    P(servo);
    P(" (");
    P(bleuette.servo.get(servo));
    P(")");
    P(", position : ");
    PLN(value[0]);


    //P("Before value :");
    //PLN(bleuette.servo.getPosition(servo));

    bleuette.servo.setValues(
        bleuette.servo.get(servo),
        value,
        1
    );

    bleuette.servo.sendValues();

    //P("After value :");
    //PLN(bleuette.servo.getPosition(servo));
}

/**
 *  Run sequence
 */
void cmd_seq() {
    char *arg;
    unsigned char seq = 0;
    unsigned int i, count, timeout = 0;
    unsigned int mode = 1;


    // Get sequence index
    arg = sCmd.next();
    if (arg == NULL) {
        ARG_ERROR
    }

    // Help ?
    if (arg[0] == '?') {
        PLN(F("Syntax : seq {index} {count} {delay} {forward}"));
        PLN("Available sequence :");
        for (i = 0; i < 11; i++) {//sizeof(sequences); i++) {
            P("- ");
            P(i);
            P(' ');
            PLN(sequences[i].name);
        }

        return;
    }

    seq = atoi(arg);
    if (seq < 0 || seq > sizeof(sequences)) {
        PLN("Sequence not found !");
        return;
    }

    // Get count
    arg = sCmd.next();
    if (arg == NULL) {
        count = 1;
    } else {
        count = atoi(arg);
    }

    // Get delay
    arg = sCmd.next();
    if (arg != NULL) {
        timeout = atoi(arg);
    }

    // backward mode ?
    arg = sCmd.next();
    if (arg != NULL) {
        mode = atoi(arg);
    }

    P("Run sequence : ");
    P(seq);
    P(", name : ");
    P(sequences[seq].name);
    P(", count : ");
    P(count);
    P(", timeout : ");
    P(timeout);
    P(", backward : ");

    if (mode) {
        PLN("Forward !");
    } else {
        PLN("Backward !");
    }

    bleuette.sequencer.setDelay(timeout);

    for (i = 0; i < count; i++) {
        if (mode) {
            bleuette.sequencer.forward(sequences[seq]);
        } else {
            bleuette.sequencer.backward(sequences[seq]);
        }
    }
}

/**
 *  Sequence push
 */
void cmd_push() {
    char *arg;
    unsigned char seq = 0;
    unsigned int i, count, delay = 0;

    // Get sequence index
    arg = sCmd.next();
    if (arg == NULL) {
        ARG_ERROR
    }

    seq = atoi(arg);
    if (seq < 0 || seq > sizeof(sequences)) {
        PLN("Sequence not found !");
        return;
    }

    PLN(seq);
    PLN(sequences[seq].name);
    //echo("Run sequence %c (%s) !\n", seq, sequences[seq].name);

    bleuette.sequencer.push(&sequences[seq]);
}

void cmd_pos() {
    PLN("Get current servo position !");

    unsigned char buffer[SERVO_COUNT];

    bleuette.servo.getPositions(*buffer);

    for (int i = 0; i < SERVO_COUNT; i++) {
        P(i);
        P("\t: ");
        PLN(buffer[i]);
    }
}

void cmd_start() {
    PLN("Start !");
    bleuette.sequencer.start();
}

void cmd_clear() {
    PLN("Clear !");
    bleuette.sequencer.clear();
}

void unrecognized(const char *command) {
    PLN("Command not found !");
}

