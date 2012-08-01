
/**
 * Futaba S3003 servo
 *
 * @param vector position The position vector
 * @param vector rotation The rotation vector
 */
module futabas3003(position, rotation)
{
	translate(position)
	{
		rotate(rotation)
	    {
			union()
			{
				// Box and ears
				translate([0,0,0])
				{
					cube([20.1, 39.9, 36.1], false);
					translate([1.1, -7.6, 26.6])
					{
                        difference() {
						    cube([18, 7.6, 2.5]);
                            translate([4, 3.5, 0]) cylinder(100, 2);
                            translate([14, 3.5, 0]) cylinder(100, 2);
                        }

                        for (i = [0 : $children - 1]) {
                            translate([4, 3.5, 0]) child(i);
                            translate([14, 3.5, 0]) child(i);
                        }
					}

					translate([1.1, 39.9, 26.6])
					{
                        difference() {
                            cube([18, 7.6, 2.5]);
                            translate([4, 4.5, 0]) cylinder(100, 2);
                            translate([14, 4.5, 0]) cylinder(100, 2);
                        }

                        for (i = [0 : $children - 1]) {
                            translate([4, 4.5, 0]) child(i);
                            translate([14, 4.5, 0]) child(i);
                        }
                    }
				}

				// Main axle
				translate([10, 30, 36.1])
				{
					cylinder(r=6, h=0.4, $fn=30);
					cylinder(r=2.5, h=4.9, $fn=20);
				}
			}
		}
	}
}

