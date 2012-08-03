
use <lib/servos.scad>
use <body.scad>

$fn = 50;

SUPPORT_SPACE = 65.0;

BODY_THICKNESS = 6;
BODY_LEG_HOLE_DIAMETER = 5.5;
BODY_LEG_HOLE_HEAD_SCREW_HEIGHT = 3.3;
BODY_LEG_HOLE_HEAD_SCREW_DIAMETER = 10;

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
            }
        }

        child();
    }
}


if (1) {
    rotate([180, 0, 0]) {
        show() {
            translate([140, 50, 0]) {
                cube(size = [120, 120, 50], center = true);
            }
        }
    }
} else {
    show() {
        translate([140, 50, 50]) {
            cube(size = [120, 120, 50], center = true);
        }
    }
}

