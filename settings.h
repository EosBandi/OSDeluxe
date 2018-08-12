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

#ifndef _SETTINGS_H_
#define _SETTINGS_H_


#define CHANNEL_MODES 6


#define RC_STATE0 0
#define RC_STATE1 1231
#define RC_STATE2 1361
#define RC_STATE3 1491
#define RC_STATE4 1621
#define RC_STATE5 1750



struct osd_settings {

	U8   eeprom_version;								// version for eeporm content version.
	U16  settings_size;


	vin_params_t vin_params[4];					// Video in

//	U8 ctr2_video_on[3];				// Enabled video inputs in crt2 channel 

	bool color_bar_x = false;
	bool color_bar_y = false;

	bool color_kill_x = false;
	bool color_kill_y = false;

	char vout1_gain = 7;  //YX
	char vout2_gain = 7;  //CX
	char vout3_gain = 7;  //YY

    channel_setting_t video_channels[CHANNEL_MODES][5]; // ch 1-ch4 (no zero) ???

    gps_widget_t gps;

	batt_volt_widget_t batt1_v;
	batt_volt_widget_t batt2_v;

	batt_cap_widget_t batt1_cap;

	batt_curr_widget_t batt1_curr;
	batt_curr_widget_t batt2_curr;

	batt_power_widget_t batt1_power;
	batt_power_widget_t batt2_power;

	compass_widget_t compass;

    status_widget_t stat;
    alt_widget_t alt;
	gs_widget_t gs;
	throttle_widget_t thr;

    vario_widget_t vario;
	vario_graph_widget_t vgraph;

    home_widget_t home_w;
    horizon_t horizon;
    mode_widget_t mode;

	pull_widget_t pull;

	message_widget_t msg_widget;
	message_list_widget_t msg_list_widget;

	move_widget_t move;

	box_t boxes[8];

	radar_widget_t radar1;
	radar_widget_t radar2;
	radar_widget_t radar3;

	rssi_widget_t rssi;

	U8 center_cross_visible = 0x01;

	U8 ctr_ch[4];



   
};


void default_settings();
U8   load_settings();
void save_settings();


#endif