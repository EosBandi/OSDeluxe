
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
    Wire.begin (I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 3000000, I2C_OP_MODE_ISR);
    Wire.setDefaultTimeout (10000); // 10ms


	//Open USB serial port for debug and UI
    Serial.begin (115200);
	//Open Serial1 port for MavLink communication
    Serial1.begin(921600);
	//Open Serial 2 port for input from weight cell
	Serial2.begin(115200);


    //delay(5000);
}

void loop ()
{

    tw_init ();
	//init_scratch_memory();




    tw_ch_settings (1, CH_ON, CH_NO_POPUP);
    tw_ch_settings (2, CH_OFF, CH_POPUP);
    tw_ch_settings (3, CH_OFF, CH_POPUP);
    tw_ch_settings (4, CH_OFF, CH_POPUP);


    tw_ch_set_window (1, 0, 0, 180);
    tw_ch_set_window (2, 4, 0 , 64);
    tw_ch_set_window (3, 116, 0, 64);
    tw_ch_set_window (4, 68, 0, 48);

    tw_set_ch_input(1,INPUT_CH_1);
    tw_set_ch_input(2,INPUT_CH_2);
    tw_set_ch_input(3,INPUT_CH_3);
    tw_set_ch_input(4,INPUT_CH_4);


    tw_write_register(0x0c8,0x03);
	tw_write_register(0x057, 0x00);  // Extra coring for sharepning
	tw_write_register(0x1aa, 0xBB);  // middle bandwith for Y DAC, reduce color crawl

    digitalWrite (LED_PIN, HIGH);

    rec_color_shadow = COLOR_REC_BLACK;
    rec_color = COLOR_REC_WHITE;
    rec_color_background = COLOR_REC_RED | REC_MIX;

    disp_color = COLOR_YELLOW;
    disp_color_background = COLOR_NONE;
    disp_color_shadow = COLOR_BLACK;


    tw_clear_all_pages();

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

////Ez torolni
//tw_osd_set_display_field(OSD_work_field);

init_home();

//Ezt is torolni
//tw_osd_fill_region(0, 0, 179, 287, 0xff, OSD_work_field, OSD_PATH_DISP, 0);
//tw_wait_for_osd_write(20);
//delay(2000);
//font_type = FONT_OUTLINE_16x12;
//disp_color = COLOR_WHITE;
//debug("normal write start - %lu\n", millis());
//tw_printf(20, 40, "IMU0: In flight calibration completed !");
//debug("normal write end - %lu\n", millis());




//tw_display_logo();

get_parameter_count();
param_send_index = total_params;
debug("Parameter count:%u\n", total_params);




// eddig es ne tovabb
//while (1);

unsigned long now;

while (1)
{
	now = millis();


	read_mavlink();


    tw_osd_fill_region (0, 0, 179, 287, 0xff, OSD_work_field, OSD_PATH_DISP, 0);
    tw_wait_for_osd_write(20);

    if (osd.horizon.visible) render_horizon(&osd.horizon);

	if (osd.gps.visible) osd_gps_render( &osd.gps );

	if (osd.bat.visible) {
		osd_battery_prerender(&osd.bat);
		osd_battery_render(&osd.bat);
	}
	
	if (osd.stat.visible) osd_status_render(&osd.stat);

	if (osd.alt.visible) osd_altitude_render(&osd.alt);

	if (osd.vario.visible) osd_vario_render(&osd.vario);

	if (osd.home_w.visible) {
		osd_home_prerender(&osd.home_w);
		osd_home_render(&osd.home_w);
	}

	if (osd.mode.visible) osd_mode_render(&osd.mode);

	if (osd.pull.visible) osd_pull_render(&osd.pull);

    if (osd.msg_widget.visible)  message_buffer_render();

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

 

    //check heartbeat
    heartbeat_validation();

	//
	send_param_list();

	//Send out mavlink heartbeat
	if ((millis() - last_heartbeat_sent) >= 1000)
	{
		heartbeat_out();
		last_heartbeat_sent = millis();
	}



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


	if (Serial.available() != 0)
	{
		char ch = Serial.read();
		if (ch == '1') {
			tw_write_register(0x0f3, 0b11100000);
			debug("ON\n");
		}
		else 
		{
			tw_write_register(0x0f3, 0b11110000);

			debug("OFF\n");
		}
	}

	//debug("Loop time: %lu\n", millis() - now);
	//debug("Bytes waiting: %u\n", Serial1.available());




}
}
