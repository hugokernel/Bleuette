
use <spacer.scad>
use <lib/polyScrewThread.scad>

$fn = 50;

ARM_THICKNESS = 3.8;

ARMS_SPACING = 20;
ARM_WIDTH = 20;

SUPPORT_HEIGHT = 30;
SUPPORT_DIAMETER = 7.7;

NUT_DIAMETER = 6;
NUT_HEIGHT = 1.9;
SCREW_DIAMETER = 2.7;

EXTERNAL_DIAMETER = 25;

DEBUG = false;

module piston() {
    button_clear = 0.4;
    difference() {
        union() {
            difference() {
                cylinder(r = ARMS_SPACING / 2, h = SUPPORT_HEIGHT);

                // 1
                translate([0, 0, SUPPORT_HEIGHT - 17]) {
                    cylinder(r = 3.5 + button_clear, h = 17);
                }

                // 2
                cylinder(r = 2.5, h = SUPPORT_HEIGHT);

                // 3
                cylinder(r = 6.5, h = 11.5);
            }

            translate([0, 0, 2.5]) {
                torus(9.3);
            }

            translate([0, 0, 13.5]) {
                torus(9.3);
            }
        }

        holes();
    }
}

module foot() {

    clear = 0.1;
    thread_height = 3;

    difference() {
        translate([0, 0, 12]) {

            difference() {
                union() {
                    difference() {
                        sphere(r = 25, $fn = 20);
                        sphere(r = 24, $fn = 20);
                    }

                    cylinder(r = EXTERNAL_DIAMETER / 2, h = 17);
                }

                translate([0, 0, 38]) {
                    cube(size = [100, 100, 100], center = true);
                }

                cylinder(r = EXTERNAL_DIAMETER / 2 + clear, h = 17);
            }

        }

        translate([0, 0, - thread_height]) {
            cylinder(r = EXTERNAL_DIAMETER / 2 + clear, h = thread_height);
        }
    }

    translate([0, 0, -6]) {
        cylinder(r = EXTERNAL_DIAMETER / 2 + 3, h = 6);
    }
}

module external() {
    clear = 0.4;
    difference() {
        cylinder(r = EXTERNAL_DIAMETER / 2, h = 17);

        translate([0, 0, -0.5]) {
            cylinder(r = ARMS_SPACING / 2 + clear, h = SUPPORT_HEIGHT + 1);
        }

        translate([0, 0, 3]) {
            torus(10.5, 1.2);
        }

        translate([0, 0, 14]) {
            torus(10.5, 1.2);
        }
    }

    translate([0, 0, 1]) {
        torus(11.6, 1.2);
    }

    translate([0, 0, -3]) {
        cylinder(r = EXTERNAL_DIAMETER / 2, h = 3);
    }

    cylinder(r = 3, h = 7);
}

module torus(size = 5.5, radius = 1) {
    rotate_extrude(convexity = 10, $fn = 100) {
        translate([size, 0, 0]) {
            circle(r = radius, $fn = 100);
        }
    }
}

module hole(complete = true) {
    if (complete) {
        cylinder(r = 1.6, h = SUPPORT_HEIGHT);
        cylinder(r = 2.7, h = 15.5);
    } else {
        cylinder(r = 2.7, h = 8.5);
    }
}

module holes(complete = true) {
    // Holes
    translate([0, 6.3, 0]) {
        hole(complete);
    }

    translate([0, -6.3, 0]) {
        hole(complete);
    }

    translate([6.3, 0, 0]) {
        hole(complete);
    }

    translate([-6.3, 0, 0]) {
        hole(complete);
    }
}

if (0) {
    foot();
} else 
if (0) {
    piston();
} else if (0) {
    external();
} else if (1) {

    difference() {
        translate([0, 0, 4]) {
            foot();
        }

        color("GREEN")
        translate([0, 0, -1]) {
            cylinder(r = EXTERNAL_DIAMETER / 2, h = 50);
            external();
        }
    }

} else {

    /*
    intersection() {
        translate([0, -25, -25]) {
            cube(size = [50, 50, 80]);
        }
*/
        union() {
            color("WHITE") piston();

            translate([0, 0, -1]) {
                color("GREY")
                external();
                translate([0, 0, 4]) {
                    color("GREY")
                    foot();
                }
            }
        }
    //}
}

