
use <spacer.scad>
use <lib/polyScrewThread.scad>

$fn = 50;

ARM_THICKNESS = 3.8;

ARMS_SPACING = 20;
ARM_WIDTH = 20;

SUPPORT_HEIGHT = 25;
SUPPORT_DIAMETER = 7.7;

SCREW_DIAMETER = 2.7;

module sensor_support_top() {
    clear = 0.2;
    difference() {
        cylinder(r = ARMS_SPACING / 2, h = SUPPORT_HEIGHT);

        // 1
        translate([0, 0, SUPPORT_HEIGHT - 15]) {
            cylinder(r = 3.7, h = 15);
        }

        // 2
        cylinder(r = 2.5, h = SUPPORT_HEIGHT);

        // 3
        cylinder(r = 6.5, h = 8.5);

        holes();

        torus(7);
    }
}

module sensor() {
    clear = 0.92;

    scale([clear, clear, clear]) {
        cylinder(r = 6.5, h = 8.5);
        holes(false);
    }

    torus();
}

module torus(size = 5.5) {
    translate([0, 0, 8.5 / 2]) {
        rotate_extrude(convexity = 10, $fn = 100) {
            translate([size, 0, 0]) {
                circle(r = 1, $fn = 100);
            }
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
        cylinder(r = 1.5, h = SUPPORT_HEIGHT);
        cylinder(r = 2.5, h = 15.5);
    } else {
        cylinder(r = 2.5, h = 8.5);
    }
}

module thread() {
    /* Bolt parameters.
    *
    * Just how thick is the head.
    * The other parameters, common to bolt and nut, are defined into k_cyl() module
    */
    b_hg=0; //distance of knurled head

$fn = 36;
PI=3.141592;
    /* Screw thread parameters
    */
    t_od=13; // Thread outer diameter
    t_st=2.5; // Step/traveling per turn
    t_lf=55; // Step angle degrees
    t_ln=7.5; // Length of the threade section
    t_rs=PI/2; // Resolution
    t_se=1; // Thread ends style
    t_gp=0; // Gap between nut and bolt threads

    screw_thread(t_od+t_gp, t_st, t_lf, t_ln, t_rs, t_se);
}

module base() {
    translate([0, 0, -7.5]) {
        //thread();
    }

    sensor();
}

base();

//sensor_support_top();

