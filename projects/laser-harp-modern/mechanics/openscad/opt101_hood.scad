// Light hood. Measure the actual OPT101 breakout before printing.
board_w = 20.0;
board_h = 20.0;
board_t = 2.0;
tube_inner_d = 10.0;
tube_length = 45.0;
wall = 2.4;
clearance = 0.5;
$fn = 80;

difference() {
  union() {
    translate([-(board_w+2*wall)/2,-(board_h+2*wall)/2,0])
      cube([board_w+2*wall,board_h+2*wall,board_t+8]);
    translate([0,0,board_t+8]) cylinder(h=tube_length,d=tube_inner_d+2*wall);
  }
  translate([-(board_w+clearance)/2,-(board_h+clearance)/2,-1])
    cube([board_w+clearance,board_h+clearance,board_t+5]);
  translate([0,0,board_t+5]) cylinder(h=tube_length+5,d=tube_inner_d);
}
