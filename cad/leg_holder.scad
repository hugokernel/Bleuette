
include <lib/servos.scad>

$fn = 25;

DEBUG = false;

BODY_HEIGHT = 65.0;
BODY_WIDTH = 27.5;

MAIN_HOLE_DIAMETER = 6;

SERVO_WIDTH = 24;
SERVO_LENGTH = 53.5;

CHANFREIN = 4;

LEG_THICKNESS = 4;
LEG_HOLE_DIAMETER = 2.5;

CONNECTION_WIDTH = 6;
CONNECTION_HEIGHT = 8;

SERVO_HOLDER_HEIGHT = 11.5;
SERVO_HOLDER_WIDTH = 16;

SERVO_HOLDER_SCREW_DIAMETER = 2.7;
SERVO_HOLDER_NUT_DIAMETER = 6;
SERVO_HOLDER_NUT_HEIGHT = 1.9;

WASHER_DIAMETER = 0;//17.7;
WASHER_HEIGHT = 1.1;

LEG_HOLDER_MIN_WALL_THICKNESS = 1;
LEG_HOLDER_MAX_WALL_THICKNESS = 3;

/**
 *  Todo:
 *  - Agrandir la fente des pattes pour limiter les frottements ?
 */

module servo_holder(is_top = true, boolean = false) {

    //position_z = (is_top) ? 8 : 7.6;
    position_z = 7.8; // Average

    module nut_holder(is_top) {

        position_x = (is_top) ? SERVO_HOLDER_WIDTH - CHANFREIN - SERVO_HOLDER_NUT_HEIGHT + 0.01 : - 0.01;

        // Nut holder
        translate([position_x, 8, position_z]) {
            rotate([0, 90, 0]) {
                rotate([0, 0, 90]) {
                    cylinder(r = SERVO_HOLDER_NUT_DIAMETER / 2, h = SERVO_HOLDER_NUT_HEIGHT, $fn = 6);
                }
            }
        }

        // Nut holder
        translate([position_x, 18, position_z]) {
            rotate([0, 90, 0]) {
                rotate([0, 0, 90]) {
                    cylinder(r = SERVO_HOLDER_NUT_DIAMETER / 2, h = SERVO_HOLDER_NUT_HEIGHT, $fn = 6);
                }
            }
        }
    }

    translate([-(SERVO_HOLDER_WIDTH - CHANFREIN) / 2, 0, - (SERVO_HOLDER_HEIGHT - CONNECTION_HEIGHT / 2)]) {
        difference() {
            // Servo holder
            cube(size = [SERVO_HOLDER_WIDTH - CHANFREIN, SERVO_WIDTH, SERVO_HOLDER_HEIGHT]);

            // Servo hole
            translate([-20, 8, position_z])  rotate([0, 90, 0]) cylinder(h = 50, r = SERVO_HOLDER_SCREW_DIAMETER / 2);
            translate([-20, 18, position_z]) rotate([0, 90, 0]) cylinder(h = 50, r = SERVO_HOLDER_SCREW_DIAMETER / 2);

            nut_holder(false);
            
            nut_holder(true);

            // Servo holder chanfrein
            translate([SERVO_WIDTH, 20, -0.1]) {
                rotate([0, -90, 0]) {
                    linear_extrude(height = 30)
                        polygon([[0, 0], [0, -SERVO_WIDTH - 0.1], [4, -SERVO_WIDTH]]);
                }
            }
        }

        // Test
        translate([(SERVO_HOLDER_WIDTH - CHANFREIN) / 2, SERVO_WIDTH, 3.5]) {
            linear_extrude(height = CONNECTION_HEIGHT) {
                // Boolean is the slide connection part
                if (boolean) {
                    polygon([[-CONNECTION_WIDTH / 2,0],[CONNECTION_WIDTH / 2,0],[CONNECTION_WIDTH / 2,4],[-CONNECTION_WIDTH / 2,4],[-3,0]]);
                } else {
                    polygon([[-CONNECTION_WIDTH / 2 + 1,0],[CONNECTION_WIDTH / 2 - 1,0],[CONNECTION_WIDTH / 2,4],[-CONNECTION_WIDTH / 2,4]]);
                }
            }
        }
    }
}


module leg_holder(slot = false) {

    max_thickness = LEG_HOLDER_MAX_WALL_THICKNESS - LEG_HOLDER_MIN_WALL_THICKNESS;

    difference() {
        linear_extrude(height = BODY_HEIGHT) {
            polygon([
                [LEG_HOLDER_MIN_WALL_THICKNESS + LEG_THICKNESS, 19],
                [LEG_HOLDER_MIN_WALL_THICKNESS + LEG_THICKNESS + max_thickness, 15 + LEG_HOLE_DIAMETER],
                [LEG_HOLDER_MIN_WALL_THICKNESS + LEG_THICKNESS + max_thickness, 12 - LEG_HOLE_DIAMETER],
                [LEG_HOLDER_MIN_WALL_THICKNESS + LEG_THICKNESS, 0],
                [0, 0],
                [0, 19]
            ]);
        }

        translate([0, LEG_THICKNESS, -1]) {
            cube(size = [LEG_THICKNESS, 100, BODY_HEIGHT - 2]);
        }

        if (slot) {
            translate([0, LEG_THICKNESS - 5, BODY_HEIGHT / 2]) {
                cube(size = [LEG_THICKNESS, 30, BODY_HEIGHT / 2 - 3]);
            }
        }
    }

    translate([0, 0, BODY_HEIGHT / 2 - 3]) {
        cube(size = [LEG_THICKNESS, 19, 3]);
    }

}

module support() {

    module support_servo_holder(rail = true) {
        // Top
        translate([CHANFREIN + SERVO_HOLDER_WIDTH / 2 - 2, -SERVO_WIDTH, SERVO_LENGTH + 4]) {
            rotate([0, 180, 0]) {
                color("RED") servo_holder(true);

                if (rail) {
                    translate([0, 0, CONNECTION_HEIGHT]) {
                        servo_holder(true, true);
                    }
                }
            }
        }

        translate([CHANFREIN + SERVO_HOLDER_WIDTH - 2.5, -SERVO_WIDTH, SERVO_LENGTH + 4]) {
            rotate([0, 180, 0]) {
                color("BLUE") servo_holder(true);

                if (rail) {
                    translate([0, 0, CONNECTION_HEIGHT]) {
                        servo_holder(true, true);
                    }
                }
            }
        }

        // Bottom
        translate([CHANFREIN + SERVO_HOLDER_WIDTH / 2 - 2, -SERVO_WIDTH, SERVO_HOLDER_HEIGHT - 3]) {
            color("RED") servo_holder(false);

            if (rail) {
                translate([0, 0, CONNECTION_HEIGHT]) {
                    servo_holder(false, true);
                }
            }
        }

        translate([CHANFREIN + SERVO_HOLDER_WIDTH - 2.5, -SERVO_WIDTH, SERVO_HOLDER_HEIGHT - 3]) {
            color("BLUE") servo_holder(true);

            if (rail) {
                translate([0, 0, CONNECTION_HEIGHT]) {
                    servo_holder(false, true);
                }
            }
        }
    }

    module rudder_holder() {
        // Rudder holder
        rotate([0, 0, -45]) {
            translate([0, -5.2, 0]) {
                color("RED") rudder();
            }
        }

        rotate([0, 0, 45]) {
            translate([19.45, -24.7, 0]) {
                color("RED") rudder();
            }
        }
    }

    difference() {
        union() {
            difference() {
                cube(size = [BODY_WIDTH, BODY_WIDTH, BODY_HEIGHT]);

                translate([13.75, 10, 0]) {
                    // Main hole
                    translate([0, 0, -1]) {
                        cylinder(h = BODY_HEIGHT + 2, r = MAIN_HOLE_DIAMETER / 2);
                    }

                    if (WASHER_DIAMETER) {
                        // Washer
                        cylinder(r = WASHER_DIAMETER / 2, h = WASHER_HEIGHT / 2, center = true);

                        translate([0, 0, BODY_HEIGHT]) {
                            cylinder(r = WASHER_DIAMETER / 2, h = WASHER_HEIGHT / 2, center = true);
                        }
                    }
                }

                support_servo_holder();
            }

            if (DEBUG) {
                support_servo_holder(false);
            }

            // Right leg holder
            translate([BODY_WIDTH, 4, 0]) {
                leg_holder(true);
            }

            // Left leg holder
            translate([ 0, 4, 0]) {
                mirror([1, 0, 0]) {
                    leg_holder(true);
                }
            }
        }

        // Hole for leg
        translate([-50, BODY_WIDTH - 10, 10]) rotate([0, 90, 0]) cylinder(h = 100, r = LEG_HOLE_DIAMETER);
        translate([-50, BODY_WIDTH - 10, 50]) rotate([0, 90, 0]) cylinder(h = 100, r = LEG_HOLE_DIAMETER);

        // Reduce size
        linear_extrude(height = BODY_HEIGHT) {
            // Body chanfrein
            polygon([[0, BODY_WIDTH - CHANFREIN], [0, BODY_WIDTH], [CHANFREIN, BODY_WIDTH]]);
            polygon([[BODY_WIDTH, BODY_WIDTH - CHANFREIN], [BODY_WIDTH, BODY_WIDTH], [BODY_WIDTH - CHANFREIN, BODY_WIDTH]]);
            polygon([[BODY_WIDTH, 0], [BODY_WIDTH, CHANFREIN], [BODY_WIDTH - CHANFREIN, 0]]);
            polygon([[0, 0], [0, CHANFREIN], [CHANFREIN, 0]]);
        }

        // Front
        translate([BODY_WIDTH / 2, BODY_HEIGHT / 2, -1]) {
            cylinder(r = 10, h = BODY_HEIGHT + 1, $fn = 10);
        }

        // Hole for cable
        translate([BODY_WIDTH / 2 + 5.5, 0, 5]) {
            rotate([90, 0, 0]) {
                cylinder(r = 2, h = 100, center = true);
            }
        }

        // Hole for cable
        translate([8.5, 0, 5]) {
            rotate([90, 0, 0]) {
                cylinder(r = 2, h = 100, center = true);
            }
        }

        rudder_holder();
    }

    if (DEBUG) {
        rudder_holder();
    }
}

module rudder(length = 8) {

    // Rudder
    RUDDER_HEIGHT = 4;
    RUDDER_CYLINDER_HEIGHT = RUDDER_HEIGHT + 2;
    RUDDER_DIAMETER = 6;
    RUDDER_HOLE_DIAMETER = 2;

    difference() {
        union() {
            translate([0, length / 2, RUDDER_HEIGHT / 2]) {
                cube(size = [5.5, length, RUDDER_HEIGHT], center = true);
            }
            cylinder(r = RUDDER_DIAMETER / 2, h = RUDDER_CYLINDER_HEIGHT);
        }

        translate([0, 0, -1]) {
            cylinder(r = RUDDER_HOLE_DIAMETER / 2, h = 100);
        }
    }

    translate([0, length, 0])
    linear_extrude(height = RUDDER_HEIGHT) {
        polygon([[-1.25,0],[1.25,0],[2.25,2],[-2.25,2]]);
    }
}


if (0) {
    translate([-14, -10, 0]) {
        support();

        if (DEBUG) {
            color("GREY")
                translate([SERVO_HOLDER_WIDTH - 27, -1, BODY_HEIGHT - SERVO_HOLDER_HEIGHT - 0.5])
                    rotate([90, 180, 90])
                        futabas3003();
        }
    }
} else {
    rudder();
    //translate([0, 0, 4])    rotate([0, 180, 0]) servo_holder(true);
    //translate([30, 0, 4])   rotate([0, 180, 0]) servo_holder(false);
}

