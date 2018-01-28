#include "osdeluxe.h"



extern mavlink_system_t mavlink_system = {0,0}; //Ardupilot:7,1  Pixhawk:100,50
extern mavlink_message_t msg; 
extern mavlink_status_t mv_status;


void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
    switch(chan) {
	case MAVLINK_COMM_0:
		Serial1.write(ch);//  mavlink_usart_send_byte(ch);
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

struct mavlink_config {
    unsigned char streams[8];
    unsigned char uav_sysid, osd_sysid;
    unsigned char heartbeat;
    uint8_t shell_rate;
};

mavlink_config config_mav;

uint8_t mavbeat = 0;
uint32_t lastMAVBeat = 0;
uint32_t lastWritePanel = 0;
uint8_t waitingMAVBeats = 1;
uint8_t apm_mav_type;
uint8_t apm_mav_system;
uint8_t apm_mav_component;
uint8_t enable_mav_request = 0;
uint32_t sys_start_time = 0;
uint32_t heatbeat_start_time = 0;
uint32_t armed_start_time = 0;
uint32_t total_armed_time = 0;

float osd_pitch = 0.0;
float osd_roll  = 0.0;

uint8_t mavlink_requested = 0;
uint32_t osd_mode = 0;
bool motor_armed = false;
bool last_motor_armed = false;
uint8_t base_mode = 0;

// true when we have received at least 1 MAVLink packet
static bool mavlink_active;
static uint8_t crlf_count = 0;

static int packet_drops = 0;
static int parse_error = 0;

#define MAX_STREAMS 6


bool getBit(unsigned char byte, int position) // position in range 0-7
{
    return (byte >> position) & 0x1;
}

void request_mavlink_rates(void)
{
    const unsigned char MAVStreams[MAX_STREAMS] = {MAV_DATA_STREAM_RAW_SENSORS,
        MAV_DATA_STREAM_EXTENDED_STATUS,
        MAV_DATA_STREAM_RC_CHANNELS,
        MAV_DATA_STREAM_POSITION,
        MAV_DATA_STREAM_EXTRA1, 
        MAV_DATA_STREAM_EXTRA2};
    //uint16_t MAVRates[MAX_STREAMS] = {0x01, 0x02, 0x05, 0x02, 0x05, 0x02};
	uint16_t MAVRates[MAX_STREAMS] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A};

//	if(apm_mav_component == 0x32) //pixhawk origin FW
//	{
//		return; //we need not change the rate
//	}
    for (uint32_t i=0; i < MAX_STREAMS; i++) {
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,
            mavlink_system.sysid, mavlink_system.compid,
            MAVStreams[i], MAVRates[i], 1);
    }
}

void read_mavlink(){
    mavlink_message_t msg; 
    mavlink_status_t mv_status;

    //grabing data 
    while(Serial1.available() > 0) { 
        uint8_t c = Serial1.read();

        /* allow CLI to be started by hitting enter 3 times, if no
        heartbeat packets have been received */
/*
        if (mavlink_active == 0 && millis() < 20000 && millis() > 5000) {
            if (c == '\n' || c == '\r') {
                crlf_count++;
            } else {
                crlf_count = 0;
            }
            if (crlf_count == 3) {
                uploadFont();
            }
        }
*/     
        //trying to grab msg  
        if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &mv_status)) {
            mavlink_active = 1;
            //handle msg

            switch(msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                {
                    mavbeat = 1;
                    apm_mav_system    = msg.sysid;
                    apm_mav_component = msg.compid;
                    apm_mav_type      = mavlink_msg_heartbeat_get_type(&msg);            
                    osd_mode = (uint8_t)mavlink_msg_heartbeat_get_custom_mode(&msg);
                    //Mode (arducoper armed/disarmed)
                    base_mode = mavlink_msg_heartbeat_get_base_mode(&msg);
                    if(getBit(base_mode,7)) motor_armed = 1;
                    else motor_armed = 0;

                    //osd_nav_mode = 0;          
                    lastMAVBeat = millis();
                    if(waitingMAVBeats == 1){
                        enable_mav_request = 1;
                    }
                }


                break;
            case MAVLINK_MSG_ID_ATTITUDE:
                {
                    osd_pitch = ToDeg(mavlink_msg_attitude_get_pitch(&msg));
                    osd_roll = ToDeg(mavlink_msg_attitude_get_roll(&msg));
//                    osd_yaw = ToDeg(mavlink_msg_attitude_get_yaw(&msg));
                debug("Roll: %i Pitch: %i\n",(int)osd_roll, (int)osd_pitch);
                }
/*
            case MAVLINK_MSG_ID_SYS_STATUS:
                {

                    osd_vbat_A = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f); //Battery voltage, in millivolts (1 = 1 millivolt)
                    osd_curr_A = mavlink_msg_sys_status_get_current_battery(&msg); //Battery current, in 10*milliamperes (1 = 10 milliampere)         
                    osd_battery_remaining_A = mavlink_msg_sys_status_get_battery_remaining(&msg); //Remaining battery energy: (0%: 0, 100%: 100)
                    //osd_mode = apm_mav_component;//Debug
                    //osd_nav_mode = apm_mav_system;//Debug
                }
                break;

            case MAVLINK_MSG_ID_GPS_RAW_INT:
                {
                    osd_lat = mavlink_msg_gps_raw_int_get_lat(&msg) / 10000000.0f;
                    osd_lon = mavlink_msg_gps_raw_int_get_lon(&msg) / 10000000.0f;
                    osd_fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
                    osd_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
                    osd_cog = mavlink_msg_gps_raw_int_get_cog(&msg);
                }
                break; 
            case MAVLINK_MSG_ID_VFR_HUD:
                {
                    osd_airspeed = mavlink_msg_vfr_hud_get_airspeed(&msg);
                    osd_groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&msg);
                    osd_heading = mavlink_msg_vfr_hud_get_heading(&msg); // 0..360 deg, 0=north
                    osd_throttle = (uint8_t)mavlink_msg_vfr_hud_get_throttle(&msg);
                    osd_alt = mavlink_msg_vfr_hud_get_alt(&msg);
                    osd_climb = mavlink_msg_vfr_hud_get_climb(&msg);
                }
                break;
            case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
                {
//                  nav_roll = mavlink_msg_nav_controller_output_get_nav_roll(&msg);
//                  nav_pitch = mavlink_msg_nav_controller_output_get_nav_pitch(&msg);
//                  nav_bearing = mavlink_msg_nav_controller_output_get_nav_bearing(&msg);
                  wp_target_bearing = mavlink_msg_nav_controller_output_get_target_bearing(&msg);
                  wp_dist = mavlink_msg_nav_controller_output_get_wp_dist(&msg);
//                  alt_error = mavlink_msg_nav_controller_output_get_alt_error(&msg);
//                  aspd_error = mavlink_msg_nav_controller_output_get_aspd_error(&msg);
                  xtrack_error = mavlink_msg_nav_controller_output_get_xtrack_error(&msg);
                }
                break;
            case MAVLINK_MSG_ID_MISSION_CURRENT:
                {
                    wp_number = (uint8_t)mavlink_msg_mission_current_get_seq(&msg);
                }
                break;
            case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
                {
//                    chan1_raw = mavlink_msg_rc_channels_raw_get_chan1_raw(&msg);
//                    chan2_raw = mavlink_msg_rc_channels_raw_get_chan2_raw(&msg);
//                    chan3_raw = mavlink_msg_rc_channels_raw_get_chan3_raw(&msg);
//                    chan4_raw = mavlink_msg_rc_channels_raw_get_chan4_raw(&msg);
                    chan5_raw = mavlink_msg_rc_channels_raw_get_chan5_raw(&msg);
                    chan6_raw = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
                    chan7_raw = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
                    chan8_raw = mavlink_msg_rc_channels_raw_get_chan8_raw(&msg);
                    osd_rssi = mavlink_msg_rc_channels_raw_get_rssi(&msg);
                }
                break;           
            case MAVLINK_MSG_ID_WIND:
                {
                  if (osd_climb < 1.66 && osd_climb > -1.66){
                  osd_winddirection = mavlink_msg_wind_get_direction(&msg); // 0..360 deg, 0=north
                  osd_windspeed = mavlink_msg_wind_get_speed(&msg); //m/s
//                  osd_windspeedz = mavlink_msg_wind_get_speed_z(&msg); //m/s
                  }
                }
                break;
            case MAVLINK_MSG_ID_SCALED_PRESSURE:
                {
                    temperature = mavlink_msg_scaled_pressure_get_temperature(&msg);
                }
                break;
*/
            default:
                //Do nothing
                break;
            }
        }
        //delayMicroseconds(138);
        //next one
    }
    // Update global packet drops counter
    packet_drops += mv_status.packet_rx_drop_count;
    parse_error += mv_status.parse_error;

}


