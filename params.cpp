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
#define PARAM(n, t, p, c) {  n, t, (void *) p,  c }
#define PARAM_END { "" }

unsigned char total_params;
unsigned char param_send_index;

struct param_def parameters[] = {
	//Generic parameters

	PARAM("VIN1_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[0], NULL),
	PARAM("VIN1_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[0], NULL),
	PARAM("VIN1_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[0], NULL),
	PARAM("VIN1_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[0], NULL),

	PARAM("VIN2_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[1], NULL),
	PARAM("VIN2_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[1], NULL),
	PARAM("VIN2_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[1], NULL),
	PARAM("VIN2_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[1], NULL),


	PARAM("VIN3_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[1], NULL),
	PARAM("VIN3_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[1], NULL),
	PARAM("VIN3_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[1], NULL),
	PARAM("VIN3_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[1], NULL),


	PARAM("VIN4_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[1], NULL),
	PARAM("VIN4_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[1], NULL),
	PARAM("VIN4_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[1], NULL),
	PARAM("VIN4_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[1], NULL),

	
	PARAM("VARIO_POSX", MAV_PARAM_TYPE_UINT16, &osd.vario.x, NULL),
	PARAM("VARIO_POSY", MAV_PARAM_TYPE_UINT16, &osd.vario.y, NULL),
	PARAM("VARIO_HEIGHT", MAV_PARAM_TYPE_UINT16, &osd.vario.h, NULL),
	PARAM("VARIO_WIDTH", MAV_PARAM_TYPE_UINT16, &osd.vario.w, NULL),
	PARAM("VARIO_NUMPOS", MAV_PARAM_TYPE_UINT8, &osd.vario.num_pos, NULL),
	PARAM("VARIO_MAXIMUM", MAV_PARAM_TYPE_REAL32, &osd.vario.vario_max, NULL),
	PARAM("VARIO_MIX", MAV_PARAM_TYPE_UINT8, &osd.vario.mix, NULL),
	PARAM("VARIO_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.vario.visible, NULL),

	PARAM("GPS_POSX", MAV_PARAM_TYPE_UINT16, &osd.gps.x, NULL),
	PARAM("GPS_POSY", MAV_PARAM_TYPE_UINT16, &osd.gps.y, NULL),
	PARAM("GPS_SAT_WARNING", MAV_PARAM_TYPE_UINT8, &osd.gps.sat_warn, NULL),
	PARAM("GPS_HDOP_WARNING", MAV_PARAM_TYPE_REAL32, &osd.gps.hdop_warn, NULL),
	PARAM("GPS_SAT_CRITICAL", MAV_PARAM_TYPE_UINT8, &osd.gps.sat_critical, NULL),
	PARAM("GPS_HDOP_CRITCAL", MAV_PARAM_TYPE_REAL32, &osd.gps.hdop_critical, NULL),
	PARAM("GPS_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.gps.visible, NULL),

	//Todo add color ?

	PARAM("BATT_POSX", MAV_PARAM_TYPE_UINT16, &osd.bat.x , NULL),
	PARAM("BATT_POSY", MAV_PARAM_TYPE_UINT16, &osd.bat.y, NULL),
	PARAM("BATT_CELLS", MAV_PARAM_TYPE_UINT8, &osd.bat.cells, NULL),
	PARAM("BATT_MIN_CELL_V", MAV_PARAM_TYPE_REAL32, &osd.bat.min_cell_voltage, NULL),
	PARAM("BATT_MAX_CELL_V", MAV_PARAM_TYPE_REAL32, &osd.bat.max_cell_voltage, NULL),
	PARAM("BATT_YELW_CELL_V", MAV_PARAM_TYPE_REAL32, &osd.bat.yellow_cell_voltage, NULL),
	PARAM("BATT_RED_CELL_V", MAV_PARAM_TYPE_REAL32, &osd.bat.red_cell_voltage, NULL),
	PARAM("BATT_BAR_TYPE", MAV_PARAM_TYPE_UINT8, &osd.bat.bar_type, NULL),
    PARAM("BATT_BAR_MIX", MAV_PARAM_TYPE_UINT8, &osd.bat.mix, NULL),
	PARAM("BATT_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.bat.visible, NULL),

	
	
	PARAM("STATUS_POSX", MAV_PARAM_TYPE_UINT16, &osd.stat.x , NULL),
	PARAM("STATUS_POSY", MAV_PARAM_TYPE_UINT16, &osd.stat.y, NULL),
	PARAM("STATUS_MIX", MAV_PARAM_TYPE_UINT8, &osd.stat.mix, NULL),
	PARAM("STATUS_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.stat.visible, NULL),


	PARAM("ALT_POSX", MAV_PARAM_TYPE_UINT16, &osd.alt.x , NULL),
	PARAM("ALT_POSY", MAV_PARAM_TYPE_UINT16, &osd.alt.y, NULL),
	PARAM("ALT_MIX", MAV_PARAM_TYPE_UINT8, &osd.alt.mix, NULL),
	PARAM("ALT_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.alt.visible, NULL),

	PARAM("PULL_POSX", MAV_PARAM_TYPE_UINT16, &osd.pull.x , NULL),
	PARAM("PULL_POSY", MAV_PARAM_TYPE_UINT16, &osd.pull.y, NULL),
	PARAM("PULL_MIX", MAV_PARAM_TYPE_UINT8, &osd.pull.mix, NULL),
	PARAM("PULL_WARNING", MAV_PARAM_TYPE_REAL32, &osd.pull.warning, NULL),
	PARAM("PULL_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.pull.visible, NULL),

	PARAM("HOME_POSX", MAV_PARAM_TYPE_UINT16, &osd.home_w.x , NULL),
	PARAM("HOME_POSY", MAV_PARAM_TYPE_UINT16, &osd.home_w.y, NULL),
	PARAM("HOME_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.home_w.visible, NULL),

	PARAM("MODE_POSX", MAV_PARAM_TYPE_UINT16, &osd.mode.mode_x , NULL),
	PARAM("MODE_POSY", MAV_PARAM_TYPE_UINT16, &osd.mode.mode_y, NULL),

	PARAM("FAILSAFE_POSX", MAV_PARAM_TYPE_UINT16, &osd.mode.fs_x , NULL),
	PARAM("FAILSAFE_POSY", MAV_PARAM_TYPE_UINT16, &osd.mode.fs_y, NULL),

	PARAM("ARM_POSX", MAV_PARAM_TYPE_UINT16, &osd.mode.arm_x , NULL),
	PARAM("ARM_POSY", MAV_PARAM_TYPE_UINT16, &osd.mode.arm_y, NULL),

	PARAM("MODE_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.mode.visible, NULL),


	PARAM("MESSAGE_POSX", MAV_PARAM_TYPE_UINT16, &osd.msg_widget.x , NULL),
	PARAM("MESSAGE_POSY", MAV_PARAM_TYPE_UINT16, &osd.msg_widget.y, NULL),
	PARAM("MESSAGE_VISIBLE", MAV_PARAM_TYPE_UINT8, &osd.msg_widget.visible, NULL),

	PARAM("CTR_RC_CH_1", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[0], NULL),
	PARAM("CTR_RC_CH_2", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[1], NULL),
	PARAM("CTR_RC_CH_3", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[2], NULL),
	PARAM("CTR_RC_CH_4", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[3], NULL),


	// Bit coded field for visible chanels (four leas significatn bits)
	PARAM("CTR2_0_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2[0], NULL),
	PARAM("CTR2_1_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2[1], NULL),
	PARAM("CTR2_2_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2[2], NULL),

	


	PARAM_END
};



const char *mavdata_type_name[] = {
    "CHAR", "UINT8_T", "INT8_T",
    "UINT16_T", "INT16_T",
    "UINT32_T", "INT32_T",
    "UINT64_T", "INT64_T",
    "FLOAT", "DOUBLE"
};


void get_parameter_count()
{
	total_params = 0;

	while (parameters[total_params].name[0] != '\0')
	{
		total_params++;

	}
}


unsigned int get_parameter_index(char *name)
{
	unsigned int idx;

	if (total_params == 0) get_parameter_count();

	for (idx = 0; idx < total_params; idx++) {
		if (strcmp(name, parameters[idx].name) == 0)
			break;
	}
	return idx;
}

float get_parameter_value(int idx, char *name)
{
	struct param_def p;
	struct param_value pv;

	p.value = &pv;

	if (total_params == 0) get_parameter_count();

	if (idx == -1)
		idx = get_parameter_index(name);

	if (idx < total_params) {
		strcpy(name, parameters[idx].name);
		return cast2float(parameters[idx].value, parameters[idx].type);
	}
	else
	{
		return 0;
	}
}

int params_set_value(char *name, float value, unsigned char trigger_cbk)
{
	struct param_def *p, sp;
	int idx;

	idx = get_parameter_index(name);
	debug("name='%s' idx=%d\n", name, idx);
	if (idx < total_params) {
		p = &parameters[idx];
		cast2param(p, value);
		if ((p->cbk != NULL) && trigger_cbk)
			p->cbk();
	}
	return idx;
}




float cast2float(void *value, unsigned char type)
{
    switch (type) {
        case MAV_PARAM_TYPE_UINT8:
            return (float) *((unsigned char*) (value));
        case MAV_PARAM_TYPE_INT8:
            return (float) *((char*) (value));
        case MAV_PARAM_TYPE_UINT16:
            return (float) *((unsigned short*) (value));
        case MAV_PARAM_TYPE_INT16:
            return (float) *((short*) (value));
        case MAV_PARAM_TYPE_REAL32:
            return (float) *((float*) (value));
        default:
            return 0;
    }
}

void cast2param(struct param_def *p, float v)
{
    switch (p->type) {
        case MAV_PARAM_TYPE_UINT8:
            *((unsigned char*) (p->value)) = (unsigned char) v;
            break;
        case MAV_PARAM_TYPE_INT8:
            *((char*) (p->value)) = (char) v;
            break;
        case MAV_PARAM_TYPE_UINT16:
            *((unsigned short*) (p->value)) = (unsigned short) v;
            break;
        case MAV_PARAM_TYPE_INT16:
            *((short*) (p->value)) = (short) v;
            break;
        case MAV_PARAM_TYPE_REAL32:
            *((float*) (p->value)) = (float) v;
            break;
        default:
            break;
    }
}
