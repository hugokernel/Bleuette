
use <lib/spring.scad>
use <spacer.scad>

$fn = 30;
ARM_THICKNESS = 3.8;
ARMS_SPACING = 20;

DEBUG = true;

module spring(only_connection = false) {
    thickness = ARM_THICKNESS;
    connection_radius = 1.5; // Todo: voir la taille des vis / foret
    connection_height = ARM_THICKNESS + (only_connection ? 5 : 0);
    offset = only_connection ? 10 : 0;
    clear = 1.03;

    if (!only_connection) {

        difference() {
            translate([0, 7, ARM_THICKNESS / 2]) {
                cube(size = [14.2, 5, ARM_THICKNESS], center = true);
            }

            rotate([0, 90, -90]) {
                translate([9.9, 0, -11.4]) {
                    scale([clear, clear, clear]) {
                        spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
                    }

                    if (DEBUG) {
                        %spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
                    }
                }
            }
        }

        difference() {
            translate([0, -6, thickness / 2]) {
                spring_square(fid=1, th=2, folds=6, lx=14.2, lz=thickness, drill=0);
            }
        }
    }
}

if (1) {
    intersection() {
        translate([0, 8, 0]) {
            cube(size = [20, 10, 10], center = true);
        }
        spring();
    }
}

