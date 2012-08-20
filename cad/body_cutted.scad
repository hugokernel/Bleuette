
use <body.scad>;
use <lib/dovetail.scad>;

PART = 0;
SUPPORT = true;

BODY_THICKNESS = 10;
BODY_WIDTH = 100;
BODY_LENGTH = 320;
 
CLEAR = 0.3;
TEETHS_COUNT = (SUPPORT) ? 9 : 6;

module body_rotated(support) {
    rotate([0, 0, 90]) {
        if (support) {
            body_support();
        } else {
            body();
        }
    }
}

module show(part, support = false) {

    teeths = [TEETHS_COUNT, 10, CLEAR];
    dim = [BODY_WIDTH * 2, BODY_LENGTH, BODY_THICKNESS];

    % cube(size = dim, center = true);

    if (part == 0) {
        color("GREEN") intersection() {
            body_rotated(support);
            cutter([0, - BODY_LENGTH / (3 * 2), 0], dim, teeths, true);
        }
    }

    if (part == 1) {
        color("RED") intersection() {
            body_rotated(support);
            cutter([0, - BODY_LENGTH / (3 * 2), 0], dim, teeths, false);
            cutter([0, BODY_LENGTH / (3 * 2), 0], dim, teeths, true);
        }
    }

    if (part == 2) {
        color("BLUE") intersection() {
            body_rotated(support);
            cutter([0, BODY_LENGTH / (3 * 2), 0], dim, teeths, false);
        }
    }
}

intersection() {
    translate([0, -60, 0]) {
        cube(size = [60, 35, 10], center = true);
    }

    //rotate([0, 180, 0])
    show(PART, SUPPORT);
}

