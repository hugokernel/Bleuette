
use <spacer.scad>

$fn = 50;

ARM_THICKNESS = 3.8;

ARMS_SPACING = 20;
ARM_WIDTH = 20;

SUPPORT_HEIGHT = 15;
SUPPORT_DIAMETER = 7.7;

SCREW_DIAMETER = 2.7;

module sensor_support_top() {
    clear = 0.2;
    difference() {
        union() {
            cylinder(r = ARMS_SPACING / 2, h = 3);

            translate([6.3, 0, 0]) {
                cylinder(r = SCREW_DIAMETER / 2 - clear, h = 10);
            }

            translate([-6.3, 0, 0]) {
                cylinder(r = SCREW_DIAMETER / 2 - clear, h = 10);
            }
        }

        // Holes
        translate([0, 6.3, -1]) {
            cylinder(r = 1.5, h = 30);
        }

        translate([0, -6.3, -1]) {
            cylinder(r = 1.5, h = 30);
        }

        poutre();
    }
}

module poutre() {
    cylinder(r = SUPPORT_DIAMETER / 2, h = SUPPORT_HEIGHT);
}

module sensor_support() {
    difference() {
        union() {
            poutre();
            cylinder(r = 10.5, h = 3);
            cylinder(r = 11.3, h = 1);
        }

        // Holes
        translate([0, 6.3, -1]) {
            cylinder(r = 2.3, h = 30);
        }

        translate([0, -6.3, -1]) {
            cylinder(r = 2.3, h = 30);
        }

        // Passe cable
        translate([0, 0, -1]) {
            cylinder(r = 2.5, h = 30);
        }
    }
}

module sensor() {
    translate([0, 0, 6]) {
        difference() {
            sphere(r = 13);
            sphere(r = 12);
            translate([0, 0, 6]) {
                cube(size = [30, 30, 20], center = true);
            }

            translate([0, 0, -6]) {
                sensor_support();
            }
        }
    }
}

translate([0, 0, SUPPORT_HEIGHT + 5]) {
    %spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS, true);
}

module sensor_all() {
    sensor();
    sensor_support();

    translate([0, 0, SUPPORT_HEIGHT]) {
        sensor_support_top();
    }
}

PRINT = -1;

if (PRINT == 0) {
    sensor();
} else if (PRINT == 1) {
    sensor_support();
} else if (PRINT == 2) {
    sensor_support_top();
} else {
    sensor_all();
}

