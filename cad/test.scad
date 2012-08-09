
use <lib/servos.scad>
use <body.scad>
use <leg.scad>

$fn = 50;

SUPPORT_SPACE = 65.0;

BODY_THICKNESS = 6;

BODY_LEG_HOLE_DIAMETER = 5.5;
BODY_LEG_HOLE_HEAD_SCREW_HEIGHT = 3.3;
BODY_LEG_HOLE_HEAD_SCREW_DIAMETER = 10;
BODY_LEG_HOLE_NUT_DIAMETER = 9.5;

SERVO_HOLDER_SCREW_DIAMETER = 2;
SERVO_HOLDER_NUT_DIAMETER = 6;
SERVO_HOLDER_NUT_HEIGHT = 1.9;

module servo_nut_test(screw_diameter, nut_diameter, nut_height = 20) {
    rotate([0, 180, 0]) {
        translate([0, 0, - BODY_THICKNESS - 1]) {
            cylinder(h = BODY_THICKNESS + 2 + 10, r = screw_diameter / 2);
        }

        translate([0, 0, - BODY_THICKNESS + 3.1 - 0.2]) {
            cylinder(r = nut_diameter / 2, h = nut_height, $fn = 6);
        }
    }
}

module show() {
    intersection() {

        union() {
            // Create body
            difference() {
                union() {
                    body();
                    translate([0, 0, SUPPORT_SPACE]) {
                        body_support();
                    }
                }

                // Hole
                translate([90, 0, - BODY_THICKNESS / 2 + BODY_LEG_HOLE_HEAD_SCREW_HEIGHT - 0.01]) {
                    cylinder(r = BODY_LEG_HOLE_DIAMETER / 2, h = 150, center = true);

                    // Head screw footprint
                    rotate([180, 0, 0]) {
                        cylinder(r1 = BODY_LEG_HOLE_DIAMETER / 2, r2 = BODY_LEG_HOLE_HEAD_SCREW_DIAMETER / 2, h = BODY_LEG_HOLE_HEAD_SCREW_HEIGHT);
                    }
                }

                translate([150, 0, - BODY_THICKNESS / 2 + BODY_LEG_HOLE_HEAD_SCREW_HEIGHT - 0.01]) {
                    cylinder(r = BODY_LEG_HOLE_DIAMETER / 2, h = 150, center = true);

                    // Head screw footprint
                    rotate([180, 0, 0]) {
                        cylinder(r1 = BODY_LEG_HOLE_DIAMETER / 2, r2 = BODY_LEG_HOLE_HEAD_SCREW_DIAMETER / 2, h = BODY_LEG_HOLE_HEAD_SCREW_HEIGHT);
                    }
                }

                // Nut test
                translate([100, 0, SUPPORT_SPACE]) {
                    servo_nut_test(SERVO_HOLDER_SCREW_DIAMETER, SERVO_HOLDER_NUT_DIAMETER, SERVO_HOLDER_NUT_HEIGHT);
                }
                translate([110, 0, SUPPORT_SPACE]) {
                    servo_nut_test(2.5, 5.8, 2.1);
                }
                translate([120, 0, SUPPORT_SPACE]) {
                    servo_nut_test(3, 6, 3.1);
                }

                translate([130, 0, SUPPORT_SPACE + 1.6]) {
                    cylinder(r = BODY_LEG_HOLE_NUT_DIAMETER / 2, h = 3, center = true, $fn = 6);
                }
            }
        }

        child();
    }
}

/*
if (0) {
    rotate([180, 0, 0]) {
        show() {
            translate([140, 50, 0]) {
                cube(size = [120, 120, 50], center = true);
            }
        }
    }
} else {
    difference() {
        show() {
            translate([140, 50, 50]) {

                cube(size = [120, 120, 50], center = true);
            }
        }
    }
}
*/

module test_arm() {
    SPACER_LENGTH = 6;

    ARMS_SPACING = 20;

    ARM_THICKNESS = 4;
    ARM_WIDTH = 20;

    difference() {
        spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS);
        translate([6, 0, 0]) {
            cube(size = [20, 30, 30], center = true);
        }
    }
}

if (1) {
    test_arm();
} else {

    difference() {
        main_arm();
        translate([0, -17, 0]) {
            cube(size = [30, 100, 30], center = true);
        }   
    }
}

