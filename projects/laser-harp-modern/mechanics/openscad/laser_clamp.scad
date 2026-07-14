// Parametric clamp for a low-power cylindrical laser module.
laser_d = 12.0;
clearance = 0.35;
wall = 3.0;
length = 28.0;
base_w = 26.0;
base_l = 34.0;
mount_hole_d = 3.4;
$fn = 80;

difference() {
  union() {
    rotate([90,0,0]) cylinder(h=length, d=laser_d + 2*wall, center=true);
    translate([-base_w/2,-base_l/2,-laser_d/2-wall]) cube([base_w,base_l,5]);
  }
  rotate([90,0,0]) cylinder(h=length+2, d=laser_d+clearance, center=true);
  translate([0,0,-20]) cube([1.4,40,40], center=true);
  for (x=[-8,8]) translate([x,0,-laser_d/2-wall-1]) cylinder(h=8,d=mount_hole_d);
}
