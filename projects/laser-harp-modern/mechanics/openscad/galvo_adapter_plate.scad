// Flat adapter only. Replace hole coordinates with measurements from the purchased galvo block.
plate_w = 90;
plate_h = 70;
plate_t = 5;
corner_hole_d = 4.3;
galvo_hole_d = 3.4;
galvo_holes = [[-20,-15],[20,-15],[-20,15],[20,15]];
$fn=60;

difference() {
  translate([-plate_w/2,-plate_h/2,0]) cube([plate_w,plate_h,plate_t]);
  for (x=[-plate_w/2+8,plate_w/2-8])
    for (y=[-plate_h/2+8,plate_h/2-8])
      translate([x,y,-1]) cylinder(h=plate_t+2,d=corner_hole_d);
  for (p=galvo_holes)
    translate([p[0],p[1],-1]) cylinder(h=plate_t+2,d=galvo_hole_d);
}
