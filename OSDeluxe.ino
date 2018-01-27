
#include "OSDeluxe.h"


void setup ()
{

    pinMode (13, OUTPUT);
    pinMode (14, OUTPUT);

    delay (1000);
    digitalWrite (14, LOW);
    delay (500);
    digitalWrite (14, HIGH);
    delay (500);

    // Setup for Master mode, pins 18/19, external pullups, 400kHz, 10ms default timeout
    Wire.begin (I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 3000000, I2C_OP_MODE_DMA);
    Wire.setDefaultTimeout (10000); // 10ms

    Serial.begin (115200);
}

void loop ()
{

    char i;
    tw_init ();

    tw_ch_set_window (1, 0, 0, 180);
    tw_ch_set_window (2, 0, 0, 180);
    tw_ch_set_window (3, 0, 0, 180);
    tw_ch_set_window (4, 0, 0, 180);

    tw_ch_settings (1, 1, 1);
    tw_ch_settings (2, 1, 1);
    tw_ch_settings (3, 1, 0);
    tw_ch_settings (4, 0, 0);


    tw_ch_set_window (1, 115, 95, 60);
    tw_ch_set_window (2, 0, 95 , 60);
    tw_ch_set_window (3, 0, 0, 180);
    tw_ch_set_window (4, 90, 72, 90);

    // Enable overlays dual mode high priority
    tw_write_register (0x238, 0b00000101);

    //Brightness ch1
    //tw_write_register( 0x00a, 64);
    //Delay mode ?
    tw_write_register(0xc5,0xff);
    //tw_write_register(0x1ab,0b00001010);
    
    //COLOR LPF !!!550Khz
    tw_write_register(0x046,0b00001111);

    tw_write_register(0x02b,0b00010001);


    digitalWrite (13, HIGH);


    unsigned long t, tl;

    
    rec_color_shadow = COLOR_REC_BLACK;
    rec_color = COLOR_REC_WHITE;
    rec_color_background = COLOR_REC_RED | REC_MIX;

    disp_color = COLOR_YELLOW;
    disp_color_background = COLOR_NONE;
    disp_color_shadow = COLOR_BLACK;

    // Clear OSD on display path both fields
    tw_osd_fill_region (0, 0, 179, 287, 0xff, FLD_EVEN, OSD_PATH_DISP);
    tw_wait_for_osd_write (100);
    tw_osd_fill_region (0, 0, 179, 287, 0xff, FLD_ODD, OSD_PATH_DISP);
    tw_wait_for_osd_write (100);
    // Clear OSD on record path both fields
    tw_osd_fill_region (0, 0, 89, 287, 0xff, FLD_EVEN, OSD_PATH_REC);
    tw_wait_for_osd_write (100);
    tw_osd_fill_region (0, 0, 89, 287, 0xff, FLD_ODD, OSD_PATH_REC);
    tw_wait_for_osd_write (100);

    OSD_path = OSD_PATH_DISP;
    OSD_work_field = FLD_EVEN;

    tw_osd_set_display (0, FLD_EVEN, FLD_EVEN);

    // OSD_path = OSD_PATH_REC;
    // OSD_work_field = FLD_ODD;
    // font_type = FONT_16x16;
    // tw_printf(5,250,"Ellapsed Time!");

    OSD_path = OSD_PATH_DISP;
    OSD_work_field = FLD_EVEN;





bw.x = 143;
bw.y = 10;
bw.min_cell_voltage = 3.3;
bw.max_cell_voltage = 4.2;
bw.red_cell_voltage    = 3.5;
bw.yellow_cell_voltage = 3.7;
bw.bar_type = BAR_MULTICOLOR;
bw.mix = 0;
bw.cells = 6;
bw.voltage = 23.4;
bw.current = 0.0;
bw.max_capacity = 12000;
bw.remaining_capacity = 7262;

g.x = 7;
g.y = 10;
g.sat = 10;
g.hdop = 0.7;
g.sat_warn = 5;
g.color = COLOR_YELLOW;

status.x = 7;
status.y = 31;
status.mix = 0;
status.gps_status = STATUS_OK;
status.ekf_status = STATUS_WARNING;
status.vibe_status = STATUS_CRITICAL;


aw.x = 7;
aw.y = 48;
aw.mix = 0;
aw.altitude = 45;

vw.x = 165;
vw.y = 80;
vw.h = 100;
vw.w = 4;
vw.vario = 5.5f;
vw.vario_max = 15.0f;
vw.mix = 0;
vw.num_pos = POS_BELOW;

hw.x = 90;
hw.y = 245;
hw.orientation = 45;
hw.home_distance = 100;


osd_gps_render( &g );
osd_battery_prerender( &bw );
osd_battery_render( &bw );
osd_status_render( &status);
osd_altitude_render( &aw);
osd_vario_render(&vw);

osd_home_prerender( &hw);
osd_home_render(&hw);
osd_center_marker();
    while (1)
        ;
}
