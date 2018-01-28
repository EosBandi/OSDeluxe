
#include "OSDeluxe.h"


#define TW_RESET_PIN 14
#define LED_PIN 13

void setup ()
{

    pinMode (LED_PIN, OUTPUT);
    pinMode (TW_RESET_PIN, OUTPUT);

    delay (1000);
    digitalWrite (TW_RESET_PIN, LOW);
    delay (500);
    digitalWrite (TW_RESET_PIN, HIGH);
    delay (500);

    // Setup for Master mode, pins 18/19, external pullups, 400kHz, 10ms default timeout
    Wire.begin (I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 3000000, I2C_OP_MODE_DMA);
    Wire.setDefaultTimeout (10000); // 10ms

    Serial.begin (115200);
}

void loop ()
{

    tw_init ();

    tw_ch_set_window (1, 0, 0, 180);
    tw_ch_set_window (2, 0, 0, 180);
    tw_ch_set_window (3, 0, 0, 180);
    tw_ch_set_window (4, 0, 0, 180);

    tw_ch_settings (1, 1, 0);
    tw_ch_settings (2, 1, 1);
    tw_ch_settings (3, 1, 1);
    tw_ch_settings (4, 0, 0);


    tw_ch_set_window (1, 0, 0, 180);
    tw_ch_set_window (2, 4, 0 , 64);
    tw_ch_set_window (3, 116, 0, 64);
    tw_ch_set_window (4, 90, 72, 90);


    tw_set_ch_input(2,INPUT_CH_1);
    tw_set_ch_input(3,INPUT_CH_1);


    digitalWrite (LED_PIN, HIGH);

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

    OSD_path = OSD_PATH_DISP;
    OSD_work_field = FLD_EVEN;

    default_settings();
    //save_settings();
    //load_settings();



osd_gps_render( &osd.gps );
osd_battery_prerender( &osd.bat );
osd_battery_render( &osd.bat );
osd_status_render( &osd.stat );
osd_altitude_render( &osd.alt);
osd_vario_render(&osd.vario);

osd_home_prerender( &osd.home);
osd_home_render(&osd.home);
osd_center_marker();
    while (1)
        ;
}
