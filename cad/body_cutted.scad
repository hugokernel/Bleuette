
use <body.scad>;
use <lib/dovetail.scad>;

PART = 0;
SUPPORT = true;

BODY_THICKNESS = 10;
BODY_WIDTH = 100;
BODY_LENGTH = 320;
 
CLEAR = 0.2;
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

SPEAKER_DIAMETER = 45;
module speaker_holes() {
    width_max = 10;

    speaker() {
        hull() {
            for (data = [
                [0, SPEAKER_DIAMETER / 2 - width_max, 0],
                [0, -SPEAKER_DIAMETER / 2 + width_max, 0]
                ]) {
                translate(data) {
                    cylinder(r = width_max, h = 30);   
                }
            }
        };
    
        cylinder(r = 1.7, h = 10);
    };
}

module wifi_scaled() {
    clear_scale = 1.1;
    scale([clear_scale, clear_scale, 1]) {
        wifi();
    }
}

module show(part, support = false) {

    teeths = [TEETHS_COUNT, 10, CLEAR];
    dim = [BODY_WIDTH * 2, BODY_LENGTH, BODY_THICKNESS];

    % cube(size = dim, center = true);

    if (part == 0) {
        difference() {
            color("GREEN") intersection() {
                body_rotated(support);
                cutter([0, - BODY_LENGTH / (3 * 2), 0], dim, teeths, true);
            }

            translate([0, -95, -BODY_THICKNESS / 2 - 1]) {
                speaker_holes();
            }

            translate([0, -137, -BODY_THICKNESS / 2 - 11]) {
                wifi_scaled();
            }
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
    %translate([0, -55, 0]) {
        cube(size = [110, 35, 10], center = true);
    }

    translate([0, -110, 2.5]) {
        cube(size = [24, 100, 5], center = true);
    }

    if (!SUPPORT) {
        rotate([0, 180, 0])
            show(PART, SUPPORT);
    } else {
        show(PART, SUPPORT);
    }
}

