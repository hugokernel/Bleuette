
#include <bleuette.h>
//#include "sequences.h"
#include <sequencer.h>
#include <servocontroller.h>
#include <SerialCommand.h>

extern HardwareSerial Serial1;

Bleuette bleuette;

/*
extern struct sequence_t sequence_neutral;
extern struct sequence_t sequence_down;
extern struct sequence_t sequence_up;
extern struct sequence_t sequence_walk;
extern struct sequence_t sequence_pump;
*/

SerialCommand sCmd;

#define ARG_ERROR   println("Argument error !"); return;

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

/*
void echo(char *str) {
    Serial.print(str);
}
//#define echo println
*/

void println(char *str) {
    Serial.println(str);
}


void waitSerial(unsigned int i) {
    echo("Waiting for serial (%i) ... ", i);
    while (Serial.read() != '\n');
    echo("[ok]\n");

    //if (!bleuette.getButtonState(BLEUETTE_BUTTON1)) {
    //Serial.println("OK");
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

    while (!Serial) ;
    println("Ready !");
}

ISR(TIMER1_OVF_vect)
{
    static unsigned int current, voltage = 0;
    unsigned int ccurrent, cvoltage = 0;

    bleuette.ledToggle(LED1);

    ccurrent = bleuette.getCurrent();
    cvoltage = bleuette.getVoltage();

    if (ccurrent > current + 10) {
        echo("! V=%i (%i); I=%i\n", cvoltage, R2 / (R1 + R2) * MAX_VOLTAGE / (1024 / cvoltage), ccurrent);
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
}

/**
 *  Init
 */
void cmd_init() {
    println("Init");
    bleuette.init();
}

/**
 *  Info
 */
void cmd_info() {
    println("Info");
    unsigned int current = bleuette.getCurrent();
    unsigned int voltage = bleuette.getVoltage();
    //echo("Voltage=%i (%f); Current=%i\n", voltage, VOLTAGE(voltage), current);

    Serial.println(4.5 / (1024 / voltage), 10);

    echo("Voltage=%i (%i); Current=%i\n", voltage, 4 / (1024 / voltage), current);

    if (!bleuette.getButtonState(BLEUETTE_BUTTON0)) {
        echo("Button 0 pressed !\n");
    }

    if (!bleuette.getButtonState(BLEUETTE_BUTTON1)) {
        echo("Button 1 pressed !\n");
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
        println("Debug enabled !\n");
    } else {
        bleuette.sequencer.setCallback(NULL);
        println("Debug disabled !\n");
    }

    //debug =! debug;
}

/**
 *  Pause
 */
void cmd_pause() {
    println("Pause");
    bleuette.servo.pause();
}

/**
 *  Resume
 */
void cmd_resume() {
    println("Resume");
    bleuette.servo.resume();
}

/**
 *  Set position
 */
void cmd_set() {
    unsigned int servo, position;
    char *arg;
    unsigned char value[1];

    println("Set");
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

    echo("Servo %i servo (%i) go to position !\n", servo, bleuette.servo.get(servo), value[0]);

    bleuette.servo.setValues(
        bleuette.servo.get(servo),
        value,
        1
    );

    bleuette.servo.sendValues();
}

/**
 *  Run sequence
 */
void cmd_seq() {
    char *arg;
    unsigned char seq = 0;
    unsigned int i, count, timeout = 0;

    // Get sequence index
    arg = sCmd.next();
    if (arg == NULL) {
        ARG_ERROR
    }

    seq = atoi(arg);
    if (seq < 0 || seq > sizeof(sequences)) {
        println("Sequence not found !");
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

    echo("Run sequence %s !\n", sequences[seq].name);
    //Serial.println(seq); // %c (%s), %i time(s) with %i ms !\n", seq, sequences[seq].name, count, timeout);

    for (i = 0; i < count; i++) {
        bleuette.sequencer.run(sequences[seq]);
        delay(timeout);
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
        println("Sequence not found !");
        return;
    }

Serial.println(seq);
    Serial.println(sequences[seq].name);
    //echo("Run sequence %c (%s) !\n", seq, sequences[seq].name);

    bleuette.sequencer.push(&sequences[seq]);
}

void cmd_start() {
    println("Start !");
    bleuette.sequencer.start();
}

void cmd_clear() {
    println("Clear !");
    bleuette.sequencer.clear();
}

void unrecognized(const char *command) {
    println("Command not found !");
}

