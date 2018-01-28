#ifndef _MAVLINK_H_
#define _MAVLINK_H_

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#include "./mavlink_gen/mavlink_types.h"

extern mavlink_system_t mavlink_system; //Ardupilot:7,1  Pixhawk:100,50
extern mavlink_message_t msg; 
extern mavlink_status_t mv_status;

void comm_send_ch(mavlink_channel_t chan, uint8_t ch);

#include "./mavlink_gen/ardupilotmega/mavlink.h"
#include "./mavlink_gen/ardupilotmega/version.h"


#endif