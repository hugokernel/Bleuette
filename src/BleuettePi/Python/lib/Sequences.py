
from Define import __, PAUSE, FRONT, BACK, DOWN, UP, MID

class Seq_StandBy:
    name = "StandBy"
    sequence = [
        [
            0,
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
            0,
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
            0,
            [
                FRONT, FRONT, MID, MID, BACK, BACK,
                UP, UP, UP, UP, UP, UP
            ],
            None
        ],
        [
            0,
            [
                __, __, __, __, __, __,
                DOWN, DOWN, DOWN, DOWN, DOWN, DOWN
            ],
            None
        ]
    ]


