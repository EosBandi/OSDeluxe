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

int total_params;						// Number of all params (int, to eliminate warning about unsigned, signed comparisions
int param_send_index;

struct param_def parameters[] = {
	//Generic parameters

	PARAM("VIN1_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].sharpening, &update_vin_settings),
	PARAM("VIN1_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].saturation, &update_vin_settings),
	PARAM("VIN1_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].contrast, &update_vin_settings),
 	PARAM("VIN1_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].brightness, &update_vin_settings),
	PARAM("VIN1_ENHANCE",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].enhance, &update_vin_settings),
	PARAM("VIN1_H_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].h_mirror, &update_vin_settings),
	PARAM("VIN1_V_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].v_mirror, &update_vin_settings),
	PARAM("VIN1_BOUNDARY",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].boundary, &update_vin_settings),
	PARAM("VIN1_PEAKING",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[0].peaking, &update_vin_settings),

	PARAM("VIN2_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].sharpening, &update_vin_settings),
	PARAM("VIN2_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].saturation, &update_vin_settings),
	PARAM("VIN2_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].contrast, &update_vin_settings),
	PARAM("VIN2_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].brightness, &update_vin_settings),
	PARAM("VIN2_ENHANCE",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].enhance, &update_vin_settings),
	PARAM("VIN2_H_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].h_mirror, &update_vin_settings),
	PARAM("VIN2_V_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].v_mirror, &update_vin_settings),
	PARAM("VIN2_BOUNDARY",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].boundary, &update_vin_settings),
	PARAM("VIN2_PEAKING",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[1].peaking, &update_vin_settings),

	PARAM("VIN3_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].sharpening, &update_vin_settings),
	PARAM("VIN3_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].saturation, &update_vin_settings),
	PARAM("VIN3_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].contrast, &update_vin_settings),
	PARAM("VIN3_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].brightness, &update_vin_settings),
	PARAM("VIN3_ENHANCE",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].enhance, &update_vin_settings),
	PARAM("VIN3_H_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].h_mirror, &update_vin_settings),
	PARAM("VIN3_V_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].v_mirror, &update_vin_settings),
	PARAM("VIN3_BOUNDARY",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].boundary, &update_vin_settings),
	PARAM("VIN3_PEAKING",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[2].peaking, &update_vin_settings),

	PARAM("VIN4_SHARPENING", MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].sharpening, &update_vin_settings),
	PARAM("VIN4_SATURATION", MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].saturation, &update_vin_settings),
	PARAM("VIN4_CONTRAST",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].contrast, &update_vin_settings),
	PARAM("VIN4_BRIGHTNESS", MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].brightness, &update_vin_settings),
	PARAM("VIN4_ENHANCE",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].enhance, &update_vin_settings),
	PARAM("VIN4_H_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].h_mirror, &update_vin_settings),
	PARAM("VIN4_V_MIRROR",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].v_mirror, &update_vin_settings),
	PARAM("VIN4_BOUNDARY",   MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].boundary, &update_vin_settings),
	PARAM("VIN4_PEAKING",    MAV_PARAM_TYPE_UINT8,&osd.vin_params[3].peaking, &update_vin_settings),


	PARAM("VARIO_POSX", MAV_PARAM_TYPE_UINT16, &osd.vario.x, NULL),
	PARAM("VARIO_POSY", MAV_PARAM_TYPE_UINT16, &osd.vario.y, NULL),
	PARAM("VARIO_HEIGHT", MAV_PARAM_TYPE_UINT16, &osd.vario.h, NULL),
	PARAM("VARIO_WIDTH", MAV_PARAM_TYPE_UINT16, &osd.vario.w, NULL),
	PARAM("VARIO_NUMPOS", MAV_PARAM_TYPE_UINT8, &osd.vario.num_pos, NULL),
	PARAM("VARIO_MAXIMUM", MAV_PARAM_TYPE_REAL32, &osd.vario.vario_max, NULL),

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

	PARAM("BAT2V_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt2_v.x, NULL),
	PARAM("BAT2V_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt2_v.y, NULL),
	PARAM("BAT2V_CELLS", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.cells, NULL),
	PARAM("BAT2V_MINCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.min_cell_voltage, NULL),
	PARAM("BAT2V_MAXCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.max_cell_voltage, NULL),
	PARAM("BAT2V_YELWCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.yellow_cell_voltage, NULL),
	PARAM("BAT2V_REDCELLV", MAV_PARAM_TYPE_REAL32, &osd.batt2_v.red_cell_voltage, NULL),
	PARAM("BAT2V_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt2_v.bar_type, NULL),

	PARAM("BAT1CAP_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt1_cap.x, NULL),
	PARAM("BAT1CAP_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt1_cap.y, NULL),
	PARAM("BAT1CAP_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt1_cap.bar_type, NULL),

	PARAM("BAT2CAP_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt2_cap.x, NULL),
	PARAM("BAT2CAP_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt2_cap.y, NULL),
	PARAM("BAT2CAP_BARTYPE", MAV_PARAM_TYPE_UINT8, &osd.batt2_cap.bar_type, NULL),

	PARAM("BAT1CUR_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt1_curr.x, NULL),
	PARAM("BAT1CUR_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt1_curr.y, NULL),

	PARAM("BAT2CUR_POSX", MAV_PARAM_TYPE_UINT16, &osd.batt2_curr.x, NULL),
	PARAM("BAT2CUR_POSY", MAV_PARAM_TYPE_UINT16, &osd.batt2_curr.y, NULL),

	PARAM("STATUS_POSX", MAV_PARAM_TYPE_UINT16, &osd.stat.x, NULL),
	PARAM("STATUS_POSY", MAV_PARAM_TYPE_UINT16, &osd.stat.y, NULL),

	PARAM("ALT_POSX", MAV_PARAM_TYPE_UINT16, &osd.alt.x, NULL),
	PARAM("ALT_POSY", MAV_PARAM_TYPE_UINT16, &osd.alt.y, NULL),

	PARAM("PULL_POSX", MAV_PARAM_TYPE_UINT16, &osd.pull.x, NULL),
	PARAM("PULL_POSY", MAV_PARAM_TYPE_UINT16, &osd.pull.y, NULL),
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

	PARAM("GS_POSX", MAV_PARAM_TYPE_UINT16, &osd.gs.x, NULL),
	PARAM("GS_POSY", MAV_PARAM_TYPE_UINT16, &osd.gs.y, NULL),

	PARAM("THROTTLE_POSX", MAV_PARAM_TYPE_UINT16, &osd.thr.x, NULL),
	PARAM("THROTTLE_POSY", MAV_PARAM_TYPE_UINT16, &osd.thr.y, NULL),

	PARAM("MOVE_POSX", MAV_PARAM_TYPE_UINT16, &osd.move.x, NULL),
	PARAM("MOVE_POSY", MAV_PARAM_TYPE_UINT16, &osd.move.y, NULL),
	PARAM("MOVE_SIZE", MAV_PARAM_TYPE_UINT16, &osd.move.size, NULL),
	PARAM("MOVE_MAX",  MAV_PARAM_TYPE_REAL32, &osd.move.max, NULL),


	PARAM("CTR_RC_CH_1", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[0], NULL),
	PARAM("CTR_RC_CH_2", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[1], NULL),
	PARAM("CTR_RC_CH_3", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[2], NULL),
	PARAM("CTR_RC_CH_4", MAV_PARAM_TYPE_UINT8, &osd.ctr_ch[3], NULL),

/*
	// Bit coded field for visible chanels (four leas significatn bits)
	PARAM("CTR2_0_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2_video_on[0], &update_channel_onoff),
	PARAM("CTR2_1_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2_video_on[1], &update_channel_onoff),
	PARAM("CTR2_2_VIDCHON", MAV_PARAM_TYPE_UINT8, &osd.ctr2_video_on[2], &update_channel_onoff),
*/
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
	PARAM("PAGE_GS", MAV_PARAM_TYPE_UINT8, &osd.gs.visible, NULL),
	PARAM("PAGE_THROTTLE", MAV_PARAM_TYPE_UINT8, &osd.thr.visible, NULL),
	PARAM("PAGE_MOVE", MAV_PARAM_TYPE_UINT8, &osd.move.visible, NULL),
	PARAM("PAGE_CENTER_M", MAV_PARAM_TYPE_UINT8, &osd.center_cross_visible , NULL),

	PARAM("VOUT_COLOR_BAR", MAV_PARAM_TYPE_UINT8, &osd.color_bar_x, &update_vout_settings),
	PARAM("VOUT_COLOR_KILL", MAV_PARAM_TYPE_UINT8, &osd.color_kill_x, &update_vout_settings),
	PARAM("VOUT_OUT1_GAIN", MAV_PARAM_TYPE_UINT8, &osd.vout1_gain, &update_vout_settings),
	PARAM("VOUT_OUT2_GAIN", MAV_PARAM_TYPE_UINT8, &osd.vout2_gain, &update_vout_settings),
	PARAM("VOUT_OUT3_GAIN", MAV_PARAM_TYPE_UINT8, &osd.vout3_gain, &update_vout_settings),


	PARAM("PIPM0_POS_H_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][1].pos_h, &update_pip),
	PARAM("PIPM0_POS_V_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][1].pos_v, &update_pip),
	PARAM("PIPM0_SIZE_CH1",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][1].len_h, &update_pip),
	PARAM("PIPM0_INPUT_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][1].input, &update_pip),
	PARAM("PIPM0_POPUP_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][1].popup, &update_pip),
	PARAM("PIPM0_ENA_CH1",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][1].enable, &update_pip),

	PARAM("PIPM0_POS_H_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][2].pos_h, &update_pip),
	PARAM("PIPM0_POS_V_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][2].pos_v, &update_pip),
	PARAM("PIPM0_SIZE_CH2",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][2].len_h, &update_pip),
	PARAM("PIPM0_INPUT_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][2].input, &update_pip),
	PARAM("PIPM0_POPUP_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][2].popup, &update_pip),
	PARAM("PIPM0_ENA_CH2",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][2].enable, &update_pip),

	PARAM("PIPM0_POS_H_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][3].pos_h, &update_pip),
	PARAM("PIPM0_POS_V_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][3].pos_v, &update_pip),
	PARAM("PIPM0_SIZE_CH3",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][3].len_h, &update_pip),
	PARAM("PIPM0_INPUT_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][3].input, &update_pip),
	PARAM("PIPM0_POPUP_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][3].popup, &update_pip),
	PARAM("PIPM0_ENA_CH3",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][3].enable, &update_pip),

	PARAM("PIPM0_POS_H_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][4].pos_h, &update_pip),
	PARAM("PIPM0_POS_V_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][4].pos_v, &update_pip),
	PARAM("PIPM0_SIZE_CH4",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][4].len_h, &update_pip),
	PARAM("PIPM0_INPUT_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][4].input, &update_pip),
	PARAM("PIPM0_POPUP_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][4].popup, &update_pip),
	PARAM("PIPM0_ENA_CH4",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[0][4].enable, &update_pip),

	PARAM("PIPM1_POS_H_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][1].pos_h, &update_pip),
	PARAM("PIPM1_POS_V_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][1].pos_v, &update_pip),
	PARAM("PIPM1_SIZE_CH1",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][1].len_h, &update_pip),
	PARAM("PIPM1_INPUT_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][1].input, &update_pip),
	PARAM("PIPM1_POPUP_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][1].popup, &update_pip),
	PARAM("PIPM1_ENA_CH1",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][1].enable, &update_pip),

	PARAM("PIPM1_POS_H_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][2].pos_h, &update_pip),
	PARAM("PIPM1_POS_V_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][2].pos_v, &update_pip),
	PARAM("PIPM1_SIZE_CH2",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][2].len_h, &update_pip),
	PARAM("PIPM1_INPUT_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][2].input, &update_pip),
	PARAM("PIPM1_POPUP_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][2].popup, &update_pip),
	PARAM("PIPM1_ENA_CH2",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][2].enable, &update_pip),

	PARAM("PIPM1_POS_H_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][3].pos_h, &update_pip),
	PARAM("PIPM1_POS_V_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][3].pos_v, &update_pip),
	PARAM("PIPM1_SIZE_CH3",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][3].len_h, &update_pip),
	PARAM("PIPM1_INPUT_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][3].input, &update_pip),
	PARAM("PIPM1_POPUP_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][3].popup, &update_pip),
	PARAM("PIPM1_ENA_CH3",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][3].enable, &update_pip),

	PARAM("PIPM1_POS_H_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][4].pos_h, &update_pip),
	PARAM("PIPM1_POS_V_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][4].pos_v, &update_pip),
	PARAM("PIPM1_SIZE_CH4",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][4].len_h, &update_pip),
	PARAM("PIPM1_INPUT_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][4].input, &update_pip),
	PARAM("PIPM1_POPUP_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][4].popup, &update_pip),
	PARAM("PIPM1_ENA_CH4",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[1][4].enable, &update_pip),
//---
	PARAM("PIPM2_POS_H_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][1].pos_h, &update_pip),
	PARAM("PIPM2_POS_V_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][1].pos_v, &update_pip),
	PARAM("PIPM2_SIZE_CH1",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][1].len_h, &update_pip),
	PARAM("PIPM2_INPUT_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][1].input, &update_pip),
	PARAM("PIPM2_POPUP_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][1].popup, &update_pip),
	PARAM("PIPM2_ENA_CH1",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][1].enable, &update_pip),

	PARAM("PIPM2_POS_H_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][2].pos_h, &update_pip),
	PARAM("PIPM2_POS_V_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][2].pos_v, &update_pip),
	PARAM("PIPM2_SIZE_CH2",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][2].len_h, &update_pip),
	PARAM("PIPM2_INPUT_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][2].input, &update_pip),
	PARAM("PIPM2_POPUP_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][2].popup, &update_pip),
	PARAM("PIPM2_ENA_CH2",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][2].enable, &update_pip),

	PARAM("PIPM2_POS_H_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][3].pos_h, &update_pip),
	PARAM("PIPM2_POS_V_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][3].pos_v, &update_pip),
	PARAM("PIPM2_SIZE_CH3",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][3].len_h, &update_pip),
	PARAM("PIPM2_INPUT_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][3].input, &update_pip),
	PARAM("PIPM2_POPUP_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][3].popup, &update_pip),
	PARAM("PIPM2_ENA_CH3",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][3].enable, &update_pip),

	PARAM("PIPM2_POS_H_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][4].pos_h, &update_pip),
	PARAM("PIPM2_POS_V_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][4].pos_v, &update_pip),
	PARAM("PIPM2_SIZE_CH4",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][4].len_h, &update_pip),
	PARAM("PIPM2_INPUT_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][4].input, &update_pip),
	PARAM("PIPM2_POPUP_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][4].popup, &update_pip),
	PARAM("PIPM2_ENA_CH4",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[2][4].enable, &update_pip),
//---
	PARAM("PIPM3_POS_H_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][1].pos_h, &update_pip),
	PARAM("PIPM3_POS_V_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][1].pos_v, &update_pip),
	PARAM("PIPM3_SIZE_CH1",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][1].len_h, &update_pip),
	PARAM("PIPM3_INPUT_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][1].input, &update_pip),
	PARAM("PIPM3_POPUP_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][1].popup, &update_pip),
	PARAM("PIPM3_ENA_CH1",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][1].enable, &update_pip),

	PARAM("PIPM3_POS_H_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][2].pos_h, &update_pip),
	PARAM("PIPM3_POS_V_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][2].pos_v, &update_pip),
	PARAM("PIPM3_SIZE_CH2",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][2].len_h, &update_pip),
	PARAM("PIPM3_INPUT_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][2].input, &update_pip),
	PARAM("PIPM3_POPUP_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][2].popup, &update_pip),
	PARAM("PIPM3_ENA_CH2",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][2].enable, &update_pip),

	PARAM("PIPM3_POS_H_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][3].pos_h, &update_pip),
	PARAM("PIPM3_POS_V_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][3].pos_v, &update_pip),
	PARAM("PIPM3_SIZE_CH3",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][3].len_h, &update_pip),
	PARAM("PIPM3_INPUT_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][3].input, &update_pip),
	PARAM("PIPM3_POPUP_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][3].popup, &update_pip),
	PARAM("PIPM3_ENA_CH3",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][3].enable, &update_pip),

	PARAM("PIPM3_POS_H_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][4].pos_h, &update_pip),
	PARAM("PIPM3_POS_V_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][4].pos_v, &update_pip),
	PARAM("PIPM3_SIZE_CH4",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][4].len_h, &update_pip),
	PARAM("PIPM3_INPUT_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][4].input, &update_pip),
	PARAM("PIPM3_POPUP_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][4].popup, &update_pip),
	PARAM("PIPM3_ENA_CH4",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[3][4].enable, &update_pip),
//-----
	PARAM("PIPM4_POS_H_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][1].pos_h, &update_pip),
	PARAM("PIPM4_POS_V_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][1].pos_v, &update_pip),
	PARAM("PIPM4_SIZE_CH1",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][1].len_h, &update_pip),
	PARAM("PIPM4_INPUT_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][1].input, &update_pip),
	PARAM("PIPM4_POPUP_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][1].popup, &update_pip),
	PARAM("PIPM4_ENA_CH1",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][1].enable, &update_pip),

	PARAM("PIPM4_POS_H_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][2].pos_h, &update_pip),
	PARAM("PIPM4_POS_V_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][2].pos_v, &update_pip),
	PARAM("PIPM4_SIZE_CH2",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][2].len_h, &update_pip),
	PARAM("PIPM4_INPUT_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][2].input, &update_pip),
	PARAM("PIPM4_POPUP_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][2].popup, &update_pip),
	PARAM("PIPM4_ENA_CH2",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][2].enable, &update_pip),

	PARAM("PIPM4_POS_H_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][3].pos_h, &update_pip),
	PARAM("PIPM4_POS_V_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][3].pos_v, &update_pip),
	PARAM("PIPM4_SIZE_CH3",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][3].len_h, &update_pip),
	PARAM("PIPM4_INPUT_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][3].input, &update_pip),
	PARAM("PIPM4_POPUP_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][3].popup, &update_pip),
	PARAM("PIPM4_ENA_CH3",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][3].enable, &update_pip),

	PARAM("PIPM4_POS_H_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][4].pos_h, &update_pip),
	PARAM("PIPM4_POS_V_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][4].pos_v, &update_pip),
	PARAM("PIPM4_SIZE_CH4",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][4].len_h, &update_pip),
	PARAM("PIPM4_INPUT_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][4].input, &update_pip),
	PARAM("PIPM4_POPUP_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][4].popup, &update_pip),
	PARAM("PIPM4_ENA_CH4",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[4][4].enable, &update_pip),
//-----
	PARAM("PIPM5_POS_H_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][1].pos_h, &update_pip),
	PARAM("PIPM5_POS_V_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][1].pos_v, &update_pip),
	PARAM("PIPM5_SIZE_CH1",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][1].len_h, &update_pip),
	PARAM("PIPM5_INPUT_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][1].input, &update_pip),
	PARAM("PIPM5_POPUP_CH1", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][1].popup, &update_pip),
	PARAM("PIPM5_ENA_CH1",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][1].enable, &update_pip),

	PARAM("PIPM5_POS_H_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][2].pos_h, &update_pip),
	PARAM("PIPM5_POS_V_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][2].pos_v, &update_pip),
	PARAM("PIPM5_SIZE_CH2",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][2].len_h, &update_pip),
	PARAM("PIPM5_INPUT_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][2].input, &update_pip),
	PARAM("PIPM5_POPUP_CH2", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][2].popup, &update_pip),
	PARAM("PIPM5_ENA_CH2",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][2].enable, &update_pip),

	PARAM("PIPM5_POS_H_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][3].pos_h, &update_pip),
	PARAM("PIPM5_POS_V_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][3].pos_v, &update_pip),
	PARAM("PIPM5_SIZE_CH3",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][3].len_h, &update_pip),
	PARAM("PIPM5_INPUT_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][3].input, &update_pip),
	PARAM("PIPM5_POPUP_CH3", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][3].popup, &update_pip),
	PARAM("PIPM5_ENA_CH3",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][3].enable, &update_pip),

	PARAM("PIPM5_POS_H_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][4].pos_h, &update_pip),
	PARAM("PIPM5_POS_V_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][4].pos_v, &update_pip),
	PARAM("PIPM5_SIZE_CH4",  MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][4].len_h, &update_pip),
	PARAM("PIPM5_INPUT_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][4].input, &update_pip),
	PARAM("PIPM5_POPUP_CH4", MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][4].popup, &update_pip),
	PARAM("PIPM5_ENA_CH4",   MAV_PARAM_TYPE_UINT8, &osd.video_channels[5][4].enable, &update_pip),


	PARAM("TEST_PIP_PAGE",MAV_PARAM_TYPE_UINT8, &g.pip_page, &update_pip),
	PARAM("TEST_OSD_PAGE", MAV_PARAM_TYPE_UINT8, &g.visible_osd_page, NULL),

	PARAM("XXX", MAV_PARAM_TYPE_UINT8, &g.test_byte, &update_test_byte),

	PARAM_END
};


void update_test_byte()
{

	tw_write_register(0x0c5, g.test_byte);

}

void update_pip()
{
	for (U8 i = 1; i<5; i++)
	{
		tw_ch_set_input(i, osd.video_channels[g.pip_page][i].input);
	
		tw_ch_settings(i, osd.video_channels[g.pip_page][i].enable,
			osd.video_channels[g.pip_page][i].popup);
	
		tw_ch_set_window(i, osd.video_channels[g.pip_page][i].pos_h,
			osd.video_channels[g.pip_page][i].pos_v,
			osd.video_channels[g.pip_page][i].len_h);
    
	}

}


void update_vout_settings()
{
	U8 reg1ab = 0;
	if (osd.color_bar_x) reg1ab =  reg1ab | 0x08;
	if (osd.color_kill_x) reg1ab = reg1ab | 0x04;
	tw_write_register(0x1ab, reg1ab);

	if (osd.vout1_gain > 7) osd.vout1_gain = 7;
	if (osd.vout2_gain > 7) osd.vout2_gain = 7;
	if (osd.vout3_gain > 7) osd.vout3_gain = 7;
	
	U8 reg041 = 0;
	reg041 = (osd.vout2_gain << 4) + osd.vout1_gain;
	tw_write_register(0x041, reg041);
	tw_write_register(0x042, osd.vout3_gain);



}


void update_vin_settings()
{
	//Check some boundaries and update all four channels
	for (int i = 0; i < 4; i++)
	{
		if (osd.vin_params[i].sharpening > 0x0f) osd.vin_params[i].sharpening = 0x0f;
		if (osd.vin_params[i].peaking > 0x07) osd.vin_params[i].peaking = 0x07;

		tw_write_register(i * 0x10 + 0x08, 0x30 + osd.vin_params[i].sharpening);
		tw_write_register(i * 0x10 + 0x09, osd.vin_params[i].contrast);
		tw_write_register(i * 0x10 + 0x0a, osd.vin_params[i].brightness);
		tw_write_register(i * 0x10 + 0x0b, osd.vin_params[i].saturation);
		tw_write_register(i * 0x10 + 0x0c, osd.vin_params[i].saturation);
		tw_write_register(i * 0x10 + 0x01, osd.vin_params[i].peaking);

		char func_reg = 0;
		if (osd.vin_params[i].enhance) func_reg += BIT3;
		if (osd.vin_params[i].h_mirror) func_reg += BIT5;
		if (osd.vin_params[i].v_mirror) func_reg += BIT4;
		if (osd.vin_params[i].boundary) func_reg += BIT1;
		tw_write_register(0x111 + (i * 0x08), func_reg);
	}





}

/*
void update_channel_onoff()
{
	osd.ctr_saved_state[1] = 5;
	debug("Called\n");

}
*/
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
	int idx;

	if (total_params == 0) get_parameter_count();

	for (idx = 0; idx < total_params; idx++) {
		if (strcmp(name, parameters[idx].name) == 0)
			break;
	}
	return idx;
}

float get_parameter_value(int idx, char *name)
{
//	struct param_def p;
//	struct param_value pv;

//	p.value = &pv;

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
	struct param_def *p;
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
