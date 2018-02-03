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



void init_home(void)
{
    osd.home.lock = HOME_NONE;
    osd.home.last_calc = millis();
}

void calc_home(void)
{
    mavlink_message_t this_msg;

    switch (osd.home.lock)
    {
    case HOME_NONE:
    default:
        if (mavdata_age(MAVLINK_MSG_ID_HEARTBEAT) > 5000) return;
        /* check arming status */
        if (osd.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) osd.home.lock = HOME_WAIT;
        break;
    case HOME_WAIT:
        if (mavdata_age(MAVLINK_MSG_ID_MISSION_ITEM) > 2000)
        {
            /* when UAV is armed, home is WP0 */
            mavlink_msg_mission_request_pack(OSD_SYS_ID, MAV_COMP_ID_OSD, &this_msg, 0, MAV_COMP_ID_ALL,0,0);
            mavlink_send_msg(&this_msg);
        }
        else
        {
            osd.home.lock = HOME_GOT;
        }
        break;
    case HOME_GOT:
        osd.home.lock = HOME_LOCKED;
        break;
    case HOME_LOCKED:
    {
        // Do nothing, this calculated at the MAVLINK_MSG_ID_GLOBAL_POSITION_INT message parsing in mavlink.cpp
        break;
    }
    }
}

float earth_distance(struct gps_coord_t *c1, struct gps_coord_t *c2)
{
    float dphi, dlam, sin_dphi, sin_dlam;
    float a, c;

    dphi = c2->lat - c1->lat;
    dlam = c2->lon - c1->lon;
    sin_dphi = sin(dphi/2);
    sin_dlam = sin(dlam/2);
    a = sin_dphi * sin_dphi + cos(c1->lat) * cos(c2->lat) * sin_dlam * sin_dlam;
    c = 2 * atan2(sqrt(a), sqrt(1-a));
    c = c * ((float) EARTH_RADIUS);

    return c;
}

float get_bearing(struct gps_coord_t *c1, struct gps_coord_t *c2)
{
    float y = sin(c2->lon - c1->lon) * cos(c2->lat);
    float x = cos(c1->lat)*sin(c2->lat) - sin(c1->lat)*cos(c2->lat)*cos(c2->lon - c1->lon);
    return RAD2DEG(atan2(y, x));
}
