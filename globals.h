
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

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define EEPROM_VERSION 0x02



#define MESSAGE_BUFFER_LINES 20



struct globals_variables_t {

	//Global variables
	unsigned char mav_type;             //MAV type from mavlink heartbeat;
	unsigned char base_mode;            //It comes from the arming...
	int           heading;
	float         airspeed;
	float         groundspeed;
	int           throttle;
	bool          arming_status;
	home_data_t   home;

	unsigned int  mode;

	float         altitude;
	float         vario;

	float		  variot;

	short         vario_graph[80];
	unsigned char vgraph_idx;


	int           ekfvel;
	int           ekfposh;
	int           ekfposv;
	int           ekfcompass;
	int           ekfterrain;

	float		  vx;					// X speed in m/s
	float		  vy;					// Y speed in m/s
	float		  yaw;					// Yaw in rad
	float		  cos_yaw, sin_yaw;		// Precalculated yaw sin/cos values


	int           b1_power;				//Battery 1 Power (Watts)
	float         b1_current;           
	float         b1_voltage;
	unsigned char b1_remaining_capacity;


	int           b2_power;				//Battery 1 Power (Watts)
	float         b2_current;
	float         b2_voltage;


	unsigned char ctr_state[4];
	unsigned char ctr_saved_state[4];

	unsigned char visible_osd_page;			//The OSD page to show. (bit coded)
	unsigned char pip_page;

	unsigned int  rcin[17];
	unsigned int  rc_rssi;

	char		   system_status;   //Se MAVLINK_STATE enum 

	char           message_buffer[MESSAGE_BUFFER_LINES][52];               //20 line of 52 character message_buffer
	U8           message_severity[MESSAGE_BUFFER_LINES];
	U8           message_buffer_line;
	U8			 message_buffer_display_line;
	long           message_buffer_display_time;
	bool           message_display; //we have message to display in the buffer
	bool           clear_req;

	char		  message_archive[MESSAGE_BUFFER_LINES][52];
	char		  message_archive_severity[MESSAGE_BUFFER_LINES];
	U8			  message_archive_line = 0;


	bool          displayed_arming_status;
	unsigned long armed_start_time;

	int displayed_mode;

	unsigned long last_capacity_query;
	unsigned long last_outgoing_heartbeat;

	char test_byte;

	U8 write_settings;
	U8 debug_looptime;

	bool pthy_redraw;					//indicated that we need to redraw osd contect on y path.


};


extern globals_variables_t g;


#endif