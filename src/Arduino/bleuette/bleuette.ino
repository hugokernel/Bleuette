
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
    //Serial.begin(9600);

    bleuette = Bleuette();
    bleuette.init();

    //Sequencer sequencer = Sequencer(bleuette.servo);

    // initialize timer1
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;

    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts


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

    sCmd.addCommand("test",     cmd_test);
    sCmd.addCommand("test2",     cmd_test2);

    while (!Serial) ;
    PLN("Ready !");
}

ISR(TIMER1_OVF_vect)
{
    static unsigned int current, voltage = 0;
    unsigned int ccurrent, cvoltage = 0;

    bleuette.ledToggle(LED1);

    ccurrent = bleuette.readCurrent();
    cvoltage = bleuette.readVoltage();

    if (ccurrent > current + 10) {
//        echo("! V=%i (%i); I=%i\n", cvoltage, R2 / (R1 + R2) * MAX_VOLTAGE / (1024 / cvoltage), ccurrent);

        //P("V=");P(cvoltage);P("; I=");PLN(ccurrent);
    }

    current = ccurrent;
    current = ccurrent;

    //TCNT1 = 0;     // reset timer ct to 130 out of 255
    //TCNT1 = 34286;            // preload timer
    TCNT1 = 0;            // preload timer
    //TIFR1 = 0x100;    // timer2 int flag reg: clear timer overflow flag
}

void loop() {
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
    PLN("Info !!");
    unsigned int current = bleuette.readCurrent();
    unsigned int voltage = bleuette.readVoltage();
    //echo("Voltage=%i (%f); Current=%i\n", voltage, VOLTAGE(voltage), current);

    //PLN(4.5 / (1024 / voltage), 10);

    //echo("Voltage=%i (%i); Current=%i\n", voltage, 4 / (1024 / voltage), current);
    P("V=");
    P(voltage);
    P(" (");
    P(bleuette.getVoltage());
    P("); I=");
    P(current);
    P(" (");
    P(bleuette.getCurrent());
    P(")");

    P("Button 0 ");
    if (!bleuette.getButtonState(BLEUETTE_BUTTON0)) {
        PLN(" pressed !");
    } else {
        PLN(" released !");
    }

    P("Button 1");
    if (!bleuette.getButtonState(BLEUETTE_BUTTON1)) {
        PLN(" pressed !");
    } else {
        PLN(" released !");
    }
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
    bleuette.sequencer._servo.pause();
}

/**
 *  Resume
 */
void cmd_resume() {
    PLN("Resume");
    bleuette.sequencer._servo.resume();
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
    P(bleuette.sequencer._servo.get(servo));
    P(")");
    P(", position : ");
    PLN(value[0]);


    P("Before value :");
    PLN(bleuette.sequencer._servo.getPosition(servo));

    bleuette.sequencer._servo.setValues(
        bleuette.sequencer._servo.get(servo),
        value,
        1
    );

    bleuette.sequencer._servo.sendValues();

    P("After value :");
    PLN(bleuette.sequencer._servo.getPosition(servo));
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

    cmd_test2();
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

void cmd_test() {
    PLN("Test !");

    for (int i = 0; i < SERVO_COUNT; i++) {
        P(i);
        P("\t: ");
        PLN(bleuette.sequencer._servo._values[i]);
    }
}

void cmd_test2() {
    PLN("Test2 !");

    for (int i = 0; i < SERVO_COUNT; i++) {
        P(i);
        P("\t: ");
        PLN(bleuette.sequencer._servo._current_values[i]);
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

