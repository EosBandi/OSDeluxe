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

#ifndef _MAVLINK_H_
#define _MAVLINK_H_

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#include "./mavlink_gen/mavlink_types.h"


extern mavlink_system_t mavlink_system; //Ardupilot:7,1  Pixhawk:100,50
extern mavlink_message_t msg; 
extern mavlink_status_t mv_status;
extern unsigned long last_heartbeat_sent;		//Timestamp of the last heartbeat sent out by the OSD

extern unsigned long mavlink_seen[256];          //Timestamp of the [id] message last seen.

void comm_send_ch(mavlink_channel_t chan, uint8_t ch);

#include "./mavlink_gen/ardupilotmega/mavlink.h"
#include "./mavlink_gen/ardupilotmega/version.h"


#define OSD_SYS_ID 200              //OSD System id for sending messages


extern unsigned int mav_seq;


void read_mavlink();
void request_mavlink_rates(void);
void request_mavlink_battery_capacity(void);
void heartbeat_validation(void);
unsigned long mavdata_age(unsigned int id);
void mavlink_send_msg(mavlink_message_t *msg);

void heartbeat_out(void);





#endif