
use <servos.scad>
use <body.scad>
use <leg_holder.scad>
use <leg.scad>
use <demo_leg.scad>

SUPPORT_SPACE = 65.0;

module legs() {
    translate([15, 165, 0]) {
        rotate([0, 0, 90]) {
            leg();
        }
    }

    translate([145, 165, 0]) {
        rotate([0, 0, 90]) {
            leg();
        }
    }

    translate([-115, 165, 0]) {
        rotate([0, 0, 90]) {
            leg();
        }
    }
}

// Create body
body();
translate([0, 0, SUPPORT_SPACE]) {
    body_support();
}

// Create leg
legs();
mirror([0, 1, 0]) {
    legs();
}

