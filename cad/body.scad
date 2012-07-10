
$fn = 20;

BODY_THICKNESS = 5;
BODY_SERVO_HOLE_DIAMETER = 6;

module leg_support() {

    width = 25;
    length = 30;

    translate([0, width / 2 , 0]) {
        difference() {
            hull() {
                cube(size = [length, width, BODY_THICKNESS], center = true);

                translate([0, length / 2, 0]) {
                    cylinder(r = length / 2, h = BODY_THICKNESS, center = true);
                }
            }

            translate([0, length / 2, 0]) {
                cylinder(r = BODY_SERVO_HOLE_DIAMETER / 2, h = BODY_THICKNESS, center = true);
            }
        }
    }
}

module body() {

    width = 100;
    length = 320;
    thickness = BODY_THICKNESS;

    cube(size = [length, width, thickness], center = true);

    translate([0, width / 2, 0]) {
        leg_support();
    }

    translate([0, - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support();
        }
    }

    translate([length / 2 - 30, width / 2, 0]) {
        leg_support();
    }

    translate([length / 2 - 30, - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support();
        }
    }

    translate([- (length / 2 - 30), width / 2, 0]) {
        leg_support();
    }

    translate([- (length / 2 - 30), - width / 2, 0]) {
        rotate([0, 0, 180]) {
            leg_support();
        }
    }
}

body();

