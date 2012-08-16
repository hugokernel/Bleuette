use <lib/servo_arm.scad>

module short() {
    servo_standard(8.5, 1);
}

module long() {
    servo_standard(14, 1);
}

rotate([0, 180, 0])
    long();
    //short();

