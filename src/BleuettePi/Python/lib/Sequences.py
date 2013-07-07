
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

Sequences = {
    'standby':  Seq_StandBy,
    'middle':   Seq_Middle,
    'pushup':   Seq_PushUp,
    'walk':     Seq_Walk
}

