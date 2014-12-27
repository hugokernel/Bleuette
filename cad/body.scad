
use <lib/servos.scad>
use <lib/arduino.scad>
use <lib/oshw.scad>

$fn = 50;

DEBUG = false;

/**kljkljlnljn
 *  Body informationkljjkljk
 */
BODY_WID
BODY_THICKNESS = 6;

/**
 *  Space between the 2 surface
 */
SUPPORT_SPACE = 65.0;

RADIUS = 7;

/**
 *  Body to leg parameters
 */
BODY_LEG_HOLE_DIAMETER = 5.5;
BODY_LEG_HOLE_HEAD_SCREW_HEIGHT = 3.3;
BODY_LEG_HOLE_HEAD_SCREW_DIAMETER = 10;
BODY_LEG_HOLE_NUT_DIAMETER = 9.5;

/**
 *  Servo screw
 */
SERVO_HOLDER_SCREW_DIAMETER = 2.7;
SERVO_HOLDER_NUT_DIAMETER = 6;
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
THREADED_ROD_DIAMETER = 3.8;
THREADED_ROD_Z_POSITION = 1;

/**
 *  Toolbox support
 */
TOOLBOX_SCREW_DIAMETER = 2;

module wifi() {
    width = 11.2;
    length = 16.2;
    height = 18.25;
    base_diameter = 5.8;
    base_height = 2;
    antenna_diameter = 8;
    antenna_height = 100;

    translate([0, 0, height / 2]) {
        cube(size = [length, width, height], center = true);
    }

    translate([0, 0, height + base_height / 2]) {
        cylinder(r = base_diameter / 2, h = 3, center = true);
    }

    translate([0, 0, height + base_height + antenna_height / 2]) {
        cylinder(r = antenna_diameter / 2, h = antenna_height, center = true);
    }
}

// Membrane diameter
SPEAKER_DIAMETER = 45;

module speaker() {
    diameter = SPEAKER_DIAMETER;
    hole = 6;
    thickness = 5;

    cylinder(r = diameter / 2, h = thickness, center = true);
    child(0);

    for (data = [
        //[0, diameter / 2 + 5, 0],
        [0, 30, 0],
        [0, -30, 0],
    ]) {
        translate(data) {
            cylinder(r = hole / 2, h = thickness, center = true);

            child(1);
        }
    }
}

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

/**
 *  Rounded box
 */
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
    clear = 1.5;

    rotate([0, 0, 90]) {
        //color("BLACK")
        futabas3003() {
            translate([0, 0, - BODY_THICKNESS - 1]) {
                cylinder(h = BODY_THICKNESS + 2, r = SERVO_HOLDER_SCREW_DIAMETER / 2);
            }

            translate([0, 0, - BODY_THICKNESS - 0.65]) {
                cylinder(r = SERVO_HOLDER_NUT_DIAMETER / 2, h = SERVO_HOLDER_NUT_HEIGHT, $fn = 6);
            }
        }

        translate([ - clear / 2, - clear / 2, 0]) {
            cube([20.1 + clear, 39.9 + clear, 36.1], false);
        }

        if (DEBUG) {
            %cube([20.1, 39.9, 36.1], false);
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

/**
 *  Make a slotted hole (oblong)
 */
module slotted_hole(radius, length) {
    hull() {
        translate([0, - length / 2 + radius, 0]) {
            cylinder(r = radius, h = 10, center = true);
        }

        translate([0, length / 2 - radius, 0]) {
            cylinder(r = radius, h = 10, center = true);
        }
    }
}

/**
 *  Toolbox support
 *  @Todo: Explain measure
 */
module toolbox_support(is_top = false) {

    radius = 2;

    if (is_top) {
        translate([0, -8, 0]) {
            cylinder(r = TOOLBOX_SCREW_DIAMETER / 2, h = 10, center = true);
        }

        slotted_hole(2, 10);

        translate([0, 8, 0]) {
            cylinder(r = TOOLBOX_SCREW_DIAMETER / 2, h = 10, center = true);
        }
    } else {

        translate([0, -19, 0]) {
            slotted_hole(radius, 10);
        }

        translate([0, -11, 0]) {
            cylinder(r = TOOLBOX_SCREW_DIAMETER / 2, h = 10, center = true);
        }

        translate([0, 0, 0]) {
            slotted_hole(radius, 16);
        }

        translate([0, 11, 0]) {
            cylinder(r = TOOLBOX_SCREW_DIAMETER / 2, h = 10, center = true);
        }

        translate([0, 19, 0]) {
            slotted_hole(radius, 10);
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

        translate([10, 10, -23]) {
            servo();
        }

        translate([10, -30, -23]) {
            servo();
        }

        // Arduino
        arduino(1);

        // Toolbox
        translate([BODY_LENGTH / 2 - 8, 0, 0]) {
            toolbox_support();
        }

        translate([- BODY_LENGTH / 2 + 8, 0, 0]) {
            toolbox_support();
        }

        // Passe cable
        translate([75, 0, 0]) {
            slotted_hole(3, 20);
        }

        translate([-75, 0, 0]) {
            slotted_hole(3, 20);
        }
    }

    // Leg support
    translate([0, width / 2 - 0.01, 0]) {
        leg_support(head_screw_footprint = false);
    }

    translate([0, - width / 2 + 0.01, 0]) {
        rotate([0, 0, 180]) {
            leg_support(head_screw_footprint = false);
        }
    }

    translate([length / 2 - 30, width / 2 - 0.01, 0]) {
        leg_support(head_screw_footprint = false);
    }

    translate([length / 2 - 30, - width / 2 + 0.01, 0]) {
        rotate([0, 0, 180]) {
            leg_support(head_screw_footprint = false);
        }
    }

    translate([- (length / 2 - 30), width / 2 - 0.01, 0]) {
        leg_support(head_screw_footprint = false);
    }

    translate([- (length / 2 - 30), - width / 2 + 0.01, 0]) {
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

        translate([0, 0, BODY_THICKNESS / 2 - 1]) {
            rotate([0, 0, -90]) {
                linear_extrude(height=2)
                    oshw_logo_2d(20);
            }
        }

        // Toolbox support
        translate([BODY_LENGTH / 2 - 8, 0, 0]) {
            toolbox_support(true);
        }

        translate([- BODY_LENGTH / 2 + 8, 0, 0]) {
            toolbox_support(true);
        }
    }

    // Leg support
    translate([0, width / 2 - 0.01, 0]) {
        leg_support(leg_length);
    }

    translate([0, - width / 2 + 0.01, 0]) {
        rotate([0, 0, 180]) {
            leg_support(leg_length);
        }
    }

    translate([length / 2 - 30, width / 2 - 0.01, 0]) {
        leg_support(leg_length);
    }

    translate([length / 2 - 30, - width / 2 + 0.01, 0]) {
        rotate([0, 0, 180]) {
            leg_support(leg_length);
        }
    }

    translate([- (length / 2 - 30), width / 2 - 0.01, 0]) {
        leg_support(leg_length);
    }

    translate([- (length / 2 - 30), - width / 2 + 0.01, 0]) {
        rotate([0, 0, 180]) {
            leg_support(leg_length);
        }
    }

    threaded_rod_holder([0, width / 2 - 11, THREADED_ROD_Z_POSITION]);
    threaded_rod_holder([0, - width / 2 + 11, THREADED_ROD_Z_POSITION]);
}

module head() {

    width = 30;
    length = 15;
    radius = 7;

    rotate([0, 0, -90])
    difference() {
        union() {
            union() {
                cube(size = [width, length, BODY_THICKNESS], center = true);

                translate([0, length / 2, 0]) {
                    cylinder(r = width / 2, h = BODY_THICKNESS, center = true);
                }
            }

            difference() {
                translate([0, - length / 2 + radius / 2, 0]) {
                    cube(size = [width + radius * 2, radius, BODY_THICKNESS], center = true);
                }

                translate([(width + radius * 2) / 2, (- length / 2 + radius / 2) + radius / 2, -5]) {
                    cylinder(r = radius, h = 20);
                }

                translate([- (width + radius * 2) / 2, (- length / 2 + radius / 2) + radius / 2, -5]) {
                    cylinder(r = radius, h = 20);
                }
            }
        }
    }
}

if (1) {

    body();

    if (DEBUG) {
        arduino(0);
    }

    translate([0, 0, SUPPORT_SPACE]) {
        body_support();
    }

} else {
    leg_support();
}

