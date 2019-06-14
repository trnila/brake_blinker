w=46 + 0.1;
h=26 + 0.1;
depth = 6.5+3+3;
wall = 2;

full_w = w + 2*wall;
full_depth = depth + 2*wall;
full_h = h + 2*wall;

paska_w = 2+0.2;
paska_thick = 1+0.2;
paska_offset=4;
paska_spacing = 3;

module paska() {
  cube([paska_thick, paska_w, 10]);    
  translate([paska_spacing, 0, 0]) cube([paska_thick, paska_w, 10]);    
};

difference() {
  cube([w+2*wall, depth+wall, h+2*wall]);
  union() {
    // back side
    translate([wall, 0, wall]) cube([w, depth, h]);
    
    // wires side
    translate([-wall, wall, wall]) cube([2*wall, depth-wall, h]);
    
    // front side
    translate([2*wall, depth, 2*wall]) cube([w-2*wall, wall, h-2*wall]);
    
    translate([0, full_depth/2-paska_w, 0]) {
      translate([paska_offset, 0, 0]) paska();
      translate([full_w - paska_offset - paska_spacing - 2*paska_thick, 0, 0]) paska();
    }
  }
}
