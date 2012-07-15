
use <body.scad>;

DEMO = true;

/**
 *  Create one teeth
 */
module teeth(width, height, thickness) {
    offset = width / 3;
    translate([- width / 2, - height / 2, 0]) {
        linear_extrude(height = thickness) {
            polygon([[0, 0], [width, 0], [width - offset, height], [offset, height]]);
        }
    }
}

/**
 *  Zip length
 *  - width of zip
 *  - teeth count
 *  - teeth height
 *  - thickness
 *  - clear
 *  - recto
 */
module zip(width, count, teeth_height, teeth_thickness, clear = 0.1, this = true) {

    clear = - clear;

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

    // Always width / 3 (ref. teeth())
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
                    translate([x, 0, 0]) {
                        rotate([0, 0, 180]) {
                            if (this == true) {
                                color("BLUE") teeth(teeth_width - clear, teeth_height - clear, teeth_thickness);
                            }
                        }
                    }
                }
            } else {
                assign(x = start_at + y / 2 + (teeth_width - offset + clear) * i) {
                    translate([x, 0, 0]) {
                        if (this == false) {
                            color("RED") teeth(teeth_width - clear, teeth_height - clear, teeth_thickness);
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
 *  - teeths (count, height)
 *  - dimension of exclude object
 *  - which parts ?
 */
module cutter(y, dimension, teeths, exclude_dimension, which = true) {

    // Todo: test if child is present
    
    difference() {
        child(0);

        translate([0, y, 0]) {
            zip(dimension[0], teeths[0], teeths[1], dimension[2], teeths[2], which);

            if (which) {
                translate([- exclude_dimension[0] / 2, - (teeths[1]/ 2 - 0.1) - exclude_dimension[1], - exclude_dimension[2] / 2]) {
                    cube(size = exclude_dimension);
                }
            } else {
                translate([- exclude_dimension[0] / 2, teeths[1] / 2 - 0.1, , - exclude_dimension[2] / 2]) {
                    cube(size = exclude_dimension);
                }
            }
        }
    }
}

//cube(size = [50, 50, 5], center = true);


//zip(50, 5, 5, 5, 0.1, false);
//teeth(20, 5, 3);

CLEAR = 0.1;

BODY_THICKNESS = 5;
BODY_WIDTH = 100;
BODY_LENGTH = 320;

/*
render() {
    cutter(- BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10]) {
        rotate([0, 0, 90]) body();
    }

    cutter(- BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10], false) {
        rotate([0, 0, 90]) body();
    }
};
*/


dim = [20, 20, 5];
if (DEMO) {
    render() {
        if (1) {
            cutter(0, dim, [2, 4, CLEAR], dim) {
                cube(size = dim, center = true);
            }
        } else {
            cutter(0, dim, [2, 4, CLEAR], dim, false) {
                cube(size = dim, center = true);
            }
        }
    }
}

