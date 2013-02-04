
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

DEBUG = false;

module piston() {
    clear = 0.2;
    difference() {
        cylinder(r = ARMS_SPACING / 2, h = SUPPORT_HEIGHT);

        // 1
        translate([0, 0, SUPPORT_HEIGHT - 20]) {
            cylinder(r = 3.7, h = 20);
        }

        // 2
        cylinder(r = 2.5, h = SUPPORT_HEIGHT);

        // 3
        cylinder(r = 6.5, h = 8.5);

        holes();
    }

    translate([0, 0, 2.5]) {
        torus(9.4);
    }

    translate([0, 0, 13.5]) {
        torus(9.4);
    }
}

module foot() {
    difference() {
        sphere(r = 25);
        translate([0, 0, 38]) {
            cube(size = [100, 100, 100], center = true);
        }

        cylinder(r = 12.5, h = 17);
    }
}

module external(filled = false) {
    clear = 0.3;
    difference() {
        cylinder(r = 12.5, h = 17);

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

    difference() {
        translate([0, 0, -7.5]) {
            thread(25, 7.5);
        }

        if (!filled) {
            translate([0, 0, -7.6]) {
                cylinder(r = SCREW_DIAMETER / 2, h = 20);
            }

            translate([0, 0, -NUT_HEIGHT]) {
                cylinder(r = NUT_DIAMETER / 2, h = NUT_HEIGHT, $fn = 6);
            }

            translate([0, 0, -7.5]) {
                cylinder(r = 4, h = 4);
            }
        }
    }
}

/*
module sensor() {
    clear = 0.92;

    scale([clear, clear, clear]) {
        cylinder(r = 6.5, h = 8.5);
        holes(false);
    }

    translate([0, 0, 8.5 / 2]) {
        torus();
    }
}
*/

module torus(size = 5.5, radius = 1) {
    rotate_extrude(convexity = 10, $fn = 100) {
        translate([size, 0, 0]) {
            circle(r = radius, $fn = 100);
        }
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

module hole(complete = true) {
    if (complete) {
        cylinder(r = 1.6, h = SUPPORT_HEIGHT);
        cylinder(r = 2.7, h = 15.5);
    } else {
        cylinder(r = 2.7, h = 8.5);
    }
}

module thread(diameter, height) {
    b_hg=0; //distance of knurled head

    $fn = 36;
    PI=3.141592;

    /* Screw thread parameters
    */
    t_od=diameter; // Thread outer diameter
    t_st=2.5; // Step/traveling per turn
    t_lf=55; // Step angle degrees
    t_ln=height; // Length of the threade section
    t_rs=PI/2; // Resolution
    t_se=1; // Thread ends style
    t_gp=0; // Gap between nut and bolt threads

    if (DEBUG) {
        cylinder(r = diameter / 2, h = height);
    } else {
        screw_thread(t_od+t_gp, t_st, t_lf, t_ln, t_rs, t_se);
    }
}

/*
module base() {
    translate([0, 0, -7.5]) {
        //thread();
    }

    sensor();
}
*/

if (0) {
    piston();
} else if (0) {
    external();
} else if (0) {
    difference() {
        translate([0, 0, 12]) {
            foot();
        }

        external(true);
    }
} else {

    intersection() {
        translate([0, -25, -15]) {
            cube(size = [50, 50, 60]);
        }

//    union() {

        union() {
            color("RED") piston();

            color("GREEN")
            translate([0, 0, -1]) {
                external();
                translate([0, 0, 12]) {
                    foot();
                }
            }
        }
    }
}

