
from Define import __, PAUSE, FRONT, BACK, DOWN, UP, MID

class Seq_StandBy:
    name = "StandBy"
    sequence = [
        [
            None,
            [
                MID, MID, MID, MID, MID, MID,
                UP, UP, UP, UP, UP, UP
            ],
            None
        ]
    ]

class Seq_Release:
    name = "Release"
    sequence = [
        [
            None,
            [
                0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0
            ],
            None
        ]
    ]

class Seq_Middle:
    name = "Middle"
    sequence = [
        [
            None,
            [
                MID, MID, MID, MID, MID, MID,
                MID, MID, MID, MID, MID, MID
            ],
            None
        ]
    ]

class Seq_PushUp:
    name = "Push Up"
    sequence = [
        [
            0.5,
            [
                FRONT, FRONT, MID, MID, BACK, BACK,
                UP, UP, UP, UP, UP, UP
            ],
            None
        ],
        [
            0.5,
            [
                __, __, __, __, __, __,
                DOWN, DOWN, DOWN, DOWN, DOWN, DOWN
            ],
            None
        ]
    ]

class Seq_Walk:
    name = "Walk"
    sequence = [
        [
            None,
            [
                __, __, __, __, __, __,                 # Assume start from standby mode
                UP, __, __, UP, UP, __
            ],
            None
        ],
        [
            None,
            [
                FRONT, BACK, BACK, FRONT, FRONT, BACK,  # Leg in front
                __, __, __, __, __, __
            ],
            None
        ],
        [
            None,
            [
                __, __, __, __, __, __,                 # Stabilisation : all leg DOWN
                DOWN, __, __, DOWN, DOWN, __
            ],
            None
        ],
        [
            None,
            [
                __, __, __, __, __, __,                 # 3 legs UP !
                __, UP, UP, __, __, UP
            ],
            None
        ],
        [
            None,
            [
                BACK, FRONT, FRONT, BACK, BACK, FRONT,  # One step !
                __, __, __, __, __, __
            ],
            None
        ],
        [
            None,
            [
                __, __, __, __, __, __,                 # Stabilisation : all leg UP
                __, DOWN, DOWN, __, __, DOWN
            ],
            None
        ],
    ]

class Seq_Right_Turn:
    name = "Right turn"
    sequence = [
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                UP,   DOWN,     DOWN,   DOWN,   DOWN,   UP
            ],
            None
        ],
        [
            None,
            [
                FRONT,  __,   __,   __,  __,  BACK,
                __,     __,     __,     __,     __,     __
            ],
            None
        ],
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                DOWN,  DOWN,    DOWN,  DOWN,   DOWN,   DOWN
            ],
            None
        ],
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                __,  UP,     __,     __,   UP,   __
            ],
            None
        ],
        [
            None,
            [
                __,     BACK,     __,     __,     FRONT,     __,
                __,     __,     __,     __,     __,     __
            ],
            None
        ],
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                __,  DOWN,     UP,     UP,   DOWN,   __
            ],
            None
        ],
        [
            None,
            [
                BACK,   FRONT,  __,  __,   BACK,   FRONT,
                __,     __,     __,     __,     __,     __
            ],
            None
        ]
    ]

class Seq_Left_Turn:
    name = "Left turn"
    sequence = [
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                UP,   DOWN,     DOWN,   DOWN,   DOWN,   UP
            ],
            None
        ],
        [
            None,
            [
                BACK,   __,     __,     __,     __,     FRONT,
                __,     __,     __,     __,     __,     __
            ],
            None
        ],
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                DOWN,  DOWN,    DOWN,  DOWN,   DOWN,   DOWN
            ],
            None
        ],
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                __,  UP,     __,     __,   UP,   __
            ],
            None
        ],
        [
            None,
            [
                __,     FRONT,  __,     __,     BACK,   __,
                __,     __,     __,     __,     __,     __
            ],
            None
        ],
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                __,  DOWN,     UP,     UP,   DOWN,   __
            ],
            None
        ],
        [
            None,
            [
                FRONT,  BACK,   __,     __,     FRONT,  BACK,
                __,     __,     __,     __,     __,     __
            ],
            None
        ]
    ]

class Seq_Up:
    name = "Up"
    sequence = [
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                UP,     UP,     UP,     UP,     UP,     UP
            ],
            None
        ]
    ]

class Seq_Down:
    name = "Down"
    sequence = [
        [
            None,
            [
                __,     __,     __,     __,     __,     __,
                DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
            ],
            None
        ]
    ]

class Seq_PushUpLateral:
    name = "Push up lateral"
    sequence = [
        [
            600,
            [
                __,     __,     __,     __,     __,     __,
                UP,     DOWN,   UP,     DOWN,   UP,     DOWN
            ],
            None
        ],
        [
            600,
            [
                __,     __,     __,     __,     __,     __,
                DOWN,  UP,     DOWN,   UP,     DOWN,   UP
            ],
            None
        ]
    ]

class Seq_MiddlePushUp:
    name = "Middle push up"
    sequence = [
        [
            1000,
            [
                __,     __,     __,     __,     __,     __,
                UP,     UP,     UP,     UP,     UP,     UP
            ],
            None
        ],
        [
            1000,
            [
                __,     __,     __,     __,     __,     __,
                MID,    MID,    MID,    MID,    MID,    MID
            ],
            None
        ],
        [
            1000,
            [
                __,     __,     __,     __,     __,     __,
                DOWN,   DOWN,   DOWN,   DOWN,   DOWN,   DOWN
            ],
            None
        ]
    ]

class Seq_Linuxfr:
    name = "Linuxfr walk"
    sequence = [
        [
            None,
            [
                FRONT, BACK, BACK, FRONT, MID, MID,
                DOWN, DOWN, DOWN, DOWN, UP, DOWN
            ],
            None
        ],
        [
            None,
            [
                MID, MID, MID, MID, FRONT, BACK,
                DOWN, UP, UP, DOWN, DOWN, DOWN
            ],
            None
        ],
        [
            None,
            [
                BACK, FRONT, FRONT, BACK, MID, MID,
                DOWN, DOWN, DOWN, DOWN, DOWN, UP
            ],
            None
        ],
        [
            None,
            [
                MID, MID, MID, MID, BACK, FRONT,
                UP, DOWN, DOWN, UP, DOWN, DOWN
            ],
            None
        ]
    ]

Sequences = {
    'standby':      Seq_StandBy,
    'middle':       Seq_Middle,
    'release':      Seq_Release,
    'pushup':       Seq_PushUp,
    'walk':         Seq_Walk,
    'rightturn':    Seq_Right_Turn,
    'leftturn':     Seq_Left_Turn,

    'up':           Seq_Up,
    'down':         Seq_Down,
    'pushlateral':  Seq_PushUpLateral,
    'middlepushup': Seq_MiddlePushUp,
    'linuxfr':      Seq_Linuxfr
}

