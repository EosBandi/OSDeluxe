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

#define PARAM(n, t, p, c) {  n, t, (void *) p,  c }
#define PARAM_END { "" }

unsigned char total_params;
unsigned char param_send_index;

struct param_def parameters[] = {
	//Generic parameters

	PARAM("VIN1_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[0], &update_vin_settings),
	PARAM("VIN1_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[0], &update_vin_settings),
	PARAM("VIN1_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[0], &update_vin_settings),
	PARAM("VIN1_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[0], &update_vin_settings),
	PARAM("VIN1_FUNCTIONS",  MAV_PARAM_TYPE_UINT8,&osd.vin_functions[0], &update_vin_settings),


	PARAM("VIN2_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[1], &update_vin_settings),
	PARAM("VIN2_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[1], &update_vin_settings),
	PARAM("VIN2_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[1], &update_vin_settings),
	PARAM("VIN2_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[1], &update_vin_settings),
	PARAM("VIN2_FUNCTIONS",  MAV_PARAM_TYPE_UINT8,&osd.vin_functions[1], &update_vin_settings),

	PARAM("VIN3_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[1], &update_vin_settings),
	PARAM("VIN3_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[1], &update_vin_settings),
	PARAM("VIN3_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[1], &update_vin_settings),
	PARAM("VIN3_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[1], &update_vin_settings),
	PARAM("VIN3_FUNCTIONS",  MAV_PARAM_TYPE_UINT8,&osd.vin_functions[1], &update_vin_settings),

	PARAM("VIN4_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_sharpening[1], &update_vin_settings),
	PARAM("VIN4_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_saturation[1], &update_vin_settings),
	PARAM("VIN4_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_contrast[1], &update_vin_settings),
	PARAM("VIN4_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_brightness[1], &update_vin_settings),
	PARAM("VIN4_FUNCTIONS",  MAV_PARAM_TYPE_UINT8,&osd.vin_functions[1], &update_vin_settings),


	PARAM("VARIO_POSX", MAV_PARAM_TYPE_UINT16, &osd.vario.x, NULL),
	PARAM("VARIO_POSY", MAV_PARAM_TYPE_UINT16, &osd.vario.y, NULL),
	PARAM("VARIO_HEIGHT", MAV_PARAM_TYPE_UINT16, &osd.vario.h, NULL),
	PARAM("VARIO_WIDTH", MAV_PARAM_TYPE_UINT16, &osd.vario.w, NULL),
	PARAM("VARIO_NUMPOS", MAV_PARAM_TYPE_UINT8, &osd.vario.num_pos, NULL),
	PARAM("VARIO_MAXIMUM", MAV_PARAM_TYPE_REAL32, &osd.vario.vario_max, NULL),
	PARAM("VARIO_MIX", MAV_PARAM_TYPE_UINT8, &osd.vario.mix, NULL),

	PARAM("GPS_POSX", MAV_PARAM_TYPE_UINT16, &osd.gps.x, NULL),
	PARAM("GPS_POSY", MAV_PARAM_TYPE_UINT16, &osd.gps.y, NULL),
	PARAM("GPS_SAT_WARNING", MAV_PARAM_TYPE_UINT8, &osd.gps.sat_warn, NULL),
	PARAM("GPS_HDOP_WARNING", MAV_PARAM_TYPE_REAL32, &osd.gps.hdop_warn, NULL),
	PARAM("GPS_SAT_CRITICAL", MAV_PARAM_TYPE_UINT8, &osd.gps.sat_critical, NULL),
	PARAM("GPS_HDOP_CRITCAL", MAV_PARAM_TYPE_REAL32, &osd.gps.hdop_critical, NULL),

//     01234567890123456
	PARAM("BAT1V_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt1_v.x, NULL),
	PARAM("BAT1V_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt1_v.y, NULL),
	PARAM("BAT1V_CELLS", MAV_PARAM_TYPE_UINT8, &osd.batt1_v.cells, NULL),
	PARAM("BAT1V_MINCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt1_v.min_cell_voltage, NULL),
	PARAM("BAT1V_MAXCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt1_v.max_cell_voltage, NULL),
	PARAM("BAT1V_YELWCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt1_v.yellow_cell_voltage, NULL),
	PARAM("BAT1V_REDCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt1_v.red_cell_voltage, NULL),
	PARAM("BAT1V_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt1_v.bar_type, NULL),
	PARAM("BAT1V_MIX", MAV_PARAM_TYPE_UINT8, &osd.batt1_v.mix, NULL),
	PARAM("BAT1V_BOX", MAV_PARAM_TYPE_UINT8, &osd.batt1_v.box, NULL),

	PARAM("BAT2V_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt2_v.x, NULL),
	PARAM("BAT2V_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt2_v.y, NULL),
	PARAM("BAT2V_CELLS", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.cells, NULL),
	PARAM("BAT2V_MINCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.min_cell_voltage, NULL),
	PARAM("BAT2V_MAXCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.max_cell_voltage, NULL),
	PARAM("BAT2V_YELWCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.yellow_cell_voltage, NULL),
	PARAM("BAT2V_REDCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.red_cell_voltage, NULL),
	PARAM("BAT2V_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.bar_type, NULL),
	PARAM("BAT2V_MIX", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.mix, NULL),
	PARAM("BAT2V_BOX", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.box, NULL),

	PARAM("BAT1CAP_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt1_cap.x, NULL),
	PARAM("BAT1CAP_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt1_cap.y, NULL),
	PARAM("BAT1CAP_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt1_cap.bar_type, NULL),
	PARAM("BAT1CAP_MIX", MAV_PARAM_TYPE_UINT8, &osd.batt1_cap.mix, NULL),
	PARAM("BAT1CAP_BOX", MAV_PARAM_TYPE_UINT8, &osd.batt1_cap.box, NULL),

	PARAM("BAT2CAP_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt2_cap.x, NULL),
	PARAM("BAT2CAP_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt2_cap.y, NULL),
	PARAM("BAT2CAP_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt2_cap.bar_type, NULL),
	PARAM("BAT2CAP_MIX", MAV_PARAM_TYPE_UINT8, &osd.batt2_cap.mix, NULL),
	PARAM("BAT2CAP_BOX", MAV_PARAM_TYPE_UINT8, &osd.batt2_cap.box, NULL),

	PARAM("BAT1CUR_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt1_curr.x, NULL),
	PARAM("BAT1CUR_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt1_curr.y, NULL),
	PARAM("BAT1CUR_MIX", MAV_PARAM_TYPE_UINT8, &osd.batt1_curr.mix, NULL),
	PARAM("BAT1CUR_BOX", MAV_PARAM_TYPE_UINT8, &osd.batt1_curr.box, NULL),

	PARAM("BAT2CUR_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt2_curr.x, NULL),
	PARAM("BAT2CUR_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt2_curr.y, NULL),
	PARAM("BAT2CUR_MIX", MAV_PARAM_TYPE_UINT8, &osd.batt2_curr.mix, NULL),
	PARAM("BAT2CUR_BOX", MAV_PARAM_TYPE_UINT8, &osd.batt2_curr.box, NULL),

	PARAM("STATUS_POSX", MAV_PARAM_TYPE_UINT16, &osd.stat.x, NULL),
	PARAM("STATUS_POSY", MAV_PARAM_TYPE_UINT16, &osd.stat.y, NULL),
	PARAM("STATUS_MIX", MAV_PARAM_TYPE_UINT8, &osd.stat.mix, NULL),


	PARAM("ALT_POSX", MAV_PARAM_TYPE_UINT16, &osd.alt.x, NULL),
	PARAM("ALT_POSY", MAV_PARAM_TYPE_UINT16, &osd.alt.y, NULL),
	PARAM("ALT_MIX", MAV_PARAM_TYPE_UINT8, &osd.alt.mix, NULL),

	PARAM("PULL_POSX", MAV_PARAM_TYPE_UINT16, &osd.pull.x, NULL),
	PARAM("PULL_POSY", MAV_PARAM_TYPE_UINT16, &osd.pull.y, NULL),
	PARAM("PULL_MIX", MAV_PARAM_TYPE_UINT8, &osd.pull.mix, NULL),
	PARAM("PULL_WARNING", MAV_PARAM_TYPE_REAL32, &osd.pull.warning, NULL),

	PARAM("HOME_POSX", MAV_PARAM_TYPE_UINT16, &osd.home_w.x, NULL),
	PARAM("HOME_POSY", MAV_PARAM_TYPE_UINT16, &osd.home_w.y, NULL),

	PARAM("MODE_POSX", MAV_PARAM_TYPE_UINT16, &osd.mode.mode_x, NULL),
	PARAM("MODE_POSY", MAV_PARAM_TYPE_UINT16, &osd.mode.mode_y, NULL),

	PARAM("FAILSAFE_POSX", MAV_PARAM_TYPE_UINT16, &osd.mode.fs_x, NULL),
	PARAM("FAILSAFE_POSY", MAV_PARAM_TYPE_UINT16, &osd.mode.fs_y, NULL),

	PARAM("ARM_POSX", MAV_PARAM_TYPE_UINT16, &osd.mode.arm_x, NULL),
	PARAM("ARM_POSY", MAV_PARAM_TYPE_UINT16, &osd.mode.arm_y, NULL),


	PARAM("MESSAGE_POSX", MAV_PARAM_TYPE_UINT16, &osd.msg_widget.x, NULL),
	PARAM("MESSAGE_POSY", MAV_PARAM_TYPE_UINT16, &osd.msg_widget.y, NULL),

	PARAM("CTR_RC_CH_1", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[0], NULL),
	PARAM("CTR_RC_CH_2", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[1], NULL),
	PARAM("CTR_RC_CH_3", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[2], NULL),
	PARAM("CTR_RC_CH_4", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[3], NULL),


	// Bit coded field for visible chanels (four leas significatn bits)
	PARAM("CTR2_0_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2_video_on[0], &update_channel_onoff),
	PARAM("CTR2_1_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2_video_on[1], &update_channel_onoff),
	PARAM("CTR2_2_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2_video_on[2], &update_channel_onoff),

	//bit coded field for osd widget visibility by pages (1-8 pages)
	PARAM("PAGE_GPS", MAV_PARAM_TYPE_UINT8, &osd.gps.visible, NULL),
	PARAM("PAGE_BATT1_VOLT", MAV_PARAM_TYPE_UINT8, &osd.batt1_v.visible, NULL),
	PARAM("PAGE_BATT2_VOLT", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.visible, NULL),
	PARAM("PAGE_BATT1_CURR", MAV_PARAM_TYPE_UINT8, &osd.batt1_curr.visible, NULL),
	PARAM("PAGE_BATT2_CURR", MAV_PARAM_TYPE_UINT8, &osd.batt2_curr.visible, NULL),
	PARAM("PAGE_BATT1_CAP", MAV_PARAM_TYPE_UINT8, &osd.batt1_cap.visible, NULL),
	PARAM("PAGE_BATT2_CAP", MAV_PARAM_TYPE_UINT8, &osd.batt2_cap.visible, NULL),
	PARAM("PAGE_STAT", MAV_PARAM_TYPE_UINT8, &osd.stat.visible, NULL),
	PARAM("PAGE_ALT", MAV_PARAM_TYPE_UINT8, &osd.alt.visible, NULL),
	PARAM("PAGE_VARIO", MAV_PARAM_TYPE_UINT8, &osd.vario.visible, NULL),
	PARAM("PAGE_HOME", MAV_PARAM_TYPE_UINT8, &osd.home_w.visible, NULL),
	PARAM("PAGE_HORIZON", MAV_PARAM_TYPE_UINT8, &osd.horizon.visible, NULL),
	PARAM("PAGE_MODE", MAV_PARAM_TYPE_UINT8, &osd.mode.visible, NULL),
	PARAM("PAGE_PULL", MAV_PARAM_TYPE_UINT8, &osd.pull.visible, NULL),
	PARAM("PAGE_MESSAGE", MAV_PARAM_TYPE_UINT8, &osd.msg_widget.visible, NULL),

	PARAM("VOUT_COLOR_BAR", MAV_PARAM_TYPE_UINT8, &osd.color_bar_x, &update_vout_settings),
	PARAM("VOUT_COLOR_KILL", MAV_PARAM_TYPE_UINT8, &osd.color_kill_x, &update_vout_settings),


	PARAM_END
};

void update_vout_settings()
{
	unsigned char reg1ab = 0;
	if (osd.color_bar_x) reg1ab =  reg1ab | 0x08;
	if (osd.color_kill_x) reg1ab = reg1ab | 0x04;

	debug("%u\n", reg1ab);
	tw_write_register(0x1ab, reg1ab);



}


void update_vin_settings()
{
	//Check some boundaries
	for (int i = 0; i < 4; i++)
	{
		if (osd.vin_sharpening[i] > 0x0f) osd.vin_sharpening[i] = 0x0f;
		tw_write_register(i * 0x10 + 0x08, 0x30 + osd.vin_sharpening[i]);

		tw_write_register(i * 0x10 + 0x09, osd.vin_contrast[i]);
		tw_write_register(i * 0x10 + 0x0a, osd.vin_brightness[i]);

		tw_write_register(i * 0x10 + 0x0b, osd.vin_saturation[i]);
		tw_write_register(i * 0x10 + 0x0c, osd.vin_saturation[i]);

		tw_write_register(0x111 + (i * 0x08), osd.vin_functions[i]);

	}
}

void update_channel_onoff()
{
	osd.ctr_saved_state[1] = 5;
	debug("Called\n");

}

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
		return (float) *((unsigned char*)(value));
	case MAV_PARAM_TYPE_INT8:
		return (float) *((char*)(value));
	case MAV_PARAM_TYPE_UINT16:
		return (float) *((unsigned short*)(value));
	case MAV_PARAM_TYPE_INT16:
		return (float) *((short*)(value));
	case MAV_PARAM_TYPE_REAL32:
		return (float) *((float*)(value));
	default:
		return 0;
	}
}

void cast2param(struct param_def *p, float v)
{
	switch (p->type) {
	case MAV_PARAM_TYPE_UINT8:
		*((unsigned char*)(p->value)) = (unsigned char)v;
		break;
	case MAV_PARAM_TYPE_INT8:
		*((char*)(p->value)) = (char)v;
		break;
	case MAV_PARAM_TYPE_UINT16:
		*((unsigned short*)(p->value)) = (unsigned short)v;
		break;
	case MAV_PARAM_TYPE_INT16:
		*((short*)(p->value)) = (short)v;
		break;
	case MAV_PARAM_TYPE_REAL32:
		*((float*)(p->value)) = (float)v;
		break;
	default:
		break;
	}
}
