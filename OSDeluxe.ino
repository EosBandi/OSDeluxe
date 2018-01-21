#include <Arduino.h>
#include <stdlib.h>
#include "i2c_t3.h"

#include "tw_func.h"
#include "graphics.h"

void debug (const char *format, ...)
{
    char buf[128];
    va_list args;
    va_start (args, format);
    vsniprintf (buf, sizeof (buf), format,
                args); // does not overrun sizeof(buf) including null terminator
    va_end (args);
    // the below assumes that the new data will fit into the I/O buffer. If not, Serial may drop it.
    // if Serial had a get free buffer count, we could delay and retry. Such does exist at the
    // device class level, but not at this level.
    Serial.write (buf); // move chars to I/O buffer, freeing up local buf
    return;
}


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

    color = COLOR_WHITE;

    unsigned long t, tl;

    font_type = FONT_SHADOW_16x8;

    rec_shadow_color = COLOR_REC_BLACK;
    rec_color = COLOR_REC_WHITE;
    rec_back_color = COLOR_REC_BLACK | REC_MIX;

    color = COLOR_YELLOW;
    back_color = COLOR_BLACK | MIX;


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

    tw_osd_set_display (0, FLD_EVEN, FLD_ODD);

    // OSD_path = OSD_PATH_REC;
    // OSD_work_field = FLD_ODD;
    // font_type = FONT_16x16;
    // tw_printf(5,250,"Ellapsed Time!");

    OSD_path = OSD_PATH_DISP;
    OSD_work_field = FLD_EVEN;


    #define SCALE 5
    float pitchrad, rollrad;
    tw_set_osd_buf (COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);

    struct point ils_points[5] = { { 0, -10 }, { +10, +20 }, { 0, 5 }, { -10, +20 } };
    struct polygon ils;

    ils.len = 4;
    ils.points = ils_points;
    
    transform_polygon(&ils,50,70,0);
    
    // tw_switch_display_field();
    
            t = millis ();
            tw_set_osd_buf (COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
            draw_polygon (&ils);
            transform_polygon(&ils,1,1,0);
            tw_set_osd_buf (COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
            draw_polygon (&ils);
            

            tl = millis () - t;

            tw_printf (10, 220, "Ellapsed Time : %u  ", tl);

            delay (100);


    while (1)
        ;
}
