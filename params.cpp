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

#define MAX_CONFIG_PARAMS   (50)

/*
const struct param_def params_mavlink[] = {
    PARAM("MAV_UAVSYSID", MAV_PARAM_TYPE_UINT8, &config.mav.uav_sysid, NULL),
    PARAM("MAV_OSDSYSID", MAV_PARAM_TYPE_UINT8, &config.mav.osd_sysid, NULL),
    PARAM("MAV_HRTBEAT", MAV_PARAM_TYPE_UINT8, &config.mav.heartbeat, NULL),
    PARAM_END,
};
*/





const char *mavdata_type_name[] = {
    "CHAR", "UINT8_T", "INT8_T",
    "UINT16_T", "INT16_T",
    "UINT32_T", "INT32_T",
    "UINT64_T", "INT64_T",
    "FLOAT", "DOUBLE"
};


float cast2float(void *value, unsigned char type)
{
    switch (type) {
        case MAV_PARAM_TYPE_UINT8:
            return (float) *((unsigned char*) (value));
        case MAV_PARAM_TYPE_INT8:
            return (float) *((char*) (value));
        case MAV_PARAM_TYPE_UINT16:
            return (float) *((unsigned int*) (value));
        case MAV_PARAM_TYPE_INT16:
            return (float) *((int*) (value));
        case MAV_PARAM_TYPE_REAL32:
            return (float) *((float*) (value));
        default:
            return 0;
    }
}

static void cast2param(struct param_def *p, float v)
{
    switch (p->type) {
        case MAV_PARAM_TYPE_UINT8:
            *((unsigned char*) (p->value)) = (unsigned char) v;
            break;
        case MAV_PARAM_TYPE_INT8:
            *((char*) (p->value)) = (char) v;
            break;
        case MAV_PARAM_TYPE_UINT16:
            *((unsigned int*) (p->value)) = (unsigned int) v;
            break;
        case MAV_PARAM_TYPE_INT16:
            *((int*) (p->value)) = (int) v;
            break;
        case MAV_PARAM_TYPE_REAL32:
            *((float*) (p->value)) = (float) v;
            break;
        default:
            break;
    }
}
