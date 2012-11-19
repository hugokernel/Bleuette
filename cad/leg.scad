
use <spring.scad>
use <spacer.scad>
$fn = 30;

SPACER_LENGTH = 6;

ARMS_SPACING = 20;

ARM_THICKNESS = 3.8;
ARM_WIDTH = 20;

module arm(width, length, height, hole = [5, 5], gap_width = 0, gap_length = 0) {

    difference() {
        hull() {
            translate([0, - length / 2, 0])
                cylinder(h = height, r1 = width / 2, r2 = width / 2);

            translate([0, length / 2, 0])
                cylinder(h = height, r1 = width / 2, r2 = width / 2);
        }

        // Hole
        translate([0, - length / 2, -1]) {
            cylinder(h = height + 2, r1 = hole[0] / 2, r2 = hole[0] / 2);
        }

        translate([0, length / 2, -1]) {
            cylinder(h = height + 2, r1 = hole[1] / 2, r2 = hole[1] / 2);
        }

        if (gap_width) {
            difference() {
                translate([0, 0,  height / 2])
                    cube(size = [gap_width, length - width, height * 2], center = true);
                translate([0, - length / 2, 0])
                    cylinder(h = height * 2, r1 = width / 2 + gap_length, r2 = width / 2 + gap_length);
                translate([0, length / 2, 0])
                    cylinder(h = height * 2, r1 = width / 2 + gap_length, r2 = width / 2 + gap_length);
            }
        }
    }
}

// Long arm
module long_arm() {
    difference() {
        arm(15, 135, ARM_THICKNESS, [5, 2], 0, 0);

        translate([0, 12.5, -1]) {
            cylinder(h = 20, r1 = 2.5, r2 = 2.5);
        }

        translate([0, -27, 0]) {
            connection();
        }

        translate([0, -47, ARM_THICKNESS]) {
            spring(true);
        }
    }
}

module short_arm() {
    difference() {
        arm(15, 80, ARM_THICKNESS, [5, 5], 0, 0);
        connection();

        translate([0, -20, ARM_THICKNESS]) {
            spring(true);
        }
    }
}

module connection() {
    translate([0, -4, 0]) {
        cube(size = [ ARM_THICKNESS, SPACER_LENGTH, 10 ], center = true);
    }

    cube(size = [ SPACER_LENGTH, ARM_THICKNESS, 10 ], center = true);

    cylinder(r = SPACER_LENGTH / 2 + 0.3, h = 16, center = true);
}

module main_arm() {

    size = [ ARM_WIDTH, 120, ARM_THICKNESS ];
    radius = 5;
    oblong_length = 3;

    difference() {
        union() {
            // From MCAD library
            cube(size - [2*radius,0,0], true);
            cube(size - [0,2*radius,0], true);
            for (x = [radius-size[0]/2, -radius+size[0]/2],
                   y = [radius-size[1]/2, -radius+size[1]/2]) {
                translate([x,y,0]) cylinder(r=radius, h=size[2], center=true);
            }
            
            difference() {
                translate([ size[0] / 2 - 12, - size[1] / 2 + 7.5, - ARM_THICKNESS / 2]) {
                    cylinder(h = ARM_THICKNESS, r1 = 17, r2 = 17);
                }

                translate([ size[0] / 2 - 30, - size[1] / 2 - 10, - ARM_THICKNESS / 2 - 0.1]) {
                    cube(size = [10, 50, 10]);
                }

                translate([ size[0] / 2 - 30, - size[1] / 2 - 15, - ARM_THICKNESS / 2 - 0.1]) {
                    cube(size = [50, 10, 10]);
                }
            }

            translate([0, - 60, - ARM_THICKNESS / 2]) {
                arm(ARM_WIDTH, 10, ARM_THICKNESS, [0, 0]);
            }
        }

        // Hole : oblong
        union() {
            hull() {
                translate([ size[0] / 2 - 12, - size[1] / 2 + 7.5, -5]) {
                    cylinder(h = 10, r1 = 2.5, r2 = 2.5);
                }

                translate([ size[0] / 2 - 12 + oblong_length, - size[1] / 2 + 7.5, -5]) {
                    cylinder(h = 10, r1 = 2.5, r2 = 2.5);
                }
            }

            translate([ size[0] / 2 - 12, - size[1] / 2 + 47.5, -5]) {
                cylinder(h = 10, r1 = 2.5, r2 = 2.5);
            }
        }

        // Spacer hole
        union() {
            translate([0, size[1] / 2 - 15, 0]) {
                connection();
            }

            translate([0, - size[1] / 2 + 27, 0]) {
                connection();
            }
        }
    }
}

if (1) {
    // View all piece...

    translate([-30, 0, 0]) {
        main_arm();
    }

    translate([-5, 0, 0]) {
        main_arm();
    }

    translate([30, 0, 0]) {
        long_arm();
    }

    translate([50, -27, 0]) {
        short_arm();
    }

    // Basic arm
    translate([80, -27, 0]) {
        short_arm();
    }

    translate([100, -27, 0]) {
        short_arm();
    }

    translate([50, 55, ARM_THICKNESS]) {
        rotate([0, 180, 0])
            spring();
    }

    translate([90, 40, 0]) {
        rotate([0, 0, 0]) {
            spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS, true);

            translate([0, 25, 0]) {
                spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS);
            }

            translate([0, 50, 0]) {
                spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
            }

            translate([0, 70, 0]) {
                spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
            }
        }
    }
} else {
    // ... or each one

    //main_arm();
    //long_arm();
    //short_arm();
}

