$fn=180;
tolerance = 0.1;
wall = 1.2;
pcb_margin = 0.5;
pcb_x = 69.5;
pcb_y = 72.1;
pcb_z = 1.6;
strut_bolt_h = 6;
bottom_strut_h = 6;
// the height of the body part of the strut
top_strut_h = 10;
bolt_d = 3.2;
// bolt head
bolt_head_h = 2.3;
bolt_head_d = 5.5;
// from the tip of the strut at bottom to the pcb top surface
pcb_h = strut_bolt_h+bottom_strut_h+pcb_z;
// the height of the upper part, pcb top surface to case top surface
upper_case_h = 14 + wall;//top_strut_h + wall + bolt_head_h;
//buttons = [[5.5, 9.7], [31.9, 9.7], [62.6, 18]];
buttons = [[62.6, 18]];
button_d = 6.5;
button_h = 4.5;
button_dz = 1.35;
//leds = [[63.36, 28.04], [63.36, 26.04], [21.84, 48.88], [24.38, 48.88], [26.29, 48.88]];
leds = [[63.36, 28.04], [63.36, 26.04], [21.84, 46.3], [24.38, 46.3], [26.29, 46.3]];
hole_support_thickness = 0.3;
nrf_extend = 6;

module usb() {
    color("#333")
    translate([16, 0, 0])
    cube([28, 16, 1.8]);
    color("silver")
    translate([0, 2, 0.5])
    cube([16, 12, 4.5]);
}

module nrf() {
    color("#333")
    cube([15.3, 29.1, 1.2]);
}

module lcd() {
    color("#DDD")
    cube([36, 31, 1.8]);
    
    color("#AAA")
    translate([1.2, 1.2, 1])
    cube([33.5, 25.5, 1.7]);
}

module pcb() {
    import("pcb.stl");
    translate([-14, 50, 8.5])
    mirror([0, 0, 1])
    usb();
    translate([53.2, 36.3, 7])
    nrf();
    translate([0, 13, pcb_z+2])
    lcd();
    // struts
    for (x = [3.5, 66], y = [3.53, 68.534]) {
        translate([x, y, -12])
        bottom_strut();
        translate([x, y, pcb_z])
        top_strut();
    }
}

module bottom_strut() {
    // nut
    nut_h = 2.5 + 0.5;
    color("#AAA")
    translate([0, 0, -tolerance])
    cylinder(d=6.3+tolerance*2, h = nut_h+tolerance, $fn=6);
    // bolt
    translate([0, 0, nut_h + hole_support_thickness])
    color("#222")
    cylinder(d=bolt_d+tolerance*2, h = bottom_strut_h+strut_bolt_h - nut_h);
    // body
    color("#222")
    translate([0, 0, strut_bolt_h])
    cylinder(d = 6.3 + tolerance*2, h = bottom_strut_h, $fn=6);
}

module top_strut() {
    // bolt
    color("#222")
    translate([0, 0, -6])
    cylinder(d = bolt_d+tolerance*2, h = top_strut_h+strut_bolt_h + wall - hole_support_thickness);
    // body
    color("#222")
    cylinder(d = 6.3 + tolerance*2, h = top_strut_h);
    // bolt head
    color("#222")
    translate([0, 0, top_strut_h + wall])
    cylinder(d = bolt_head_d + tolerance*2, h = bolt_head_h + tolerance + 100);
}

module button() {
    button_exposed = 1;
    intersection() {
        union() {
            cylinder(d = button_d - tolerance*4, h = upper_case_h - button_h);
            cube([button_d + wall*2 - tolerance*4, 2 - tolerance*4, (wall + button_dz*2 - tolerance*4)*2], center=true);
            translate([0, 0, upper_case_h - button_h])
            resize([button_d - tolerance*4, button_d - tolerance*4, button_exposed*2])
            sphere(d = button_d - tolerance*2);
        }
        cylinder(d = button_d + wall*2, h = upper_case_h - button_h + button_exposed);
    }
}

module screw() {
    cylinder(d=4, h=15);
    cylinder(d1 = 8, d2 = 4, h = 2.4);
}

module mount() {
    difference() {
        hull() {
            cylinder(d = 15, h = 3);
            translate([0, 15, 0])
            cylinder(d = 15, h = 3);
        }
        translate([0, 15, 3 + tolerance])
        rotate(180, [1, 0, 0])
        screw();
    }
}

module lower_half() {
    color("#666")
    difference() {
        union() {
            difference() {
                union() {
                    // body
                    cube([pcb_x + pcb_margin*2 + wall*2, pcb_y + pcb_margin*2 + wall*2 + nrf_extend, pcb_h - wall/2]);
                    // edge
                    translate([wall/2+tolerance/2, wall/2+tolerance/2, 0])
                    cube([pcb_x + pcb_margin*2 + wall*2-wall-tolerance, pcb_y + pcb_margin*2 + wall*2  + nrf_extend -wall-tolerance, pcb_h]);
                    // mounts
                    translate([(pcb_x + pcb_margin*2 + wall*2)/2, pcb_y + pcb_margin*2 + wall*2 + nrf_extend - 9, 0])
                    mount();
                    translate([(pcb_x + pcb_margin*2 + wall*2)/2, 9, 0])
                    rotate(180, [0, 0, 1])
                    mount();
                }
                
                translate([wall, wall, wall])
                cube([pcb_x + pcb_margin*2, pcb_y + pcb_margin*2 + nrf_extend, pcb_h]);
            }
            for (x = [3.5, 66], y = [3.53, 68.534]) {
                translate([x+wall+pcb_margin, y+wall+pcb_margin, 0]) {
                    cylinder(d = 9, h = 4);
                    translate([0, 0, 4])
                    cylinder(d1=9, d2=6, h=2);
                }
            }
        }
        translate([wall+pcb_margin, wall+pcb_margin, 12])
        pcb();
    }
}

module upper_half_normal() {
    pcb_offset = wall+pcb_margin;
    color("#668")
    difference() {
        union() {
            difference() {
                // body
                translate([0, 0, -wall/2])
                cube([pcb_x + pcb_margin*2 + wall*2, pcb_y + pcb_margin*2 + wall*2 + nrf_extend, upper_case_h + wall/2]);
                // main cavity
                translate([wall, wall, -tolerance])
                cube([pcb_x + pcb_margin*2, pcb_y + pcb_margin*2 + nrf_extend, upper_case_h - wall + tolerance]);
                // little edge
                translate([wall/2-tolerance/2, wall/2-tolerance/2, -wall/2 - tolerance])
                cube([pcb_x + pcb_margin*2 + wall*2 - wall+tolerance, pcb_y + pcb_margin*2 + wall*2 + nrf_extend - wall + tolerance, wall/2 + tolerance]);
            }
            // for bolts
            for (x = [3.5, 66], y = [3.53, 68.534]) {
                translate([x+wall+pcb_margin, y+wall+pcb_margin, top_strut_h]) {
                    cylinder(d = 9, h = upper_case_h - top_strut_h);
                }
            }
            // lcb support
            lcd_top = 2 + 2.7 + tolerance;
            translate([0+pcb_margin+wall, 13+pcb_margin+wall, lcd_top])
            *cube([33.5+1.2*2, 25.5+1.2*2, upper_case_h-lcd_top]);
            // buttons casings
            for (c = buttons) {
                translate([pcb_offset + c[0], pcb_offset + c[1], button_h + tolerance])
                cylinder(d = button_d + wall*2, h = upper_case_h - button_h - tolerance);
            }
            // for lights
            led_h = 1;
            for (c = leds) {
                translate([pcb_offset + c[0], pcb_offset + c[1], led_h])
                cylinder(d = 1.75 + wall, h = upper_case_h - led_h);
            }

        }
        translate([wall+pcb_margin, wall+pcb_margin, -pcb_z])
        pcb();
        // cuts
        cut_h = upper_case_h + tolerance*2;
        // for usb
        translate([-tolerance, 46, -wall])
        cube([tolerance*2 + wall, 21, 8 + wall]);
        // for lcd
        lcd_padding = 1.5/2;
        translate([0+1.2+pcb_offset+lcd_padding, 13+1.2+pcb_offset + lcd_padding, 0])
        *cube([33.5 - lcd_padding*2, 25.5 - lcd_padding*2, cut_h]);
        // for buttons
        for (c = buttons) {
            translate([pcb_offset + c[0], pcb_offset + c[1], button_h - tolerance]) {
                // holes without tolerance to drill the layers for smoothness
                cylinder(d = button_d, h = upper_case_h - button_h + tolerance*2);
                cube([button_d + wall*2 + tolerance*2, 2 + tolerance*2, (wall + button_dz*2 + tolerance*2)*2], center=true);
            }
        }
        // for rotator
        translate([pcb_offset + 49.94, pcb_offset + 9.084, 0])
        *cylinder(d = 7 + tolerance*2, h = cut_h);
        // for lights
        for (c = leds) {
            translate([pcb_offset + c[0], pcb_offset + c[1], -tolerance])
            cylinder(d = 1.75 + tolerance*2, h = cut_h);
        }
        // cut in lights housings for usb
        translate([pcb_offset + 19, pcb_offset + leds[2][1] -wall*2, 0])
        cube([10, 5, upper_case_h - wall]);
        // cut for crystal on nrf
        translate([pcb_offset + 63, pcb_offset + 41, upper_case_h - wall - tolerance])
        *cube([4.5, 15, wall/2]);
        // text
        translate([12, 68, upper_case_h - 0.4])
        rotate(-90, [0, 0, 1])
        linear_extrude(wall)
        text("Kagami house black", 4);
        translate([25, 50, upper_case_h - 0.4])
        linear_extrude(wall)
        text("R T", 2);
        // hole for the antenna
        translate([58, wall+pcb_offset + 74.6, 10.3])
        rotate(90, [-1, 0, 0])
        cylinder(d = 6.3 + tolerance*2, h = wall*4);
    }
}

module assembly() {
    lower_half();
    translate([0, 0, pcb_h]) 
    upper_half_normal();
    translate([wall+pcb_margin, wall+pcb_margin, 12])
    *pcb();
}


// bolt cut inspection
*intersection() {
    assembly();
    translate([-tolerance, -tolerance, -tolerance])
    cube([pcb_x + wall*2+pcb_margin*2 + tolerance*2, 3.53+wall+pcb_margin + tolerance, pcb_h + upper_case_h + tolerance*2]);
}

*assembly();

lower_half();
rotate(180, [1, 0, 0])
translate([80, -81, -upper_case_h])
upper_half_normal();
*button();
