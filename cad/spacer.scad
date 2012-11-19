
$fn = 30;
SPACER_LENGTH = 6;

ARM_THICKNESS = 3.8;

ARMS_SPACING = 20;
ARM_WIDTH = 20;

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
                    //%cube(size = [ length, SPACER_LENGTH - SPACE, thickness ], center = true);
                }

                translate([ - length / 2, 0, thickness / 2 - 0.01 ]) {
                    rotate([90, 0, 90])
                    linear_extrude(height = length) {
                        polygon([[-1.5,0],[-(SPACER_LENGTH - SPACE) / 2, thickness + 0.01],[(SPACER_LENGTH - SPACE) / 2,thickness + 0.01],[1.5,0]]);
                    }
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

if (1) {
    // Small
    //spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS, true);
    spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS, false);

    // Large
    //spacer(ARMS_SPACING + ARM_THICKNESS * 2 - 1, 15, ARM_THICKNESS);
}

