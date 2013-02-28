
#define P   Serial.print
#define PLN Serial.println

#include <bleuette.h>

Bleuette bleuette;

#include <sequencer.h>
#include "sequences.h"
#include <servocontroller.h>
#include <SerialCommand.h>

//#include <Streaming.h>

extern HardwareSerial Serial1;


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

Sequencer::Status waitSerial(unsigned int i) {
    P("Waiting for serial, sequence ");
    P(i);
    P(" ...");
    while (Serial.read() != '\n');
    PLN("[ok]");

    return Sequencer::STATUS_OK;
}

Sequencer::Status waitButton(unsigned int i) {
    P("Waiting for button, sequence ");
    P(i);
    P(" ...");
    while (!bleuette.getButtonState(BLEUETTE_BUTTON1));
    PLN("[ok]");

    return Sequencer::STATUS_OK;
}

