$fn = $preview?16:32;
eps1 = 0.001;
eps2 = 2*eps1;
nozzle = 0.4;

plate_x = 75.0;
plate_y = 50.0;

// pillars for screws
d1 = 1.9;  // inner diameter pillar
d2 = 4.75; // outer diameter pillar
// screws
dscrew = 25.4;
alpha1 = 90+120;
alpha2 = 90+55;
alpha3 = 90-80;
// screw 1
x1 = dscrew * cos(alpha1);
y1 = dscrew * sin(alpha1);
t1 = (d2-d1)/2; // bump on pillar
cut1 = 0.9;     // bump on pillar
// screw 2
x2 = dscrew * cos(alpha2);
y2 = dscrew * sin(alpha2);
t2 = (d2-d1)/2; // bump on pillar
cut2 = 0.9;     // bump on pillar
// screw 3
x3 = dscrew * cos(alpha3);
y3 = dscrew * sin(alpha3);
t3 = -d2+0.9;   // cut on pillar
cut3 = 4.0;     // cut on pillar

// central hole size
l1 = 7.12;
l2 = 9.37;

h1 = 3.0;
h2 = 4.0;

dmax = 3.0;

// support for attaching arduino nano
pcb_support = true;

pcb_x = 66.0;
pcb_y = 12.0;
pcbscrew_x = pcb_x - 5.0;
pcbscrew_d = 2.0;

module stageplate() {
    difference() {
        stageplate_body();
        stageplate_holes();
    }
}

module stageplate_body() {
    translate([-plate_x/2, -plate_y/2, 0])
    linear_extrude(h1)
    offset(-dmax)
    offset(2*dmax)
    offset(-dmax)
    union() {
        square([plate_x, plate_y]);
        if (pcb_support)
        translate([plate_x, plate_y-pcb_y, 0])
        square([pcb_x, pcb_y]);
    }

    // screws
    translate([x1, y1, h1-eps1])
    difference() {
        union() {
            cylinder(h=h2, d=d2);
            translate([-d2, -d2/2, 0])
            cube([d2, d2, h2]);
        }
        translate([t1-d2/2, -d2, h2-cut1])
        cube([d2, 2*d2, h2]);
    }
    translate([x2, y2, h1-eps1])
    difference() {
        union() {
            cylinder(h=h2, d=d2);
            translate([-d2, -d2/2, 0])
            cube([d2, d2, h2]);
        }
        translate([t2-d2/2, -d2, h2-cut2])
        cube([d2, 2*d2, h2]);
    }
    translate([x3, y3, h1-eps1])
    difference() {
        union() {
            cylinder(h=h2, d=d2);
            translate([0, -d2/2, 0])
            cube([d2, d2, h2]);
        }
        translate([t3-d2/2, -d2/2, h2-cut3])
        cube([d2, d2, h2+eps1]);
    }
}

module stageplate_holes() {
    h = h1+h2+eps2;
    translate([x1, y1, -eps1])
    cylinder(h=h, d=d1);
    translate([x2, y2, -eps1])
    cylinder(h=h, d=d1);
    translate([x3, y3, -eps1])
    cylinder(h=h, d=d1);
    translate([-l1/2, -l2/2, -eps1])
    linear_extrude(h)
    offset(dmax)
    square([l1, l2]);
    // pcb
    if (pcb_support)
    translate([plate_x/2 + pcbscrew_x, plate_y/2-pcb_y/2, -eps1])
    cylinder(h=h, d=d1);
}


stage_x = 75.0;
stage_y = 25.0;
stage_z = 3.0;
foot_x = 13.0 - nozzle;
foot_y = 12.0 - nozzle;
foot_z = 2.2;
chamfer = 1.6;
wall_thickness = 1.6;
module stage(no_hole) {
    difference() {
        stage_body();
        stage_holes(no_hole); // no_hole = true or false
    }
}

module stage_body() {
    translate([-stage_x/2, -stage_y/2, 0])
    cube([stage_x, stage_y, stage_z]);
    translate([-foot_x/2, -foot_y/2, stage_z-eps1])
    cube([foot_x, foot_y, foot_z]);

    hull() {
        translate([0, 0, h1+foot_z+4])
        linear_extrude(eps1)
        offset(chamfer)
        offset(-chamfer)
        square([l1-nozzle, l2-nozzle], center = true);
        translate([0, 0, h1+foot_z])
        linear_extrude(eps1)
        offset(chamfer)
        offset(-chamfer)
        square([l1, l2], center = true);
    }
}

module stage_holes(solid = true) {
    translate([0, 0, solid ? -h1-foot_z-eps1 : 0])
    translate([-l1/2, -l2/2, h1+foot_z-eps1])
    linear_extrude(h1*10)
    offset(wall_thickness)
    offset(-2*wall_thickness)
    square([l1, l2]);
}

module assembly() {
    stageplate();
    translate([0, 0, 10])
    mirror([0,0,1])
    stage(false); // no hole in stage
}

assembly();
//projection()
//stageplate();
//stage(false); // no hole in stage
//stage(true); // hole in stage
