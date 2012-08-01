
use <lib/servos.scad>
use <leg_holder.scad>
use <leg.scad>

ARMS_SPACING = 20;
ARM_THICKNESS = 4;

SERVO_HOLDER_HEIGHT = 11.5;
SERVO_HOLDER_WIDTH = 16;

BODY_HEIGHT = 65.0;

ARM_WIDTH = 20;

DEBUG = 1;

$fn = 20;

module mounted_arm() {

    translate([ 0, 0, ARM_THICKNESS / 2 ]) {
        main_arm();

        translate([ 0, 0, ARMS_SPACING + ARM_THICKNESS]) {
            rotate([ 0, 0, 0 ]) {
                main_arm();
            }
        }

        translate([ 0, -33, ARMS_SPACING / 2 + ARM_THICKNESS / 2 ]) {
            rotate([ 0, 90, 90 ]) {
                spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS);
            }
        }

        translate([ 0, 45, ARMS_SPACING / 2 + ARM_THICKNESS / 2 ]) {
            rotate([ 0, 90, -90 ]) {
                spacer(ARMS_SPACING, ARM_WIDTH, ARM_THICKNESS);
            }
        }
    }

    // Top arm
    union() {
        translate([ -69, -52, - ARM_THICKNESS]) {
            rotate([ 0, 0, 90]) {
                long_arm();
            }

            translate([ 27, 0, 18 ]) {
                rotate([ 0, 90, 0 ]) {
                    spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
                }
            }
        }

        translate([ -42, -52, ARMS_SPACING + ARM_THICKNESS * 2]) {
            rotate([ 0, 0, 90]) {
                short_arm();
            }
        }
    }

    // Bottom arm
    union() {
        translate([ -42, -12, ARMS_SPACING + ARM_THICKNESS * 2]) {
            rotate([ 0, 0, 90]) {
                short_arm();
            }
        }

        translate([ -42, -12, - ARM_THICKNESS]) {
            rotate([ 0, 0, 90]) {
                short_arm();
            }

            translate([ 0, 0, 18 ]) {
                rotate([ 0, 90, 0 ]) {
                    spacer(ARMS_SPACING + ARM_THICKNESS * 2, 15, ARM_THICKNESS);
                }
            }
        }
    }
}

module leg() {
    translate([-100, ARMS_SPACING + 8, 0]) {
        rotate([0, 0, -90]) {
            support();

            translate([SERVO_HOLDER_WIDTH - 27, -1, BODY_HEIGHT - SERVO_HOLDER_HEIGHT - 0.5])
                rotate([90, 180, 90])
                    futabas3003();
        }
    }

    rotate([ -90, 0, 0]) {
        mounted_arm();
    }
}

leg();

