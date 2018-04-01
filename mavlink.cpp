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

#include "osdeluxe.h"

mavlink_system_t mavlink_system = { OSD_SYS_ID, 111 }; // Ardupilot:7,1  Pixhawk:100,50
mavlink_message_t msg;
mavlink_status_t mv_status;

unsigned long mavlink_seen[256]; // Timestamp of the [id] message last seen.
unsigned long last_heartbeat_sent;		//Timestamp of the last heartbeat sent out by the OSD

void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
    switch (chan)
    {
    case MAVLINK_COMM_0:
        Serial1.write(ch); //  mavlink_usart_send_byte(ch);
        break;
    default:
        break;
    }
}

const static unsigned char mavlink_stream_map[] = {
    MAV_DATA_STREAM_ALL,
    /* SCALED_IMU2, SCALED_PRESSURE, SENSOR_OFFSETS */
    MAV_DATA_STREAM_RAW_SENSORS,
    /* MEMINFO, MISSION_CURRENT, GPS_RAW_INT, NAV_CONTROLLER_OUTPUT, LIMITS_STATUS */
    MAV_DATA_STREAM_EXTENDED_STATUS,
    /* SERVO_OUTPUT_RAW, RC_CHANNELS_RAW */
    MAV_DATA_STREAM_RC_CHANNELS,
    /* RC_CHANNELS_SCALED (HIL) */
    MAV_DATA_STREAM_RAW_CONTROLLER,
    /* GLOBAL_POSITION_INT */
    MAV_DATA_STREAM_POSITION,
    /* ATTITUDE, SIMSTATE (SITL) */
    MAV_DATA_STREAM_EXTRA1,
    /* VFR_HUD */
    MAV_DATA_STREAM_EXTRA2,
    /* AHRS, HWSTATUS, SYSTEM_TIME */
    MAV_DATA_STREAM_EXTRA3,
};

struct mavlink_config
{
    unsigned char streams[8];
    unsigned char uav_sysid, osd_sysid;
    unsigned char heartbeat;
    uint8_t shell_rate;
};

byte mav_message[52]; // in MavLink max size is 50
byte mav_msg_severity;

unsigned int mav_seq = 0;


mavlink_config config_mav;
uint8_t mavbeat = 0;
uint32_t last_mav_beat = 0;
uint32_t last_nobeat_message = 0;
uint32_t lastWritePanel = 0;

uint8_t waitingMAVBeats = 1;

uint8_t mav_type;

uint8_t enable_mav_request = 0;
uint32_t sys_start_time = 0;
uint32_t heatbeat_start_time = 0;
uint32_t armed_start_time = 0;
uint32_t total_armed_time = 0;

float osd_pitch = 0.0;
float osd_roll = 0.0;

float vibex, vibey, vibez;

uint8_t mavlink_requested = 0;
uint32_t osd_mode = 0;
bool motor_armed = false;
bool last_motor_armed = false;
uint8_t base_mode = 0;

// true when we have received at least 1 MAVLink packet
bool mavlink_active;

int packet_drops = 0;
int parse_error = 0;

#define MAX_STREAMS 7

bool getBit(unsigned char byte, int position) // position in range 0-7
{
    return (byte >> position) & 0x1;
}


void heartbeat_validation(void)
{
    unsigned long now;


    now = millis();
    // if no mavlink update for 3 secs, show warning and request mavlink rate again
    if (now > (last_mav_beat + 3000))
    {
        if (waitingMAVBeats && (now > (last_nobeat_message + 5000)) )         //Do not flood message queue with No heartbeat messages
        {
            message_buffer_add_line("No MAVLink heartbeat received!", 1);     //TODO:Move strings to an external definition
            last_nobeat_message = now;
        }
        heatbeat_start_time = 0;
        waitingMAVBeats = 1;
    }

    if (enable_mav_request == 1)
    {
        for (int n = 0; n < 3; n++)
        {
            request_mavlink_rates(); // Three times to make sure it will be readed
            delay(200); // wait for 100ms
        }
        enable_mav_request = 0;
        waitingMAVBeats = 0;
    }
}


void request_mavlink_rates(void)
{
    const unsigned char MAVStreams[MAX_STREAMS] = { MAV_DATA_STREAM_RAW_SENSORS, 
                                                    MAV_DATA_STREAM_EXTENDED_STATUS,
                                                    MAV_DATA_STREAM_RC_CHANNELS,
                                                    MAV_DATA_STREAM_POSITION,
                                                    MAV_DATA_STREAM_EXTRA1,
                                                    MAV_DATA_STREAM_EXTRA2,
                                                    MAV_DATA_STREAM_EXTRA3 };
    uint16_t MAVRates[MAX_STREAMS] = { 0x01, 0x05, 0x05, 0x02, 0x0a, 0x05, 0x02 };

    for (uint32_t i = 0; i < MAX_STREAMS; i++)
    {
//        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0, mavlink_system.sysid, mavlink_system.compid, MAVStreams[i], MAVRates[i], 1);
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0, 0, 0, MAVStreams[i], MAVRates[i], 1);
    }
}

void request_mavlink_battery_capacity(void)
{
//    mavlink_msg_param_request_read_send(MAVLINK_COMM_0,mavlink_system.sysid, mavlink_system.compid, "BATT_CAPACITY",-1);
    mavlink_msg_param_request_read_send(MAVLINK_COMM_0,0, 0, "BATT_CAPACITY",-1);
}



void read_mavlink()
{
    mavlink_message_t msg;
    mavlink_status_t mv_status;

	int mm = 0;

    // grabing data
    while (Serial1.available() > 0)
    {
        uint8_t c = Serial1.read();

        // trying to grab msg
        if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &mv_status))
        {
            // It returned non -1, so we have a valid message in the &msg buffer

            mavlink_seen[msg.msgid] = millis(); // Update the last seen pointer for the message;
            mavlink_active = 1; // We are having active mavlink communication

            // handle msg

            switch (msg.msgid)
            {
            case MAVLINK_MSG_ID_HEARTBEAT:
            {
                if (msg.compid != 1)
                {
                    // Message is not from ardupilot, discard
                    break;
                }
                mav_type = mavlink_msg_heartbeat_get_type(&msg);
       
                if (mav_type == MAV_TYPE_GCS)
                {
                    // Message is from a GCS, ignore it 
                    break;
                }

                //Ok it is from an arupilot UAV, go ahead a process
                mavbeat = 1;                        // heartbeat is received
                last_mav_beat = millis();
                osd.mav_type = mav_type;            // We already has this
                osd.mode.mode = mavlink_msg_heartbeat_get_custom_mode(&msg);
                osd.base_mode = mavlink_msg_heartbeat_get_base_mode(&msg);
                osd.system_status = mavlink_msg_heartbeat_get_system_status(&msg);

                if (getBit(osd.base_mode, 7))
                    osd.arming_status = true;
                else
                    osd.arming_status = false;

                if (waitingMAVBeats == 1)
                {
                    enable_mav_request = 1;
                    waitingMAVBeats = 0;
                }
            }
            break;

            case MAVLINK_MSG_ID_ATTITUDE:
            {
                osd.horizon.pitch = ToDeg(mavlink_msg_attitude_get_pitch(&msg));
                osd.horizon.roll = ToDeg(mavlink_msg_attitude_get_roll(&msg));
                // osd_yaw = ToDeg(mavlink_msg_attitude_get_yaw(&msg));
				mm++;
                break;
            }

            case MAVLINK_MSG_ID_VFR_HUD:
            {
                //osd.home.orientation = mavlink_msg_vfr_hud_get_heading(&msg);
                osd.airspeed = mavlink_msg_vfr_hud_get_airspeed(&msg);
                osd.groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&msg);
                osd.heading = mavlink_msg_vfr_hud_get_heading(&msg); // 0..360 deg, 0=north
                osd.throttle = (uint8_t)mavlink_msg_vfr_hud_get_throttle(&msg);
                osd.alt.altitude = mavlink_msg_vfr_hud_get_alt(&msg);
                osd.vario.vario = mavlink_msg_vfr_hud_get_climb(&msg);
            }
            break;

            case MAVLINK_MSG_ID_SYS_STATUS:
            {

                osd.batt1_v.voltage = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f); // Battery voltage, in millivolts (1 = 1 millivolt)
                osd.batt1_curr.current = ((float)mavlink_msg_sys_status_get_current_battery(&msg) / 100.0f); // Battery current, in 10*milliamperes (1 = 10 milliampere)
                osd.batt1_cap.remaining_capacity = mavlink_msg_sys_status_get_battery_remaining(&msg); // Remaining battery energy: (0%: 0, 100%: 100)
            }
            break;

            case MAVLINK_MSG_ID_RC_CHANNELS:
            {

                osd.rcin[1] = mavlink_msg_rc_channels_get_chan1_raw(&msg);
                osd.rcin[2] = mavlink_msg_rc_channels_get_chan2_raw(&msg);
                osd.rcin[3] = mavlink_msg_rc_channels_get_chan3_raw(&msg);
                osd.rcin[4] = mavlink_msg_rc_channels_get_chan4_raw(&msg);
                osd.rcin[5] = mavlink_msg_rc_channels_get_chan5_raw(&msg);
                osd.rcin[6] = mavlink_msg_rc_channels_get_chan6_raw(&msg);
                osd.rcin[7] = mavlink_msg_rc_channels_get_chan7_raw(&msg);
                osd.rcin[8] = mavlink_msg_rc_channels_get_chan8_raw(&msg);
                osd.rcin[9] = mavlink_msg_rc_channels_get_chan9_raw(&msg);
                osd.rcin[10] = mavlink_msg_rc_channels_get_chan10_raw(&msg);
                osd.rcin[11] = mavlink_msg_rc_channels_get_chan11_raw(&msg);
                osd.rcin[12] = mavlink_msg_rc_channels_get_chan12_raw(&msg);
                osd.rcin[13] = mavlink_msg_rc_channels_get_chan13_raw(&msg);
                osd.rcin[14] = mavlink_msg_rc_channels_get_chan14_raw(&msg);
                osd.rcin[15] = mavlink_msg_rc_channels_get_chan15_raw(&msg);
                osd.rcin[16] = mavlink_msg_rc_channels_get_chan16_raw(&msg);
                osd.rc_rssi = mavlink_msg_rc_channels_get_rssi(&msg);

				//ctr_state[] contains the actual switch state of a given control channel
				//ctr_chp[]   contains the rc channel id for pip and osd_page channels

                //set up ctr_state variables
                for (int i=0;i<4;i++)
                {
                    if (osd.rcin[osd.ctr_ch[i]] < RC_STATE1) osd.ctr_state[i] = 0;
                    else if (osd.rcin[osd.ctr_ch[i]] < RC_STATE2) osd.ctr_state[i] = 1;
					else if (osd.rcin[osd.ctr_ch[i]] < RC_STATE3) osd.ctr_state[i] = 2;
					else if (osd.rcin[osd.ctr_ch[i]] < RC_STATE4) osd.ctr_state[i] = 3;
					else if (osd.rcin[osd.ctr_ch[i]] < RC_STATE5) osd.ctr_state[i] = 4;
                    else osd.ctr_state[i] = 5;
                }

            }
            break;
            case MAVLINK_MSG_ID_STATUSTEXT:
            {
                mav_msg_severity = mavlink_msg_statustext_get_severity(&msg);
                // if(MAV_SEVERITY_INFO >= mav_msg_severity) {
                byte len = mavlink_msg_statustext_get_text(&msg, (char *)mav_message);
                mav_message[len] = 0; // add trail
                //}
                message_buffer_add_line((char *)mav_message, mav_msg_severity);
                // debug("%u - %s\n",mav_msg_severity, mav_message);
            }
            break;

            case MAVLINK_MSG_ID_GPS_RAW_INT:
            {
                osd.gps.hdop = (float)mavlink_msg_gps_raw_int_get_eph(&msg) / 100.0;
                osd.gps.sat = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
                osd.gps.fix = mavlink_msg_gps_raw_int_get_fix_type(&msg);

				//osd.gps.hdop = 1.2;
				//osd.gps.sat = 8;
				//osd.gps.fix = GPS_FIX_TYPE_3D_FIX;

                osd.stat.gps_status = STATUS_OK;
                if (osd.gps.fix < GPS_FIX_TYPE_3D_FIX)
                {
                    osd.stat.gps_status = STATUS_CRITICAL;
                    break;
                }

                if (osd.gps.sat <= osd.gps.sat_critical || osd.gps.hdop > osd.gps.hdop_critical)
                {
                    osd.stat.gps_status = STATUS_CRITICAL;
                    break;
                }

                if (osd.gps.sat <= osd.gps.sat_warn || osd.gps.hdop > osd.gps.hdop_warn)
                {
                    osd.stat.gps_status = STATUS_WARNING;
                    break;
                }
            }
            break;

            case MAVLINK_MSG_ID_VIBRATION:
            {
                vibex = mavlink_msg_vibration_get_vibration_x(&msg);
                vibey = mavlink_msg_vibration_get_vibration_y(&msg);
                vibez = mavlink_msg_vibration_get_vibration_z(&msg);

                osd.stat.vibe_status = STATUS_OK;
                if (vibex > 15 || vibey > 15 || vibez > 15) osd.stat.vibe_status = STATUS_WARNING;
                if (vibex >= 30 || vibey >= 30 || vibez >= 30) osd.stat.vibe_status = STATUS_CRITICAL;
            }
            break;

            case MAVLINK_MSG_ID_MISSION_ITEM:
            {
                if (mavlink_msg_mission_item_get_seq(&msg) == 0)
                {
                    osd.home.home_coord.lat = DEG2RAD(mavlink_msg_mission_item_get_x(&msg));
                    osd.home.home_coord.lon = DEG2RAD(mavlink_msg_mission_item_get_y(&msg));
                    osd.home.home_alt = (int)mavlink_msg_mission_item_get_z(&msg);
                    // debug("Mission item received:%f, %f\n",osd.home.home_coord.lat, osd.home.home_coord.lon );
                }
            }
                break;

            case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
            {
                osd.home.uav_coord.lat = DEG2RAD(mavlink_msg_global_position_int_get_lat(&msg) / 10000000.0);
                osd.home.uav_coord.lon = DEG2RAD(mavlink_msg_global_position_int_get_lon(&msg) / 10000000.0);
                osd.home.uav_alt = (int)(mavlink_msg_global_position_int_get_alt(&msg) / 1000);
                osd.home.uav_heading = (int)(mavlink_msg_global_position_int_get_hdg(&msg) / 100);

                // Do home widget calculation
                // TODO: Check if we have home widget enabled

                if (osd.home.lock == HOME_LOCKED)
                {
                    osd.home.uav_bearing = (int)get_bearing(&osd.home.home_coord, &osd.home.uav_coord);
                    osd.home.direction = osd.home.uav_bearing + 180;
                    osd.home.direction -= osd.home.uav_heading;
                    if (osd.home.direction < 0) osd.home.direction += 360;
                    osd.home.distance = earth_distance(&osd.home.home_coord, &osd.home.uav_coord);
                    osd.home_w.home_distance = osd.home.distance;
                    osd.home_w.orientation = osd.home.direction;
                }
            }
            break;

            case MAVLINK_MSG_ID_EKF_STATUS_REPORT:
            {
                osd.ekfvel = (int)(mavlink_msg_ekf_status_report_get_velocity_variance(&msg) * 100);
                osd.ekfposh = (int)(mavlink_msg_ekf_status_report_get_pos_horiz_variance(&msg) * 100);
                osd.ekfposv = (int)(mavlink_msg_ekf_status_report_get_pos_vert_variance(&msg) * 100);
                osd.ekfcompass = (int)(mavlink_msg_ekf_status_report_get_compass_variance(&msg) * 100);
                osd.ekfterrain = (int)(mavlink_msg_ekf_status_report_get_terrain_alt_variance(&msg) * 100);

                osd.stat.ekf_status = STATUS_OK;

				if (osd.ekfvel > 50 ||
                    osd.ekfposh > 50 ||
                    osd.ekfposv > 50 ||
                    osd.ekfcompass > 50 ||
                    osd.ekfterrain > 50) osd.stat.ekf_status = STATUS_WARNING;

                if (osd.ekfvel > 80 ||
                    osd.ekfposh > 80 ||
                    osd.ekfposv > 80 ||
                    osd.ekfcompass > 80 ||
                    osd.ekfterrain > 80) osd.stat.ekf_status = STATUS_CRITICAL;
            }
            break;
			
			//Process incoming parameter values (queried by the OSD)
            case MAVLINK_MSG_ID_PARAM_VALUE:
            {
                char param_name[17];
                mavlink_msg_param_value_get_param_id(&msg,(char *)param_name);
                if ( strcmp(param_name,"BATT_CAPACITY") == 0 )
                {
                    osd.batt1_cap.max_capacity = (int)mavlink_msg_param_value_get_param_value(&msg);
                }
            }
			break;
			case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
			{
				unsigned char sys, comp;

				sys = mavlink_msg_param_request_list_get_target_system(&msg);
				comp = mavlink_msg_param_request_list_get_target_component(&msg);

				if ((sys != OSD_SYS_ID) || (comp != MAV_COMP_ID_OSD))
					break;

				param_send_index = 0; //init sending


			}
			break;
			case MAVLINK_MSG_ID_PARAM_SET:
			{
				unsigned char sys, comp;
				mavlink_message_t msg2;
				//unsigned int len;
				char param_name[17];
				float param_value;
				int idx;

				sys = mavlink_msg_param_set_get_target_system(&msg);
				comp = mavlink_msg_param_set_get_target_component(&msg);

				if ((sys != OSD_SYS_ID) || (comp != MAV_COMP_ID_OSD))
					break;
				
				//len = mavlink_msg_param_set_get_param_id(&msg, param_name);
				param_name[16] = '\0';

				param_value = mavlink_msg_param_set_get_param_value(&msg);

				//debug("set_param: %s - %f\n", param_name, param_value);

				idx = params_set_value(param_name, param_value, 1);

				/* broadcast new parameter value */
				mavlink_msg_param_value_pack(OSD_SYS_ID, MAV_COMP_ID_OSD, &msg2,
					param_name, param_value, MAVLINK_TYPE_FLOAT,
					total_params, idx);
				mavlink_send_msg(&msg2);

			}



            default:
                // Do nothing
                break;
            }
			//Check for timeslot expiry
			//Abandon MavLink reading if we used up our timeslot.

        }
    }
    // Update global packet drops counter
    packet_drops += mv_status.packet_rx_drop_count;
    parse_error += mv_status.parse_error;

	//debug("Attitude messages processed:%u\n", mm);
}


void send_param_list()
{

	if (param_send_index == total_params) return;

	mavlink_message_t msg;
	float param_value;
	char param_name[17];

	param_value = get_parameter_value(param_send_index, param_name);
	mavlink_msg_param_value_pack(OSD_SYS_ID, MAV_COMP_ID_OSD, &msg,
		param_name, param_value, MAVLINK_TYPE_FLOAT,
		total_params, param_send_index++);
	mavlink_send_msg(&msg);
}





void mavlink_send_msg(mavlink_message_t *msg)
{

    unsigned int len;
    unsigned char buf[MAVLINK_MAX_PACKET_LEN];

    len = mavlink_msg_to_send_buffer(buf, msg);
    
    Serial1.write(buf, len);

}





unsigned long mavdata_age(unsigned int id)
{
    if (mavlink_seen[id] != 0)
        return millis() - mavlink_seen[id];
    else
        return 99999999;
}

void heartbeat_out(void)
{
    mavlink_message_t msg;
   
    mavlink_msg_heartbeat_pack(OSD_SYS_ID,
            MAV_COMP_ID_OSD, &msg, MAV_TYPE_ONBOARD_CONTROLLER,
            MAV_AUTOPILOT_INVALID,
            MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, // base_mode
            0, //custom_mode
            MAV_STATE_ACTIVE);

    mavlink_send_msg(&msg);
}
