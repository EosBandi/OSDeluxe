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
#define MESSAGE_BUFFER_LINES 20

#define RC_STATE0 1300
#define RC_STATE1 1700



struct osd_settings {

    channel_setting video_channels[CHANNEL_MODES][5]; // ch 1-ch4 (no zero) ???
    gps_widget_t gps;
    battery_widget_t bat;
    status_widget_t stat;
    alt_widget_t alt;
    vario_widget_t vario;
    home_widget_t home_w;
    horizon_t horizon;
    mode_widget_t mode;

	pull_widget_t pull;
    


    //Global variables
    unsigned char mav_type;             //MAV type from mavlink heartbeat;
    unsigned char base_mode;            //It comes from the arming...
    int           heading;
    int           airspeed;
    int           groundspeed;
    int           throttle;
    bool          arming_status;
    home_data_t     home;

    int           ekfvel;
    int           ekfposh; 
    int           ekfposv;
    int           ekfcompass;
    int           ekfterrain;


    unsigned char ctr_ch[4];
    unsigned char ctr_state[4];
    unsigned char ctr_saved_state[4];

    unsigned int           rcin[17];
    unsigned int           rc_rssi;

    char         system_status;   //Se MAVLINK_STATE enum 



    char          message_buffer[MESSAGE_BUFFER_LINES][52];               //20 line of 52 character message_buffer
    char          message_severity[MESSAGE_BUFFER_LINES];
    char          message_buffer_line;
    char          message_buffer_display_line;
    long          message_buffer_display_time;
    bool          message_display; //we have message to display in the buffer
    bool          clear_req;

    bool          displayed_arming_status;
    unsigned long armed_start_time;

    int displayed_mode;

    unsigned long last_capacity_query;
    unsigned long last_outgoing_heartbeat;
    //unsigned long 
   
};


void default_settings();
void load_settings();
void save_settings();
void message_buffer_add_line(char *message, char severity);
void message_buffer_render(void);

#endif