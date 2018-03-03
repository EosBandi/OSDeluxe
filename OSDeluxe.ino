
/*
    OSDeluxe - Color PIP Mavlink OSD
    Copyright (C) 2018  Andras Schaffer - Dronedoktor.eu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    With Grateful Acknowledgements to the projects:
        AlceOSD by Luis Alves
        PlayUAV OSD
        Brain FPV Flight Controller(https://github.com/BrainFPV/TauLabs) by Tau Labs

 */

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
    Wire.begin (I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 3000000, I2C_OP_MODE_IMM);
    Wire.setDefaultTimeout (10000); // 10ms


	//Open USB serial port for debug and UI
    Serial.begin (115200);
	//Open Serial1 port for MavLink communication
    Serial1.begin(115200);
	//Open Serial 2 port for input from weight cell
	Serial2.begin(115200);


    //delay(5000);
}

void loop ()
{

    tw_init ();

    
    tw_ch_settings (1, 1, 0);
    tw_ch_settings (2, 1, 1);
    tw_ch_settings (3, 1, 1);
    tw_ch_settings (4, 1, 1);


    tw_ch_set_window (1, 0, 0, 180);
    tw_ch_set_window (2, 4, 0 , 64);
    tw_ch_set_window (3, 116, 0, 64);
    tw_ch_set_window (4, 68, 0, 48);
/*
    tw_write_register(0x081,0xff);
    tw_write_register(0x082,0xff);

    tw_write_register(0x083,0xff);
    tw_write_register(0x084,0xff);

    tw_write_register(0x130,0x00);
    tw_write_register(0x131,0xb4);
    tw_write_register(0x132,0x00);
    tw_write_register(0x133,0x90);
    
*/

    tw_set_ch_input(1,INPUT_CH_1);
    tw_set_ch_input(2,INPUT_CH_2);
    tw_set_ch_input(3,INPUT_CH_3);
    tw_set_ch_input(4,INPUT_CH_4);


    tw_write_register(0x0c8,0x03);
            

    digitalWrite (LED_PIN, HIGH);

    rec_color_shadow = COLOR_REC_BLACK;
    rec_color = COLOR_REC_WHITE;
    rec_color_background = COLOR_REC_RED | REC_MIX;

    disp_color = COLOR_YELLOW;
    disp_color_background = COLOR_NONE;
    disp_color_shadow = COLOR_BLACK;


    tw_clear_all_pages();
    /*
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
*/
    OSD_path = OSD_PATH_DISP;
    OSD_work_field = FLD_EVEN;

    tw_osd_set_display_field(FLD_EVEN);
    tw_osd_set_rec_field(FLD_EVEN);
    tw_osd_set_display_page(0);

    OSD_path = OSD_PATH_DISP;
    OSD_work_field = FLD_EVEN;

    default_settings();
    //save_settings();
    //load_settings();

// Prerender 

osd.displayed_mode = -1;        // Signal startup
osd_center_marker();


OSD_work_field = FLD_ODD;
OSD_display_field = FLD_EVEN;

memset(&osd.message_buffer, 0, sizeof(osd.message_buffer) );
osd.message_buffer_line = 0;
osd.message_buffer_display_time = 0;

init_home();

long t, l;

while (1)
{

    tw_osd_fill_region (0, 0, 179, 287, 0xff, OSD_work_field, OSD_PATH_DISP, 0);
    tw_wait_for_osd_write(20);


    if (osd.horizon.visible) render_horizon(&osd.horizon);


    osd_gps_render( &osd.gps );
    osd_battery_prerender(&osd.bat);
    osd_home_prerender(&osd.home_w);
    osd_battery_render(&osd.bat);
    osd_status_render(&osd.stat);
    osd_altitude_render(&osd.alt);
    osd_vario_render(&osd.vario);
    osd_home_render(&osd.home_w);
    osd_mode_render(&osd.mode);

	osd_pull_render(&osd.pull);

    message_buffer_render();

    //tw_printf(10,50,"ch1:%u, ch2:%u, ch3:%u. ch4:%u", osd.ctr_state[0],osd.ctr_state[1],osd.ctr_state[2],osd.ctr_state[3]);

    if (osd.ctr_saved_state[0] != osd.ctr_state[0])
    {
        // There is a change in ctr1 state
        osd.ctr_saved_state[0] = osd.ctr_state[0]; // Save it, to prevent unneccessary state changes in the main loop
        switch (osd.ctr_state[0])
        {

		//Default view
		// 1-front 2-right 3-left 4-Back 
        case 0:
            tw_set_ch_input(1, INPUT_CH_1);
            tw_set_ch_input(2, INPUT_CH_2);
            tw_set_ch_input(3, INPUT_CH_3);
            tw_set_ch_input(4, INPUT_CH_4);
            break;
		// 1-back,2-right, 3-left, 4-front
        case 1:
            tw_set_ch_input(1, INPUT_CH_4);
            tw_set_ch_input(2, INPUT_CH_2);
            tw_set_ch_input(3, INPUT_CH_3);
            tw_set_ch_input(4, INPUT_CH_1);
            break;
        case 2:
            tw_set_ch_input(1, INPUT_CH_4);
            tw_set_ch_input(2, INPUT_CH_2);
            tw_set_ch_input(3, INPUT_CH_3);
            tw_set_ch_input(4, INPUT_CH_1);
            break;
        }
    }

    if (osd.ctr_saved_state[1] != osd.ctr_state[1])
    {
        // There is a change in ctr1 state
        osd.ctr_saved_state[1] = osd.ctr_state[1]; // Save it, to prevent unneccessary state changes in the main loop
        switch (osd.ctr_state[1])
        {
        case 0:
			//All four windows are visible
            tw_ch_settings(1, 1, 0);
            tw_ch_settings(2, 1, 1);
            tw_ch_settings(3, 1, 1);
            tw_ch_settings(4, 1, 1);
            break;
        case 2:
			//only first screen
            tw_ch_settings(1, 1, 0);
            tw_ch_settings(2, 0, 1);
            tw_ch_settings(3, 0, 1);
            tw_ch_settings(4, 0, 1);
            break;
        case 1:
			//Disable fourth smalles screen
            tw_ch_settings(1, 1, 0);
            tw_ch_settings(2, 1, 1);
            tw_ch_settings(3, 1, 1);
            tw_ch_settings(4, 0, 1);
            break;
        }
    }

    if (osd.ctr_saved_state[2] != osd.ctr_state[2])
    {
        // There is a change in ctr1 state
        osd.ctr_saved_state[2] = osd.ctr_state[2]; // Save it, to prevent unneccessary state changes in the main loop
        switch (osd.ctr_state[2])
        {
        case 0:
            tw_osd_set_display_page(0);
            tw_osd_set_rec_field(FLD_EVEN);
			osd.horizon.visible = true;
            break;
        case 1:
			tw_osd_set_display_page(0);
			tw_osd_set_rec_field(FLD_EVEN);
			osd.horizon.visible = false;
			break;
		case 2:
			tw_osd_set_display_page(1);
			tw_osd_set_rec_field(FLD_ODD);
			osd.horizon.visible = false;
			break;
		}
    }

    //Switch OSD_work_field for smooth redraw
    if (OSD_work_field == FLD_ODD){
        OSD_work_field = FLD_EVEN;
        OSD_display_field = FLD_ODD;
    } else {
        OSD_work_field = FLD_ODD;
        OSD_display_field = FLD_EVEN;
    }

    tw_osd_set_display_field(OSD_display_field);

    read_mavlink();

    //check heartbeat
    heartbeat_validation();
    if (millis() > (osd.home.last_calc+HOME_CALC_INTERVAL)) calc_home();
    if ( (osd.bat.max_capacity == 0) && (millis() > (osd.last_capacity_query+5000)) ) request_mavlink_battery_capacity();

	if (Serial2.available() == 0) 
	{
		Serial2.write("X"); // Any character will generate reading
	}
	else
	{
		int b;
		char read_buffer[10];

		if (Serial2.available() > 16) {
			Serial2.clear();
		}
		else {
			b = 0;
			while (Serial2.available())
			{
				read_buffer[b++] = Serial2.read();
			}
			read_buffer[b-1] = 0; //Null terminator
			osd.pull.pull = atof(read_buffer) *9.8f;  //Convert to newtons
			if (osd.pull.pull < 0) osd.pull.pull = 0;

		}
	}





}
}
