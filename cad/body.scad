
use <lib/servos.scad>
use <lib/arduino.scad>

$fn = 50;

/**
 *  Body information
 */
BODY_WIDTH = 100;
BODY_LENGTH = 320;
BODY_THICKNESS = 6;

/**
 *  Space between the 2 surface
 */
SUPPORT_SPACE = 65.0;

RADIUS = 7;

/**
 *  Body to leg parameters
 */
BODY_LEG_HOLE_DIAMETER = 5;
BODY_LEG_HOLE_HEAD_SCREW_HEIGHT = 3;
BODY_LEG_HOLE_HEAD_SCREW_DIAMETER = 9;
BODY_LEG_HOLE_NUT_DIAMETER = 9;

/**
 *  Servo screw
 */
SERVO_HOLDER_SCREW_DIAMETER = 2;
SERVO_HOLDER_NUT_DIAMETER = 5.5;
SERVO_HOLDER_NUT_HEIGHT = 1.9;

/**
 *  Servo information
 */
SERVO_WIDTH = 20;
SERVO_LENGTH = 40;
SERVO_CLEAR = 1.5;

/**
 *  Threaded rod parameters
 */
THREADED_ROD_DIAMETER = 3.9;
THREADED_ROD_Z_POSITION = 1;


module leg_support(length = 25, width = 30, head_screw_footprint = true) {

    translate([0, length / 2 , 0]) {
        difference() {
            union() {
                union() {
                    cube(size = [width, length, BODY_THICKNESS], center = true);

                    translate([0, length / 2, 0]) {
                        cylinder(r = width / 2, h = BODY_THICKNESS, center = true);
                    }
                }

                difference() {
                    translate([0, - length / 2 + RADIUS / 2, 0]) {
                        cube(size = [width + RADIUS * 2, RADIUS, BODY_THICKNESS], center = true);
                    }

                    translate([(width + RADIUS * 2) / 2, (- length / 2 + RADIUS / 2) + RADIUS / 2, -5]) {
                        cylinder(r = RADIUS, h = 20);
                    }

                    translate([- (width + RADIUS * 2) / 2, (- length / 2 + RADIUS / 2) + RADIUS / 2, -5]) {
                        cylinder(r = RADIUS, h = 20);
                    }
                }
            }

            // Hole
            translate([0, length / 2, -1]) {
                cylinder(r = BODY_LEG_HOLE_DIAMETER / 2, h = BODY_THICKNESS + 5, center = true);
            }

            // Head screw footprint
            if (head_screw_footprint) {
                translate([0, length / 2, BODY_THICKNESS / 2 - BODY_LEG_HOLE_HEAD_SCREW_HEIGHT + BODY_LEG_HOLE_HEAD_SCREW_HEIGHT / 2 + 0.01]) {
                    cylinder(r1 = BODY_LEG_HOLE_DIAMETER / 2, r2 = BODY_LEG_HOLE_HEAD_SCREW_DIAMETER / 2, h = BODY_LEG_HOLE_HEAD_SCREW_HEIGHT, center = true);
                }
            } else {
                translate([0, length / 2, - BODY_THICKNESS / 2]) {
                    cylinder(r = BODY_LEG_HOLE_NUT_DIAMETER / 2, h = 3, center = true, $fn = 6);
                }
            }
        }
    }
}

module rbox(width, length, thickness, radius) {

    cube(size = [width, length - radius * 2, thickness], center = true);
    cube(size = [width - radius * 2, length, thickness], center = true);

    // Rounded corner
    translate([width / 2 - radius, length / 2 - radius, - thickness / 2 ]) {
        cylinder(r = radius, h = thickness);
    }

    translate([- (width / 2 - radius), length / 2 - radius, - thickness / 2 ]) {
        cylinder(r = radius, h = thickness);
    }

    translate([width / 2 - radius, - (length / 2 - radius), - thickness / 2 ]) {
        cylinder(r = radius, h = thickness);
    }

    translate([- (width / 2 - radius), -(length / 2 - radius), - thickness / 2 ]) {
        cylinder(r = radius, h = thickness);
    }
}

module threaded_rod_holder(position, substract = false) {

    module threaded_rod(diameter, length) {
        // Threaded rod
        translate([0, 0, position[2]]) {
            rotate([0, 90, 0]) {
                //translate([0, width / 2 - 11, 0]) {
                translate([position[0], position[1], 0]) {
                    cylinder(r = diameter, h = length, center = true);
                }

                //translate([0, - width / 2 + 11, 0]) {
                //    cylinder(r = diameter, h = length, center = true);
                //}
            }
        }
    }

    if (substract == true) {
        threaded_rod(THREADED_ROD_DIAMETER / 2, BODY_LENGTH + 10);
    } else {
        difference() {
            threaded_rod(THREADED_ROD_DIAMETER / 2 + 2, BODY_LENGTH);
            threaded_rod(THREADED_ROD_DIAMETER / 2, BODY_LENGTH + 10);
        }
    }
}

module servo() {
    rotate([0, 0, 90]) {
        color("BLACK") futabas3003() {
            translate([0, 0, - BODY_THICKNESS - 1]) {
                cylinder(h = BODY_THICKNESS + 2, r = SERVO_HOLDER_SCREW_DIAMETER / 2);
            }

            translate([0, 0, - BODY_THICKNESS - SERVO_HOLDER_NUT_HEIGHT]) {
                cylinder(r = SERVO_HOLDER_NUT_DIAMETER / 2, h = SERVO_HOLDER_NUT_HEIGHT, $fn = 6);
            }
        }
    }
}

module arduino(hole = 1) {
    translate([35, -18, BODY_THICKNESS / 2 + 2]) {
        rotate([0, 0, 90]) {
            Arduino(hole);
        }
    }
}

module body() {

    width = BODY_WIDTH;
    length = BODY_LENGTH;
    thickness = BODY_THICKNESS;

    difference() {
        rbox(length, width, thickness, RADIUS);

        // Threaded rod
        threaded_rod_holder([0, width / 2 - 11, - THREADED_ROD_Z_POSITION], true);
        threaded_rod_holder([0, - width / 2 + 11, - THREADED_ROD_Z_POSITION], true);

        // Servo hole
        translate([length / 2 - 20, 10, -23]) {
            servo();
        }

        translate([length / 2 - 20, -30, -23]) {
            servo();
        }

        translate([- length / 2 + 60, -30, -23]) {
            servo();
        }

        translate([- length / 2 + 60, 10, -23]) {
            servo();
        }

        translate([20, 10, -23]) {
            servo();
        }

        translate([20, -30, -23]) {
            servo();
        }

        // Arduino
        arduino(1);
    }

    // Leg support
    translate([0, width / 2, 0]) {
        leg_support(head_screw_footprint = false);
    }

    translate([0, - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support(head_screw_footprint = false);
        }
    }

    translate([length / 2 - 30, width / 2, 0]) {
        leg_support(head_screw_footprint = false);
    }

    translate([length / 2 - 30, - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support(head_screw_footprint = false);
        }
    }

    translate([- (length / 2 - 30), width / 2, 0]) {
        leg_support(head_screw_footprint = false);
    }

    translate([- (length / 2 - 30), - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support(head_screw_footprint = false);
        }
    }

    // Threaded rod
    threaded_rod_holder([0, width / 2 - 11, - THREADED_ROD_Z_POSITION]);
    threaded_rod_holder([0, - width / 2 + 11, - THREADED_ROD_Z_POSITION]);
}

module body_support() {

    width = 50;
    length = BODY_LENGTH;
    thickness = BODY_THICKNESS;

    leg_length = BODY_WIDTH / 2 - width / 2 + 25;

    difference() {
        rbox(length, width, thickness, RADIUS);

        // Threaded rod
        threaded_rod_holder([0, width / 2 - 11, THREADED_ROD_Z_POSITION], true);
        threaded_rod_holder([0, - width / 2 + 11, THREADED_ROD_Z_POSITION], true);
    }

    // Leg support
    translate([0, width / 2, 0]) {
        leg_support(leg_length);
    }

    translate([0, - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support(leg_length);
        }
    }

    translate([length / 2 - 30, width / 2, 0]) {
        leg_support(leg_length);
    }

    translate([length / 2 - 30, - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support(leg_length);
        }
    }

    translate([- (length / 2 - 30), width / 2, 0]) {
        leg_support(leg_length);
    }

    translate([- (length / 2 - 30), - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support(leg_length);
        }
    }

    threaded_rod_holder([0, width / 2 - 11, THREADED_ROD_Z_POSITION]);
    threaded_rod_holder([0, - width / 2 + 11, THREADED_ROD_Z_POSITION]);
}

if (1) {

    body();

    arduino(0);

    translate([0, 0, SUPPORT_SPACE]) {
        body_support();
    }
} else {
    leg_support();
}

