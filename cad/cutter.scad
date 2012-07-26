
use <body.scad>;

DEMO = true;

/**
 *  Create one teeth
 */
module dovetail_teeth(width, height, thickness) {
    offset = width / 3;
    translate([- width / 2, - height / 2, 0]) {
        linear_extrude(height = thickness) {
            polygon([[0, 0], [width, 0], [width - offset, height], [offset, height]]);
        }
    }
}

/**
 *  Dovetail length
 *  - width of dovetail
 *  - teeth count
 *  - teeth height
 *  - thickness
 *  - clear
 *  - recto
 */
module dovetail(width, count, teeth_height, teeth_thickness, clear = 0.1, male = true) {

    clear = clear;

    /**
     * 4 sections :
     *
     * |1/2         1/2|
     * |--+ <- 3 -> +--|--+         +--
     * |   \1  1  1/   |   \       /
     * |    \_____/    |    \_____/
     */
    y = ((width - count * 2 * clear) / count) / 4;
    teeth_width = y * 3;

    // Always width / 3 (ref. dovetail_teeth())
    offset = teeth_width / 3;
    start_at = clear + teeth_width / 2 - width / 2;

    translate([0, 0, - teeth_thickness / 2]) {

        // Debug purpose only
        % union() {
            translate([- width / 2, 0, - width / 2]) cube(size = [width, 0.01, width]);
        }

        for (i = [-1 : count * 2 - 1] ) {
            if (i % 2) {
                assign(x = start_at + y / 2 + (teeth_width - offset + clear) * i + 0.1) {
                    translate([x, - clear, 0]) {
                        rotate([0, 0, 180]) {
                            if (male == true) {
                                color("BLUE") dovetail_teeth(teeth_width - clear, teeth_height - clear, teeth_thickness);
                            }
                        }
                    }
                }
            } else {
                assign(x = start_at + y / 2 + (teeth_width - offset + clear) * i) {
                    translate([x, clear, 0]) {
                        if (male == false) {
                            color("RED") dovetail_teeth(teeth_width - clear, teeth_height - clear, teeth_thickness);
                        }
                    }
                }
            }
        }
    }
}

/**
 *  Cutter
 *  - y position of cut
 *  - dimension (width, length, thickness)
 *  - teeths (count, height, clear)
 *  - dimension of excluded object
 *  - male or female parts ?
 */
module cutter(position, dimension, teeths, male = true) {

    translate(position) {
        dovetail(dimension[0], teeths[0], teeths[1], dimension[2], teeths[2], male);

        if (male) {
            translate([- dimension[0] / 2, - (teeths[1] / 2 - 0.1) - dimension[1], - dimension[2] / 2]) {
                cube(size = dimension);
            }
        } else {
            translate([- dimension[0] / 2, teeths[1] / 2 - 0.1, , - dimension[2] / 2]) {
                cube(size = dimension);
            }
        }
    }
}


module demo() {
    dim = [20, 60, 5];
    clear = 0.1;

    % cube(size = dim, center = true);

    color("GREEN") intersection() {
        cube(size = dim, center = true);
        cutter([0, -10, 0], dim, [2, 4, clear], true);
    }

    //translate([22, 0, 0])
    color("RED") intersection() {
        cube(size = dim, center = true);
        cutter([0, -10, 0], dim, [2, 4, clear], false);
        cutter([0, 10, 0], dim, [2, 4, clear], true);
    }

    //translate([44, 0, 0])
    color("BLUE") intersection() {
        cube(size = dim, center = true);
        cutter([0, 10, 0], dim, [2, 4, clear], false);
    }
}

demo();

