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



struct osd_settings osd;               //This will contain all osd setting


void save_settings()
{
  unsigned int address = 0;
  char *ptr;


  ptr = (char*)&osd;
  for (address = 0; address < sizeof(osd); address++)
  {
      EEPROM.write(address, *ptr++);
  }

}

void load_settings()
{
  unsigned int address = 0;
  char *ptr;


  ptr = (char*)&osd;
  for (address = 0; address < sizeof(osd); address++)
  {
      *ptr++ = EEPROM.read(address);
  }
}



void default_settings()
{
	osd.ctr2_video_on[0] = 0b00001111;
	osd.ctr2_video_on[1] = 0b00000111;
	osd.ctr2_video_on[2] = 0b00000001;


	osd.vin_sharpening[0] = 0xf;
	osd.vin_saturation[0] = 0x50;
	osd.vin_contrast[0] = 0x64;
	osd.vin_brightness[0] = 0x00;
	osd.vin_functions[0] = 0x00;

	osd.vin_sharpening[1] = 0xf;
	osd.vin_saturation[1] = 0x50;
	osd.vin_contrast[1] = 0x64;
	osd.vin_brightness[1] = 0x00;
	osd.vin_functions[1] = 0x00;

	osd.vin_sharpening[2] = 0xf;
	osd.vin_saturation[2] = 0x50;
	osd.vin_contrast[2] = 0x64;
	osd.vin_brightness[2] = 0x00;
	osd.vin_functions[2] = 0x00;

	osd.vin_sharpening[3] = 0xf;
	osd.vin_saturation[3] = 0x50;
	osd.vin_contrast[3] = 0x64;
	osd.vin_brightness[3] = 0x00;
	osd.vin_functions[3] = 0x00;


    osd.video_channels[0][1].pos_h = 0;
    osd.video_channels[0][1].pos_v = 0;
    osd.video_channels[0][1].len_h = 180;
    osd.video_channels[0][1].input = 0;
    osd.video_channels[0][1].enable = 1;
    osd.video_channels[0][1].popup = 0;

    osd.video_channels[0][2].pos_h = 4;
    osd.video_channels[0][2].pos_v = 0;
    osd.video_channels[0][2].len_h = 64;
    osd.video_channels[0][2].input = 1;
    osd.video_channels[0][2].enable = 0;
    osd.video_channels[0][2].popup = 1;

    osd.video_channels[0][3].pos_h = 116;
    osd.video_channels[0][3].pos_v = 0;
    osd.video_channels[0][3].len_h = 64;
    osd.video_channels[0][3].input = 1;
    osd.video_channels[0][3].enable = 0;
    osd.video_channels[0][3].popup = 1;

	osd.video_channels[0][4].pos_h = 68;
	osd.video_channels[0][4].pos_v = 0;
	osd.video_channels[0][4].len_h = 48;
	osd.video_channels[0][4].input = 1;
	osd.video_channels[0][4].enable = 0;
	osd.video_channels[0][4].popup = 1;



    osd.gps.x = 7;
    osd.gps.y = 232;
    osd.gps.sat_warn = 5;
    osd.gps.color = COLOR_YELLOW;
    osd.gps.sat = 0;
    osd.gps.hdop = 999;
	osd.gps.visible = 0x01;
    

	osd.batt1_v.x = 145;
	osd.batt1_v.y = 230;
	osd.batt1_v.min_cell_voltage = 3.3;
	osd.batt1_v.max_cell_voltage = 4.2;
	osd.batt1_v.red_cell_voltage = 3.6;
	osd.batt1_v.yellow_cell_voltage = 3.8;
	osd.batt1_v.bar_type = BAR_SINGLE_COLOR;
	osd.batt1_v.mix = 0;
	osd.batt1_v.cells = 12;
	osd.batt1_v.voltage = 11.11;
	osd.batt1_v.box = true;
	osd.batt1_v.visible = 0x01;


	osd.batt2_v.x = 145;
	osd.batt2_v.y = 230;
	osd.batt2_v.min_cell_voltage = 3.3;
	osd.batt2_v.max_cell_voltage = 4.2;
	osd.batt2_v.red_cell_voltage = 3.6;
	osd.batt2_v.yellow_cell_voltage = 3.8;
	osd.batt2_v.bar_type = BAR_SINGLE_COLOR;
	osd.batt2_v.mix = 0;
	osd.batt2_v.cells = 6;
	osd.batt2_v.voltage = 22.22;
	osd.batt2_v.box = true;
	osd.batt2_v.visible = 0x00;


	osd.batt1_cap.x = 145;
	osd.batt1_cap.y = 250;
	osd.batt1_cap.max_capacity = 0;
	osd.batt1_cap.remaining_capacity = 50;
	osd.batt1_cap.bar_type = BAR_SINGLE_COLOR;
	osd.batt1_cap.mix = true;
	osd.batt1_cap.box = false;
	osd.batt1_cap.visible = 0x01;

	osd.batt2_cap.x = 145;
	osd.batt2_cap.y = 230;
	osd.batt2_cap.max_capacity = 3000;
	osd.batt2_cap.remaining_capacity = 10;
	osd.batt2_cap.bar_type = BAR_SINGLE_COLOR;
	osd.batt2_cap.mix = true;
	osd.batt2_cap.box = true;
	osd.batt2_cap.visible = 0x00;

	osd.batt1_curr.x = 145;
	osd.batt1_curr.y = 272;
	osd.batt1_curr.mix = false;
	osd.batt1_curr.box = true;
	osd.batt1_curr.visible = 0x01;
	osd.batt1_curr.current = 100;

	osd.batt2_curr.x = 50;
	osd.batt2_curr.y = 50;
	osd.batt2_curr.mix = true;
	osd.batt2_curr.box = true;
	osd.batt2_curr.visible = 0x00;

    osd.stat.x = 7;
    osd.stat.y = 253;
    osd.stat.mix = 0;
    osd.stat.gps_status = STATUS_NONE;
    osd.stat.ekf_status = STATUS_NONE;
    osd.stat.vibe_status = STATUS_NONE;
	osd.stat.visible = 0x01;

    osd.alt.x = 7;
    osd.alt.y = 270;
    osd.alt.mix = 0;
    osd.alt.altitude = 0;
	osd.alt.visible = 0x01;

    osd.vario.x = 165;
    osd.vario.y = 110;
    osd.vario.h = 100;
    osd.vario.w = 4;
    osd.vario.vario = 0;
    osd.vario.vario_max = 5.0f;
    osd.vario.mix = 1;
    osd.vario.num_pos = POS_BELOW;
	osd.vario.visible = 0x01;

    osd.home_w.x = 90;
    osd.home_w.y = 252;
    osd.home_w.orientation = 0;
    osd.home_w.home_distance = 0;
	osd.home_w.visible = 0x01;


    osd.horizon.x = 146;
    osd.horizon.y = 145;
	osd.horizon.visible = 0x01;


   osd.mode.mode_x =  90;
   osd.mode.mode_y = 5;

   osd.mode.fs_x =  90;
   osd.mode.fs_y = 23;

   osd.mode.arm_x =  90;
   osd.mode.arm_y = 36;

   osd.mode.mix = 0;
   osd.mode.mode = 0;

   osd.mode.visible = 0x01;

   osd.arming_status = false;
   osd.displayed_arming_status = true;
   osd.armed_start_time = 0;

   osd.last_capacity_query = 0;

   osd.ctr_ch[0] = 10;
   osd.ctr_ch[1] = 11;
   osd.ctr_ch[2] = 12;
   osd.ctr_ch[3] = 13;
    
	osd.pull.x = 7;
	osd.pull.y = 105;
	osd.pull.mix = false;
	osd.pull.pull = 0;
	osd.pull.warning = 7*9.8f;
	osd.pull.visible = 0x01;


	osd.msg_widget.x = 4;
	osd.msg_widget.y = 218;
	osd.msg_widget.visible = 0x01;

}