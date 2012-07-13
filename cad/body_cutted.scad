
use <body.scad>;
use <cutter.scad>;

CLEAR = 0.1;
BODY_THICKNESS = 5;
BODY_WIDTH = 100;
BODY_LENGTH = 320;

module part0() {
    cutter(- BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10], false) {
        rotate([0, 0, 90]) body();
    }
}

module part1() {
    cutter(- BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10]) {
        rotate([0, 0, 90]) body();
    }

/*
    cutter(BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10]) {
        rotate([0, 0, 90]) body();
    }

    cutter(BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10], false) {
        rotate([0, 0, 90]) body();
    }
*/
}

module part2() {
    cutter(- BODY_LENGTH / (3 * 2), [BODY_WIDTH, BODY_LENGTH, BODY_THICKNESS], [3, 10, CLEAR], [300, 300, 10]) {
        rotate([0, 0, 90]) body();
    }
}

render() {
    //part0();
    //part1();
    part2();
};

