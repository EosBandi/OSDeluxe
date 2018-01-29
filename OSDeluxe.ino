
#include "OSDeluxe.h"


#define TW_RESET_PIN 14
#define LED_PIN 13

uint32_t FreeRam(){ // for Teensy 3.0
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is the free, available ram.
    return stackTop - heapTop;
}

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

    Serial1.begin(57600);
    request_mavlink_rates();
    request_mavlink_rates();
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

// Prerender 


osd_center_marker();


OSD_work_field = FLD_ODD;
OSD_display_field = FLD_EVEN;

memset(&osd.message_buffer, 0, sizeof(osd.message_buffer) );
osd.message_buffer_line = 0;
osd.message_buffer_display_time = 0;


while (1)
{
    tw_osd_fill_region (0, 0, 179, 287, 0xff, OSD_work_field, OSD_PATH_DISP);
    render_horizon(&osd.horizon);


    osd_gps_render( &osd.gps );
    osd_battery_prerender(&osd.bat);
    osd_home_prerender(&osd.home);
    osd_battery_render(&osd.bat);
    osd_status_render(&osd.stat);
    osd_altitude_render(&osd.alt);
    osd_vario_render(&osd.vario);
    osd_home_render(&osd.home);
    osd_mode_render(&osd.mode);
    message_buffer_render();
 

    if (OSD_work_field == FLD_ODD){
        OSD_work_field = FLD_EVEN;
        OSD_display_field = FLD_ODD;
    } else {
        OSD_work_field = FLD_ODD;
        OSD_display_field = FLD_EVEN;
    }
    tw_osd_set_display(0,OSD_display_field,FLD_EVEN);
    read_mavlink();
    //debug("Free mem:%u\n", FreeRam());    
}
}
