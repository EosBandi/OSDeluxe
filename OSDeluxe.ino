
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
    Wire.begin (I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 4000000, I2C_OP_MODE_IMM);
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

	digitalWrite(LED_PIN, HIGH);


    tw_init ();

	osd.pip_page = 0;

	tw_write_register(0x0c8, 0x03);
	tw_write_register(0x057, 0x00);  // Extra coring for sharepning
	//tw_write_register(0x1aa, 0x66);  // middle bandwith for Y DAC, reduce color crawl


	//We will use these screens 
	//Don't clear all since pages larger than 0 share memory with scratch table
	OSD256_clear_screen(PTH_X, 0);
	OSD256_clear_screen(PTH_X, 1);
	OSD256_clear_screen(PTH_Y, 0);

	
	OSD256_set_display_page(0);

    default_settings();
    //save_settings();
    //load_settings();

	update_pip();


	osd.displayed_mode = -1;        // Signal startup
	
									
	//osd_center_marker();


//	OSD_work_field = FLD_ODD;
//	OSD_display_field = FLD_EVEN;

//	tw_osd_set_display_field(OSD_display_field);


	memset(&osd.message_buffer, 0, sizeof(osd.message_buffer) );
	osd.message_buffer_line = 0;
	osd.message_buffer_display_time = 0;


	//This is for selecting different sets
	osd.visible_osd_page = 0x01; //bit coded 
	osd.pip_page = 0x00;

	init_home();

	get_parameter_count();
	param_send_index = total_params;

	//Display both fields !!!!
	tw_write_register(0x20f, 0x0c);

	OSD256_wr_page = 0;


	//Commented out for quick start, need to run at every powerup
	//init_font_tables();
	init_bitmaps();

	OSD256_clear_screen(PTH_X,0);
	OSD256_clear_screen(PTH_X,1);

	//From now on we assume that extended OSD functions are enabled...
	tw_write_register(0x240, 0x01);

	unsigned long now;


	OSD256_wr_page = 1;
	//OSD256_wr_page = 0;

	OSD256_set_display_page(0);
	//OSD256_Block_Transfer(SCRATCH, DISPLAY, 0, 0, 0, 0, 719, 575);

	/*
	for (int i = -50; i < 20; i++) {
		OSD256_clear_screen(PTH_X, OSD256_wr_page);
		osd.vx = i;
		osd.vy = 0;
		movement_render(&osd.move);
		OSD256_set_display_page(OSD256_wr_page);
		if (OSD256_wr_page == 0) OSD256_wr_page = 1;
		else OSD256_wr_page = 0;
	}
	while (1);


	*/

//Main loop
while (1)
{
	read_mavlink();

	OSD256_clear_screen(PTH_X, OSD256_wr_page);

	now = millis();
	
	if (osd.center_cross_visible & osd.visible_osd_page) osd_center_marker();

	if (osd.move.visible & osd.visible_osd_page) movement_render(&osd.move);

    if (osd.horizon.visible & osd.visible_osd_page) render_horizon(&osd.horizon);

	if (osd.gps.visible & osd.visible_osd_page) osd_gps_render( &osd.gps );

	if (osd.stat.visible & osd.visible_osd_page) osd_status_render(&osd.stat);

	if (osd.batt1_v.visible & osd.visible_osd_page)  osd_batt_volt_render(&osd.batt1_v);

	if (osd.batt2_v.visible & osd.visible_osd_page)  osd_batt_volt_render(&osd.batt2_v);

	if (osd.batt1_cap.visible  & osd.visible_osd_page) osd_batt_cap_render(&osd.batt1_cap);

	if (osd.batt2_cap.visible  & osd.visible_osd_page) osd_batt_cap_render(&osd.batt2_cap);

	if (osd.batt1_curr.visible  & osd.visible_osd_page) osd_batt_curr_render(&osd.batt1_curr);

	if (osd.batt2_curr.visible  & osd.visible_osd_page) osd_batt_curr_render(&osd.batt2_curr);
	
	if (osd.alt.visible & osd.visible_osd_page) osd_altitude_render(&osd.alt);

	if (osd.vario.visible & osd.visible_osd_page) osd_vario_render(&osd.vario);

	if (osd.home_w.visible & osd.visible_osd_page) osd_home_render(&osd.home_w);

	if (osd.mode.visible & osd.visible_osd_page) osd_mode_render(&osd.mode);

	if (osd.pull.visible & osd.visible_osd_page) osd_pull_render(&osd.pull);

    if (osd.msg_widget.visible & osd.visible_osd_page)  message_buffer_render();

	if (osd.gs.visible & osd.visible_osd_page) osd_groundspeed_render(&osd.gs);

	if (osd.thr.visible & osd.visible_osd_page) osd_throttle_render(&osd.thr);

	//Switch working page for smooth redraw
	OSD256_set_display_page(OSD256_wr_page);
	if (OSD256_wr_page == 0) OSD256_wr_page = 1;
	else OSD256_wr_page = 0;


	//Control channel 1 - Control PIP mode
    if (osd.ctr_saved_state[0] != osd.ctr_state[0])
    {
        // There is a change in ctr1 state
        osd.ctr_saved_state[0] = osd.ctr_state[0]; // Save it, to prevent unneccessary state changes in the main loop
    	osd.pip_page = osd.ctr_state[0];

		for (unsigned char i = 1; i < 5; i++)
		{
			tw_ch_set_input(i, osd.video_channels[osd.pip_page][i].input);

			tw_ch_settings(i, osd.video_channels[osd.pip_page][i].enable,
				osd.video_channels[osd.pip_page][i].popup);

			tw_ch_set_window(i, osd.video_channels[osd.pip_page][i].pos_h,
				osd.video_channels[osd.pip_page][i].pos_v,
				osd.video_channels[osd.pip_page][i].len_h);
		}
		
    }

    //Control channel 2 - OSD widgets page

    if (osd.ctr_saved_state[1] != osd.ctr_state[1])
    {
        // There is a change in ctr1 state
        osd.ctr_saved_state[1] = osd.ctr_state[1]; // Save it, to prevent unneccessary state changes in the main loop

		//visible_osd_page is a bit coded value bit represents the one page (1 - page 1, 2-page 2, 4-page 3, 8-page 4.... up to page 5)
		osd.visible_osd_page = 0x01 << osd.ctr_state[1];

    }

    //check heartbeat
    heartbeat_validation();

	//Send five params at once, to speed up parameter download time
	if (param_send_index != total_params)
	{
		for (int i = 0; i < 8; i++)send_param_list();
	}
	
	//Send out mavlink heartbeat
	if ((millis() - last_heartbeat_sent) >= 1000)
	{
		heartbeat_out();
		last_heartbeat_sent = millis();
	}



    if (millis() > (osd.home.last_calc+HOME_CALC_INTERVAL)) calc_home();
    if ( (osd.batt1_cap.max_capacity == 0) && (millis() > (osd.last_capacity_query+5000)) ) request_mavlink_battery_capacity();

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

	//0x111 enhance mode.... keep looking
    //0x57 coring for shapening
	//a7 0d instead of 0x0c


	//0x1a2 50 vagy 90 Y path megjelenitese :D


	if (Serial.available() != 0)
	{
		char ch = Serial.read();
		if (ch == '1') {
			tw_write_register(0x1a8, 0x25);
			debug("ON\n");
		}
		else 
		{
			tw_write_register(0x1a8,0x20);

			debug("OFF\n");
		}
	}

	debug("Looptime : %lu\n", millis() - now);
	//debug("Loop time: %lu\n", millis() - now);
	//debug("Bytes waiting: %u\n", Serial1.available());




}
}
