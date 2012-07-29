
use <body.scad>;
use <cutter.scad>;

BODY_THICKNESS = 6;
BODY_WIDTH = 100;
BODY_LENGTH = 320;
  
CLEAR = 0.1;
TEETHS_COUNT = 6;

teeths = [TEETHS_COUNT, 10, CLEAR];
dim = [BODY_WIDTH * 2, BODY_LENGTH, BODY_THICKNESS];

% cube(size = dim, center = true);

module body_rotated() {
    rotate([0, 0, 90]) body();
}

color("GREEN") intersection() {
    body_rotated();
    cutter([0, - BODY_LENGTH / (3 * 2), 0], dim, teeths, true);
}

//translate([22, 0, 0])
%color("RED") intersection() {
    body_rotated();
    cutter([0, - BODY_LENGTH / (3 * 2), 0], dim, teeths, false);
    cutter([0, BODY_LENGTH / (3 * 2), 0], dim, teeths, true);
}

//translate([44, 0, 0])
%color("BLUE") intersection() {
    body_rotated();
    cutter([0, BODY_LENGTH / (3 * 2), 0], dim, teeths, false);
}

