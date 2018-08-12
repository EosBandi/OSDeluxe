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

  debug("Settings written to EEPROM\n");

}

uint8_t load_settings()
{
  unsigned int address = 0;
  char *ptr;


  ptr = (char*)&osd;

  for (address = 0; address < sizeof(osd); address++)
  {
      *ptr++ = EEPROM.read(address);
  }

  debug("Settings readed from EEPROM\n");

  return osd.eeprom_version;

}



void default_settings()
{

	debug("Default Settings invoked\n");


	osd.eeprom_version = EEPROM_VERSION;
	osd.settings_size = sizeof(osd);


	memset(&osd.boxes, 0, sizeof(osd.boxes));
	
	osd.boxes[0] = { 0,540,720,29,3,1,1 };

	osd.color_bar_x = 0;
	osd.color_bar_y = 0;
	osd.color_kill_x = 0;
	osd.color_kill_y = 0;


	osd.vout1_gain = 5;
	osd.vout2_gain = 5;
	osd.vout3_gain = 5;

		                    // sharp, sat, contr, bright, enhance, hmir, vmir, bound, peak
	osd.vin_params[0] = { 0x00, 0x50, 0x64, 0x00, 1, 0, 0, 0, 0};
	osd.vin_params[1] = { 0x00, 0x50, 0x64, 0x00, 1, 0, 0, 1, 0};
	osd.vin_params[2] = { 0x00, 0x50, 0x64, 0x00, 1, 0, 0, 1, 0};
	osd.vin_params[3] = { 0x00, 0x50, 0x64, 0x00, 1, 0, 0, 1 ,0};

							//   input, enable, popup, pos_h, pos_vm, lenh, 
	osd.video_channels[0][1] = { 1, 1, 0, 0, 0, 180 };
	osd.video_channels[0][2] = { 2, 0, 1, 0, 0, 64 };
	osd.video_channels[0][3] = { 3, 0, 1, 116, 0, 64 };
	osd.video_channels[0][4] = { 4, 0, 1, 64, 0, 52 };
	
							//   input, enable, popup, pos_h, pos_vm, lenh, 
	osd.video_channels[1][1] = { 1, 1, 0, 0, 0, 180 };
	osd.video_channels[1][2] = { 2, 0, 1, 0, 0, 64 };
	osd.video_channels[1][3] = { 3, 0, 1, 116, 0, 64 };
	osd.video_channels[1][4] = { 4, 0, 1, 64, 0, 52 };

							//   input, enable, popup, pos_h, pos_vm, lenh, 
	osd.video_channels[2][1] = { 1, 1, 0, 0, 0, 180 };
	osd.video_channels[2][2] = { 2, 1, 1, 0, 0, 64 };
	osd.video_channels[2][3] = { 3, 1, 1, 116, 0, 64 };
	osd.video_channels[2][4] = { 4, 0, 1, 64, 0, 52 };

							//   input, enable, popup, pos_h, pos_vm, lenh, 
	osd.video_channels[3][1] = { 4, 1, 0, 0, 0, 180 };
	osd.video_channels[3][2] = { 2, 0, 1, 0, 0, 64 };
	osd.video_channels[3][3] = { 3, 0, 1, 116, 0, 64 };
	osd.video_channels[3][4] = { 1, 1, 1, 64, 0, 52 };

	osd.video_channels[4][1] = { 1, 1, 0, 0, 0, 180 };
	osd.video_channels[4][2] = { 2, 0, 1, 0, 0, 64 };
	osd.video_channels[4][3] = { 3, 0, 1, 116, 0, 64 };
	osd.video_channels[4][4] = { 4, 1, 1, 64, 0, 52 };

	osd.video_channels[5][1] = { 1, 1, 0, 0, 0, 180 };
	osd.video_channels[5][2] = { 2, 0, 1, 0, 0, 64 };
	osd.video_channels[5][3] = { 3, 0, 1, 116, 0, 64 };
	osd.video_channels[5][4] = { 4, 0, 1, 64, 0, 52 };

	osd.gps.x = 10;
    osd.gps.y = 460;
    osd.gps.sat_warn = 6;
    osd.gps.color = COLOR_YELLOW;
    osd.gps.sat = 0;
    osd.gps.hdop = 999;
	osd.gps.visible = 0x01;
    osd.gps.hdop_warn = 1.4f;
    osd.gps.hdop_critical = 2.0f;
    osd.gps.sat_critical = 5;
    

	osd.batt1_v.x = 600;
	osd.batt1_v.y = 460;
	osd.batt1_v.min_cell_voltage = 3.3;
	osd.batt1_v.max_cell_voltage = 4.2;
	osd.batt1_v.red_cell_voltage = 3.6;
	osd.batt1_v.yellow_cell_voltage = 3.8;
	osd.batt1_v.bar_type = BAR_SINGLE_COLOR;
	osd.batt1_v.mix = 0;
	osd.batt1_v.cells = 0;
	osd.batt1_v.box = true;
	osd.batt1_v.visible = 0x01;

	osd.batt1_cap.x = 600;
	osd.batt1_cap.y = 500;
	osd.batt1_cap.max_capacity = 0;
	osd.batt1_cap.bar_type = BAR_SINGLE_COLOR;
	osd.batt1_cap.mix = true;
	osd.batt1_cap.box = false;
	osd.batt1_cap.visible = 0x01;

	osd.batt1_curr.x = 600;
	osd.batt1_curr.y = 544;
	osd.batt1_curr.mix = false;
	osd.batt1_curr.box = true;
	osd.batt1_curr.visible = 0x01;

	osd.batt2_v.x = 145;
	osd.batt2_v.y = 230;
	osd.batt2_v.min_cell_voltage = 3.3;
	osd.batt2_v.max_cell_voltage = 4.2;
	osd.batt2_v.red_cell_voltage = 3.6;
	osd.batt2_v.yellow_cell_voltage = 3.8;
	osd.batt2_v.bar_type = BAR_SINGLE_COLOR;
	osd.batt2_v.mix = 0;
	osd.batt2_v.cells = 6;
	osd.batt2_v.box = true;
	osd.batt2_v.visible = 0x00;

	osd.batt2_curr.x = 50;
	osd.batt2_curr.y = 50;
	osd.batt2_curr.mix = true;
	osd.batt2_curr.box = true;
	osd.batt2_curr.visible = 0x00;

	osd.batt1_power.x = 614;
	osd.batt1_power.y = 440;
	osd.batt1_power.visible = 0x01;

	osd.batt2_power.x = 10;
	osd.batt2_power.y = 450;
	osd.batt2_power.visible = 0x00;

	osd.compass.x = 285;
	osd.compass.y = 10;
	osd.compass.visible = 0x01;

    osd.stat.x = 10;
    osd.stat.y = 510;
    osd.stat.mix = 0;
    osd.stat.gps_status = STATUS_NONE;
    osd.stat.ekf_status = STATUS_NONE;
    osd.stat.vibe_status = STATUS_NONE;
	osd.stat.visible = 0x01;

    osd.alt.x = 10;
    osd.alt.y = 544;
    osd.alt.mix = 0;
	osd.alt.visible = 0x01;

	osd.gs.x = 150;
	osd.gs.y = 544;
	osd.gs.visible = 0x01;

	osd.thr.x = 450;
	osd.thr.y = 544;
	osd.thr.visible = 0x01;

    osd.vario.x = 685;
    osd.vario.y = 188;
    osd.vario.h = 200;
    osd.vario.w = 10;
    osd.vario.vario_max = 5.0f;
    osd.vario.mix = 1;
    osd.vario.num_pos = POS_BELOW;
	osd.vario.visible = 0x01;

	osd.vgraph.x = 613;
	osd.vgraph.y = 120;
	osd.vgraph.h = 60;
	osd.vgraph.w = 80;
	osd.vgraph.vario_max = 2.0f;
	osd.vgraph.visible = 0x01;
	osd.vgraph.mix = 1;


    osd.home_w.x = 360;
    osd.home_w.y = 509;
    osd.home_w.orientation = 0;
    osd.home_w.home_distance = 0;
	osd.home_w.visible = 0x01;


    osd.horizon.x = 360;
    osd.horizon.y = 288;
	osd.horizon.visible = 0x00;


   osd.mode.mode_x =  120;
   osd.mode.mode_y = 512;
   osd.mode.mode_centered = 0;
   osd.mode.mode_short = 0;


   osd.mode.fs_x =  360;
   osd.mode.fs_y = 30;
   osd.mode.fs_centered = 1;

   osd.mode.arm_x =  360;
   osd.mode.arm_y = 320;
   osd.mode.arm_centered = 1;

   osd.mode.visible = 0x01;

   osd.center_cross_visible = 0x01;



   osd.ctr_ch[0] = 10;
   osd.ctr_ch[1] = 11;
   osd.ctr_ch[2] = 12;
   osd.ctr_ch[3] = 13;
    
	osd.pull.x = 7;
	osd.pull.y = 215;
	osd.pull.pull = 0;
	osd.pull.warning = 7*9.8f;
	osd.pull.visible = 0x00;


	osd.msg_widget.x = 10;
	osd.msg_widget.y = 435;
	osd.msg_widget.visible = 0x01;

	osd.msg_list_widget.x = 4;
	osd.msg_list_widget.y = 20;
	osd.msg_list_widget.visible = 0x04;


	osd.move.x = 7;
	osd.move.y = 250;
	osd.move.size = 100;
	osd.move.max = 1.0f;
	osd.move.visible = 0x01;


	osd.radar1.x = 10;
	osd.radar1.y = 10;
	osd.radar1.size = 100;
	osd.radar1.scale = 1;
	osd.radar1.type = 0;
	osd.radar1.visible = 0x01;

	osd.radar2.x = 210;
	osd.radar2.y = 138;
	osd.radar2.size = 300;
	osd.radar2.scale = 2;
	osd.radar2.type = 1;
	osd.radar2.visible = 0x02;

	osd.radar3.x = 100;
	osd.radar3.y = 100;
	osd.radar3.size = 100;
	osd.radar3.scale = 1;
	osd.radar3.type = 2;
	osd.radar3.visible = 0x00;

	osd.rssi.x = 580;
    osd.rssi.y = 20;
    osd.rssi.rssi_critical = 20;
    osd.rssi.rssi_warning = 50;
    osd.rssi.visible = 0x01;


}