
#include <stdio.h>

#define CHECK_BIT(var, pos) (var & (1 << pos))

#define SERVO24_COUNT 14

enum SERVO24_LIST {

    SERVO24_SERVO0 = 1,     // RA0
    SERVO24_SERVO1 = 2,     // RA1

    SERVO24_SERVO2 = 4,     // RA2
    SERVO24_SERVO3 = 8,     // RA3

    SERVO24_SERVO4 = 16,    // RA4
    SERVO24_SERVO5 = 32,    // RC0

    SERVO24_SERVO6 = 64,    // RB0
    SERVO24_SERVO7 = 128,   // RB1

    SERVO24_SERVO8 = 256,   // RB2
    SERVO24_SERVO9 = 512,   // RB3

    SERVO24_SERVO10 = 1024,  // RB4
    SERVO24_SERVO11 = 2048,  // RB5

    SERVO24_SERVO12 = 4096,  // RD0
    SERVO24_SERVO13 = 8192,  // RD1

};

unsigned char _values[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

unsigned char _limits[] = {
    5, 50,
    5, 50,
    5, 50,
    5, 50,
    5, 50,
    5, 50,
    5, 50,
    5, 50,

    20, 100,
    20, 100,
    20, 100,
    20, 100,
    20, 100,
    20, 100,
    20, 100,
    20, 100,

    30, 200,
    30, 200,
    30, 200,
    30, 200,
    30, 200,
    30, 200,
    30, 200,
    30, 200
};

void setLimit(char servo, unsigned char min, unsigned char max) {

    unsigned char c = 0;

    // Test limits
    for (c = 0; c < 8; c++) {
        if (CHECK_BIT(servo, c)) {
            _limits[c * 2] = min;
            _limits[c * 2 + 1] = max;
        }
    }
}

void setLimits(unsigned char * limits) {
    memcpy(_limits, limits, sizeof(_limits));
}

void getLimits(unsigned char servo, unsigned char * limits)
{
    limits[0] = _limits[servo * 3];
    limits[1] = _limits[servo * 3 + 1];
}

void testLimit(unsigned char data[], unsigned char index) {

}

void testLimits(unsigned char data[], unsigned char index) {
    unsigned char c = 0;
    unsigned char limits[2];
    unsigned char toto, tata;

    // Test limits
    for (c = 0; c < 8; c++) {
        if (CHECK_BIT(data[0], c)) {
            getLimits(c + (index * 8), &limits);

            toto = data[1];
printf("%u (min: %u, max: %u) (%u)\n", data[1], limits[0], limits[1], c + index * 8);
            // Too low !
            if (data[1] < limits[0]) {
                data[1] = limits[0];
                tata = 'l';
                break;
            }

            // Too big !
            if (data[1] > limits[1]) {
                data[1] = limits[1];
                tata = 'h';
                break;
            }

            //printf("%u\t(min: %u, max: %u)\t-> %u (%c)\n", toto, limits[0], limits[1], data[1], tata);
        }
    }
}

void setValues(long servos, unsigned char * values)
{
    unsigned char b, c = 0;
    for (b = 0; b < SERVO24_COUNT; b++) {
        if (CHECK_BIT(servos, b)) {
            printf("%u : %u\n", b, values[c]);
            _values[b] = values[c];
        }
    }
}

int main() {

/*
    unsigned char val0[] = {
        255, 230
    };

    unsigned char val1[] = {
        255, 10,
    };

    unsigned char val2[] = {
        255, 5
    };

    testLimits(&val0, 0);
    printf("Val0: %u\n", val0[1]);

    testLimits(&val1, 1);
    printf("Val1: %u\n", val1[1]);

    testLimits(&val2, 2);
    printf("Val2: %u\n", val2[1]);
*/

/*
    setLimit(2, 11, 211);
    printf("Limit: %u, %u\n", _limits[2], _limits[3]);
*/

    printf("Values: %u, %u\n", _values[0], _values[1]);
    unsigned char toto[] = { 200 };
    setValues(1, toto);
    printf("Values: %u, %u\n", _values[0], _values[1]);

    return 0;
}

