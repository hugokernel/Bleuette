
use <lib/spring.scad>
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
        }

        // Hole
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

module spacer(length, width, thickness, support = false) {

    BLOCKER_THICKNESS = 2;
    SPACE = 0.3;
    CLEAR = 0.7;

    MAIN_HOLE_DIAMETER = 2.5;

    SCREW_DIAMETER = 2.7;
    NUT_DIAMETER = 6;
    NUT_HEIGHT = 1.9;

    difference() {
        union() {
            cube([ length, width, thickness ], center = true);

            translate([0, 0, - SPACE / 2]) {
                cube(size = [ length + thickness * 2 + CLEAR * 2, SPACER_LENGTH - SPACE, ARM_THICKNESS - SPACE ], center = true);
            }

            if (support) {
                translate([ 0, 0, thickness / 2]) {
                    cylinder(r = 10, h = thickness);
                }
            } else {
                translate([ 0, 0, thickness ]) {
                    cube(size = [ length, SPACER_LENGTH - SPACE, thickness ], center = true);
                }
                
                translate([ 0, 0, thickness / 2]) {
                    cylinder(r = 5, h = thickness);
                }
            }

            // Blocker
            translate([ - (length + thickness * 2) / 2 - BLOCKER_THICKNESS / 2 - CLEAR, 0, -SPACE / 2 ]) {
                cube(size = [ BLOCKER_THICKNESS, SPACER_LENGTH - SPACE + 2, ARM_THICKNESS - SPACE ], center = true);
            }

            translate([ (length + thickness * 2) / 2 + BLOCKER_THICKNESS / 2 + CLEAR, 0, -SPACE / 2 ]) {
                cube(size = [ BLOCKER_THICKNESS, SPACER_LENGTH - SPACE + 2, ARM_THICKNESS - SPACE ], center = true);
            }
        }

        // Main hole
        translate([ 0, 0, -10]) {
            cylinder(r = MAIN_HOLE_DIAMETER, h = 20);
        }

        if (support) {
            // Small hole
            for (i = [0 : 3]) {
                rotate([0, 0, i * 90]) {

                    translate([-(length / 4 + MAIN_HOLE_DIAMETER / 2), 0, -5]) {
                        cylinder(r = SCREW_DIAMETER / 2, h = 20);

                        // Nut
                        translate([0, 0, ARM_THICKNESS + 5 + 0.01]) {
                            rotate([0, 0, 90])
                                cylinder(r = NUT_DIAMETER / 2, h = NUT_HEIGHT, $fn = 6);
                        }
                    }
                }
            }
        }
    }
}

module spring(only_connection = false) {
    thickness = ARM_THICKNESS;
    connection_radius = 1.2;
    connection_height = ARM_THICKNESS + (only_connection ? 5 : 0);
    offset = only_connection ? 10 : 0;

    if (!only_connection) {
        translate([5, 0, 0]) {
            linear_extrude(height = thickness) {
                polygon([[-10,0],[-10,6],[-6,10],[-4,10],[0,6],[0,0]]);
            }
        }

        difference() {
            translate([0, -7, thickness / 2]) {
                spring_square(fid=1, th=1, folds=7, lx=14.2, lz=thickness, drill=0);
            }

            /*
            translate([-7.5, 1, 1]) {
                cube(size = [5, 3, thickness * 2], center = true);
            }
            */
        }
    }

    translate([0, 7.5, -connection_height]) {
        // Spring connection
       cylinder(h = connection_height + offset, r = connection_radius);

        translate([0, -5, 0]) {
            cylinder(h = connection_height + offset, r = connection_radius);
        }
    }
}

if (0) {
    main_arm();

    translate([30, 0, 0]) {
        long_arm();
    }

    // Basic arm
    translate([60, -27, 0]) {
        short_arm();
    }

    translate([70, 40, 0]) {
        rotate([0, 0, 0]) {
            spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS);

            translate([0, 30, 0]) {
                spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
            }
        }
    }
} else {
    //main_arm();
    //long_arm();
    short_arm();

    translate([0, -20, ARM_THICKNESS]) {
        color("BLUE")
        spring();
    }

    //spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS, true);
    //spacer(ARMS_SPACING + ARM_THICKNESS * 2 - 1, 15, ARM_THICKNESS);
}

