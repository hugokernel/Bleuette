


#ifndef Sequencer_h
#define Sequencer_h

#include <servocontroller.h>

#define SEQUENCER_SERVO_COUNT   SERVO_COUNT

#define __      230
#define PAUSE   231

#define FRONT   235
#define BACK    236

#define DOWN      240
#define UP    241

//#define MID     245

#define HMIN    85
#define HMAX    170
#define HMID    HMIN + (HMAX - HMIN)  / 2

#define VMIN    90
#define VMAX    210
#define VMID    VMIN + (VMAX - VMIN)  / 2

const unsigned char servo_limits[][2] = {

    // Horizontal servos
    { HMIN, HMAX },
    { HMAX, HMIN },
    { HMIN, HMAX },
    { HMAX, HMIN },
    { HMIN, HMAX },
    { HMAX, HMIN },

    // Vertical servos
    { VMIN, VMAX },
    { VMAX, VMIN },
    { VMIN, VMAX },
    { VMAX, VMIN },
    { VMAX, VMIN },
    { VMIN, VMAX }

};

typedef struct motion_t {
    unsigned int delay;
    unsigned char pos[SEQUENCER_SERVO_COUNT];
    void (*function)();
};

typedef struct sequence_t {
    char * name;
    unsigned int count;
    motion_t *motion;
};

#define DELAY_MIN 150

motion_t motion_standby[] = {
    {
        0,
        {
            HMID,   HMID,   HMID,   HMID,   HMID,   HMID,
            UP,   UP,   UP,   UP,   UP,   UP
        },
        NULL
    }
};

sequence_t seq_standby = {
    "Standby",
    1, //sizeof(motion_UP) / sizeof(motion_down[0]),
    motion_standby
};

motion_t motion_down[] = {
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,     DOWN,  DOWN,     DOWN,     DOWN,     DOWN
        },
        NULL
    }
};

sequence_t seq_down = {
    "Leg down",
    1, //sizeof(motion_down) / sizeof(motion_up[0]),
    motion_down
};

motion_t motion_up[] = {
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            UP,   UP,   UP,   UP,   UP,   UP
        },
        NULL
    }
};

sequence_t seq_up = {
    "Leg up",
    1, //sizeof(motion_up) / sizeof(motion_down[0]),
    motion_up
};


motion_t motion_walk[] = {
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,     // Assume start from standby mode
            UP,     __,     __,     UP,     UP,     __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            FRONT,  BACK,   BACK,   FRONT,  FRONT,  BACK,   // Leg in front
            __,     __,     __,     __,     __,     __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,     // Stabilisation : all leg DOWN
            DOWN,   __,     __,     DOWN,   DOWN,   __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,     // 3 legs UP !
            __,     UP,     UP,     __,     __,     UP
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            BACK,   FRONT,  FRONT,  BACK,   BACK,   FRONT,  // One step !
            __,     __,     __,     __,     __,     __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,     // Stabilisation : all leg UP
            __,     DOWN,   DOWN,   __,     __,     DOWN
        },
        NULL
    }
        /*
        ,
        0,
        {
            __,     __,     __,     __,     __,     __,     // 3 legs DOWN !
            DOWN,     __,     __,     UP,     UP,     __
        },
        0,
        {
            FRONT,  BACK,   BACK,   FRONT,  FRONT,  BACK,   // Leg in front
            __,     __,     __,     __,     __,     __
        },
        */
};

sequence_t seq_walk = {
    "Walk",
    6, //sizeof(motion_UP) / sizeof(motion_down[0]),
    motion_walk
};

motion_t motion_pivot[] = {
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            UP,   DOWN,     DOWN,   DOWN,   DOWN,   UP
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            FRONT,  __,   __,   __,  __,  BACK,
            __,     __,     __,     __,     __,     __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,  DOWN,    DOWN,  DOWN,   DOWN,   DOWN
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            __,  UP,     __,     __,   UP,   __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     BACK,     __,     __,     FRONT,     __,
            __,     __,     __,     __,     __,     __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            __,  DOWN,     UP,     UP,   DOWN,   __
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            BACK,   FRONT,  __,  __,   BACK,   FRONT,
            __,     __,     __,     __,     __,     __
        },
        NULL
    },
    //GOOD AVANT
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,     // Stabilisation : all leg UP
            __,     UP,   DOWN,   __,     __,     DOWN
        },
        NULL
    }
};

sequence_t seq_pivot = {
    "Pivot right",
    7, //sizeof(motion_UP) / sizeof(motion_down[0]),
    motion_pivot
};

sequence_t s_pump[] = {
    seq_up,
    seq_down,
    seq_up,
    seq_down
};

sequence_t sequences[] = {
    seq_standby,
    seq_up,
    seq_down,
    seq_walk,
    seq_pivot
//    sequence_pump
};

class Sequencer
{
private:
    ServoController _servo;

    sequence_t      *_sequences[10];
    unsigned char   _sequences_count;
    unsigned char   _sequences_index;

    void (*_callback)(unsigned int);
    void _run(struct sequence_t, unsigned int, unsigned int, bool);
public:
    Sequencer();
    Sequencer(ServoController);

    inline unsigned char getValue(unsigned char, unsigned char);

    void setCallback(void (*)(unsigned int));

    void add(sequence_t *);

    void push(sequence_t *);
    void clear();

    void start();

    void run(struct sequence_t);
    void run(struct sequence_t, bool);
    void run(struct sequence_t, unsigned int);
    void run(struct sequence_t, unsigned int, unsigned int);
};

Sequencer::Sequencer() {
    memset(&_sequences, 0, sizeof(_sequences));
    _sequences_count = 0;
    _sequences_index = 0;

    _callback = NULL;
}

Sequencer::Sequencer(ServoController servo) :
    _servo(servo)
{
    //memset(&_sequences, 0, sizeof(_sequences));
    _sequences_count = 0;
    _sequences_index = 0;

    _callback = NULL;
}

/*
bool Sequencer::runLine(struct sequence_t seq, unsigned int i, unsigned int timeout)
{
    servo.setValues(seq.motion[i].servos, seq.motion[i].values, seq.motion[i].count);
    servo.sendValues();
    delay(seq.motion[i].delay + timeout);
}
*/

inline unsigned char Sequencer::getValue(unsigned char servo, unsigned char pos) {
    switch (pos) {
        case FRONT:
        case DOWN:
            return servo_limits[servo][0];
        case BACK:
        case UP:
            return servo_limits[servo][1];
        default:
            // Todo: verifier qu'on dépasse pas les limites...
            return pos;
//        case MID:
//            return servo_limits[servo][1] + (servo_limits[servo][0] - servo_limits[servo][0]) / 2;
    }
}

void Sequencer::setCallback(void (*function)(unsigned int)) {
    _callback = function;
}

void Sequencer::add(sequence_t *seq) {
    //_sequences = seq;
}

void Sequencer::push(sequence_t *seq) {

//Serial.print(seq.name);

    //char * args = (char *)malloc(size + 3);
    _sequences[_sequences_count++] = seq;
    //Serial.print(_sequences[_sequences_count].name);

    if (_sequences_count > sizeof(_sequences)) {
        _sequences_count = 0;
    }
}

void Sequencer::clear() {
    _sequences_count = 0;
    _sequences_index = 0;
}

void Sequencer::start() {
    Serial.print("Sequence count :");
    Serial.println(_sequences_count);
    for (unsigned int i = 0; i < _sequences_count; i++) {
        //Serial.print(_sequences[i].name);
        run(*_sequences[i++]);
    }
}

void Sequencer::_run(struct sequence_t seq, unsigned int start, unsigned int end, bool forward) {

    // Todo: implement reverse mode !

    for (unsigned int index = start; index < end; index++) {
        // ToDo: Calculate delay if struct.delay = 0 with current value

        for (unsigned char i = 0; i < SEQUENCER_SERVO_COUNT; i++) {
            if (seq.motion[index].pos[i] != __) {
                //Serial.print(getValue(i, seq.motion[index].pos[i]));
                //Serial.print(", ");
                _servo.setValue(i, getValue(i, seq.motion[index].pos[i]));
            }
        }

        if (_callback != NULL) {
            _callback(index);
        }

        if (seq.motion[index].function) {
            seq.motion[index].function();
        }

        //Serial.println(seq.name);
        _servo.sendValues();
        
        //Serial.print("Delay: ");
        //Serial.println(seq.motion[index].delay);

        if (!seq.motion[index].delay) {
            delay(DELAY_MIN);
        } else {
            delay(seq.motion[index].delay);
        }
    }
}

void Sequencer::run(struct sequence_t seq) {
   _run(seq, 0, seq.count, true); 
}

void Sequencer::run(struct sequence_t seq, bool forward) {
   _run(seq, 0, seq.count, forward); 
}

void Sequencer::run(struct sequence_t seq, unsigned int start) {
   _run(seq, start, seq.count, true); 
}

void Sequencer::run(struct sequence_t seq, unsigned int start, unsigned int end) {
   _run(seq, start, end, true); 
}

/*
bool Bleuette::runSequenceR(struct sequence_t seq, unsigned int timeout) {
    for (unsigned int i = 0; i < SEQUENCER_SERVO_COUNT; i++) {
        runLine(seq, i, timeout);
    }
}
*/

#endif

