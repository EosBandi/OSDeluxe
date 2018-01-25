
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


    #define SCALE 5
    float pitchrad, rollrad;
  
    struct point ils_points[5] = { { 0, -10 }, { +10, +20 }, { 0, 5 }, { -10, +20 } };
    struct polygon ils;

    ils.len = 4;
    ils.points = ils_points;
    

 #define X_POS 40
 #define Y_POS 100
 #define Y_HEIGHT 15
 #define X_WIDTH  20


b.x = 10;
b.y = 5;
b.w = 30;
b.h = 8;
b.max = 25.2;
b.min = 20.0;
b.val = 22.6;
b.warn = 21.0;
b.mix = true;

g.x = 40;
g.y = 100;
g.sat = 8;
g.sat_warn = 5;
g.hdop = 12.33;
g.color = COLOR_YELLOW;

bw.x = 30;
bw.y = 100;

osd_battery_prerender( &bw );
osd_battery_render( & bw );




    while (1)
        ;
}
