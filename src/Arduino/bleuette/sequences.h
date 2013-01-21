
#ifndef Sequences_h
#define Sequences_h

#include <bleuette.h>
#include <servocontroller.h>

/**
 *  SERVO HORIZONTAL
 */

#define SH0    SERVO_0
#define SH1    SERVO_1
#define SH2    SERVO_2
#define SH3    SERVO_3
#define SH4    SERVO_4
#define SH5    SERVO_5

// Servo horizontal max / mid / min values
#define SH_MIN  85
#define SH_MAX  170
#define SH_MID  SH_MIN + (SH_MAX - SH_MIN) / 2

// Servo Horizontal Front
#define SH0_F  SH_MIN
#define SH1_F  SH_MAX
#define SH2_F  SH_MIN
#define SH3_F  SH_MAX
#define SH4_F  SH_MIN
#define SH5_F  SH_MAX

// Servo Horizontal Back
#define SH0_B  SH_MAX
#define SH1_B  SH_MIN
#define SH2_B  SH_MAX
#define SH3_B  SH_MIN
#define SH4_B  SH_MAX
#define SH5_B  SH_MIN

/**
 *  SERVO VERTICAL
 */

#define SV0    SERVO_6
#define SV1    SERVO_7
#define SV2    SERVO_8
#define SV3    SERVO_9
#define SV4    SERVO_10
#define SV5    SERVO_11

// Servo vertical max / mid / min values
#define SV_MIN  90
#define SV_MAX  210
#define SV_MID  SV_MIN + (SV_MAX - SV_MIN)  / 2

// Servo Vertical Up
#define SV0_U  SV_MAX
#define SV1_U  SV_MIN
#define SV2_U  SV_MAX
#define SV3_U  SV_MIN
#define SV4_U  SV_MIN
#define SV5_U  SV_MAX

// Servo Vertical Down
#define SV0_D  SV_MIN
#define SV1_D  SV_MAX
#define SV2_D  SV_MIN
#define SV3_D  SV_MAX
#define SV4_D  SV_MAX
#define SV5_D  SV_MIN

// All leg to neutral position
motion_t motion_neutral[] = {
    {
        1000,
        SH0 | SH1 | SH2 | SH3 | SH4 | SH5 |
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        12,
        {
            SH_MID, SH_MID, SH_MID, SH_MID, SH_MID, SH_MID,
            SV_MID, SV_MID, SV_MID, SV_MID, SV_MID, SV_MID,
        },
    }
};

sequence_t sequence_neutral = {
    true,
    sizeof(motion_neutral) / sizeof(motion_neutral[0]),
    motion_neutral,
    "Neutral position"
};

// All leg to up position
motion_t motion_up[] = {
    {
        1000,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_U, SV1_U, SV2_U, SV3_U, SV4_U, SV5_U
        },
    }
};

sequence_t sequence_up = {
    true,
    sizeof(motion_up) / sizeof(motion_up[0]),
    motion_up,
    "Up position"
};

// All leg to down position
motion_t motion_down[] = {
    {
        1000,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_D, SV1_D, SV2_D, SV3_D, SV4_D, SV5_D
        },
    }
};

sequence_t sequence_down = {
    true,
    sizeof(motion_down) / sizeof(motion_down[0]),
    motion_down,
    "Down position"
};

// All leg to down position
#define PUMP_DELAY 300
motion_t motion_pump[] = {
    {
        PUMP_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_D, SV1_D, SV2_D, SV3_D, SV4_D, SV5_D
        },
    },
    {
        PUMP_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_U, SV1_U, SV2_U, SV3_U, SV4_U, SV5_U
        },
    },
    {
        PUMP_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_D, SV1_D, SV2_D, SV3_D, SV4_D, SV5_D
        },
    },
    {
        PUMP_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_U, SV1_U, SV2_U, SV3_U, SV4_U, SV5_U
        },
    }
};

sequence_t sequence_pump = {
    true,
    4,
    motion_pump,
    "Pump"
};

// Walk !
#define WALK_DELAY 800
motion_t motion_walk[] = {
    // Neutral
    {
        WALK_DELAY,
        SH0 | SH1 | SH2 | SH3 | SH4 | SH5 |
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        12,
        {
            SH_MID, SH_MID, SH_MID, SH_MID, SH_MID, SH_MID,
            SV_MID, SV_MID, SV_MID, SV_MID, SV_MID, SV_MID,
        }
    },

    /**
     *  First phase
     */

    // 1
    {
        WALK_DELAY,
        SV0 | SV3 | SV4,
        3,
        {
            SV0_U, SV3_U, SV4_U
        }
    },

    // 2
    {
        WALK_DELAY,
        SH0 | SH3 | SH4,
        3,
        {
            SH0_F, SH3_F, SH4_F
        }
    },

    // 3
    {
        WALK_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_D, SV1_U, SV2_U, SV3_D, SV4_D, SV5_U
        }
    },

    // 4
    {
        WALK_DELAY,
        SH0 | SH1 | SH2 | SH3 | SH4 | SH5,
        6,
        {
            SH0_B, SH1_F, SH2_F, SH3_B, SH4_B, SH5_F
        }
    },

    // 5
    {
        WALK_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_D, SV1_D, SV2_D, SV3_D, SV4_D, SV5_D
        }
    },

    // 5 bis
    {
        WALK_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_U, SV1_D, SV2_D, SV3_U, SV4_U, SV5_D
        }
    },

    // 6
    {
        WALK_DELAY,
        SH0 | SH1 | SH2 | SH3 | SH4 | SH5,
        6,
        {
            SH0_F, SH1_B, SH2_B, SH3_F, SH4_F, SH5_B
        }
    },

    // 7
    {
        WALK_DELAY,
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        6,
        {
            SV0_D, SV1_D, SV2_D, SV3_D, SV4_D, SV5_D
        }
    },

    // 8
    {
        WALK_DELAY,
        SH0 | SH1 | SH2 | SH3 | SH4 | SH5 |
        SV0 | SV1 | SV2 | SV3 | SV4 | SV5,
        12,
        {
            SH0_B, SH1_F, SH2_F, SH3_B, SH4_B, SH5_F,
            SV0_D, SV1_U, SV2_U, SV3_D, SV4_D, SV5_U
        }
    }
};

sequence_t sequence_walk = {
    true,
    10, //sizeof(motion_walk) / sizeof(motion_walk[0]),
    motion_walk,
    "Walk"
};

#endif

