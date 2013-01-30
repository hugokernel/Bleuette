
// Standby
motion_t motion_standby[] = {
    {
        0,
        {
            HMID,   HMID,   HMID,   HMID,   HMID,   HMID,
            UP,     UP,     UP,     UP,     UP,     UP
        },
        NULL
    }
};

sequence_t seq_standby = {
    "Standby",
    1, //sizeof(motion_UP) / sizeof(motion_down[0]),
    motion_standby
};

// Middle
motion_t motion_middle[] = {
    {
        0,
        {
            MID,    MID,    MID,    MID,    MID,    MID,
            MID,    MID,    MID,    MID,    MID,    MID
        },
        NULL
    }
};

sequence_t seq_middle = {
    "Middle",
    1, //sizeof(motion_UP) / sizeof(motion_down[0]),
    motion_middle
};

// Down
motion_t motion_down[] = {
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
        },
        NULL
    }
};

sequence_t seq_down = {
    "Leg down",
    1, //sizeof(motion_down) / sizeof(motion_up[0]),
    motion_down
};

// Up
motion_t motion_up[] = {
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            UP,     UP,     UP,     UP,     UP,     UP
        },
        NULL
    }
};

sequence_t seq_up = {
    "Leg up",
    1, //sizeof(motion_up) / sizeof(motion_down[0]),
    motion_up
};

// Push up
motion_t motion_pushup[] = {
    {
        DELAY_MIN,
        {
            FRONT,  FRONT,  HMID,   HMID,   BACK,   BACK,
            UP,     UP,     UP,     UP,     UP,     UP
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
        },
        NULL
    }
};

sequence_t seq_pushup = {
    "Push up",
    2, //sizeof(motion_up) / sizeof(motion_down[0]),
    motion_pushup
};



// Middle push up
motion_t motion_middle_pushup[] = {
    {
        1000,
        {
            __,     __,     __,     __,     __,     __,
            UP,     UP,     UP,     UP,     UP,     UP
        },
        NULL
    },
    {
        1000,
        {
            __,     __,     __,     __,     __,     __,
            MID,    MID,    MID,    MID,    MID,    MID
        },
        NULL
    },
    {
        1000,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
        },
        NULL
    }
};

sequence_t seq_middle_pushup = {
    "Middle push up",
    3, //sizeof(motion_up) / sizeof(motion_down[0]),
    motion_middle_pushup
};



// Lateral pushup
motion_t motion_pushup_lateral[] = {
    {
        600,
        {
            __,     __,     __,     __,     __,     __,
            UP,     DOWN,   UP,     DOWN,   UP,     DOWN
        },
        NULL
    },
    {
        600,
        {
            __,     __,     __,     __,     __,     __,
            DOWN,  UP,     DOWN,   UP,     DOWN,   UP
        },
        NULL
    }
};

sequence_t seq_pushup_lateral = {
    "Lateral push up",
    2, //sizeof(motion_up) / sizeof(motion_down[0]),
    motion_pushup_lateral
};


// Walk
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
};

sequence_t seq_walk = {
    "Walk",
    6, //sizeof(motion_UP) / sizeof(motion_down[0]),
    motion_walk
};

// Right turn
motion_t motion_right_turn[] = {
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
    }
};

sequence_t seq_right_turn = {
    "Right turn",
    sizeof(motion_right_turn) / sizeof(motion_right_turn[0]),
    motion_right_turn
};


// Left turn
motion_t motion_left_turn[] = {
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
            BACK,   __,     __,     __,     __,     FRONT,
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
            __,     FRONT,  __,     __,     BACK,   __,
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
            FRONT,  BACK,   __,     __,     FRONT,  BACK,
            __,     __,     __,     __,     __,     __
        },
        NULL
    }
};

sequence_t seq_left_turn = {
    "Left turn",
    sizeof(motion_left_turn) / sizeof(motion_left_turn[0]),
    motion_left_turn
};



// Linuxfr
motion_t motion_linuxfr[] = {
    {
        DELAY_MIN,
        {
            FRONT, BACK, BACK, FRONT, HMID, HMID,
            DOWN, DOWN, DOWN, DOWN, UP, DOWN
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            HMID, HMID, HMID, HMID, FRONT, BACK,
            DOWN, UP, UP, DOWN, DOWN, DOWN
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            BACK, FRONT, FRONT, BACK, HMID, HMID,
            DOWN, DOWN, DOWN, DOWN, DOWN, UP
        },
        NULL
    },
    {
        DELAY_MIN,
        {
            HMID, HMID, HMID, HMID, BACK, FRONT,
            UP, DOWN, DOWN, UP, DOWN, DOWN
        },
        NULL
    }
};

sequence_t seq_linuxfr = {
    "Linuxfr",
    sizeof(motion_linuxfr) / sizeof(motion_linuxfr[0]),
    motion_linuxfr
};


sequence_t s_pushup[] = {
    seq_up,
    seq_down,
    seq_up,
    seq_down
};

sequence_t sequences[] = {
    seq_standby,        // 0
    seq_up,             // 1
    seq_down,           // 2
    seq_walk,           // 3
    seq_right_turn,     // 4
    seq_left_turn,      // 5
    seq_pushup,         // 6
    seq_pushup_lateral, // 7
    seq_middle_pushup,  // 8
    seq_linuxfr,        // 9
    seq_middle          // 10
};

/*
0	: 85
1	: 85
2	: 85
3	: 85
4	: 85
5	: 85
6	: 90
7	: 90
8	: 90
9	: 90
10	: 90
11	: 90
*/
