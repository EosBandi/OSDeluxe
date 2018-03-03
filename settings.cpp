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

    osd.video_channels[0][1].pos_h = 0;
    osd.video_channels[0][1].pos_v = 0;
    osd.video_channels[0][1].len_h = 180;
    osd.video_channels[0][1].input = INPUT_CH_1;
    osd.video_channels[0][1].enable = 1;
    osd.video_channels[0][1].popup = 0;

    osd.video_channels[0][2].pos_h = 4;
    osd.video_channels[0][2].pos_v = 10;
    osd.video_channels[0][2].len_h = 64;
    osd.video_channels[0][2].input = INPUT_CH_2;
    osd.video_channels[0][2].enable = 1;
    osd.video_channels[0][2].popup = 1;

    osd.video_channels[0][3].pos_h = 116;
    osd.video_channels[0][3].pos_v = 10;
    osd.video_channels[0][3].len_h = 64;
    osd.video_channels[0][3].input = INPUT_CH_3;
    osd.video_channels[0][3].enable = 1;
    osd.video_channels[0][3].popup = 1;

    osd.gps.x = 7;
    osd.gps.y = 232;
    osd.gps.sat_warn = 5;
    osd.gps.color = COLOR_YELLOW;
    osd.gps.sat = 10;
    osd.gps.hdop = 0.21;
    
    osd.bat.x = 145;
    osd.bat.y = 230;
    osd.bat.min_cell_voltage = 3.3;
    osd.bat.max_cell_voltage = 4.2;
    osd.bat.red_cell_voltage = 3.5;
    osd.bat.yellow_cell_voltage = 3.7;
    osd.bat.bar_type = BAR_SINGLE_COLOR;
    osd.bat.mix = 0;
    osd.bat.cells = 3;

    osd.bat.voltage = 23.4;
    osd.bat.current = 10.0;
    osd.bat.max_capacity = 0;
    osd.bat.remaining_capacity = 50;

    osd.stat.x = 7;
    osd.stat.y = 253;
    osd.stat.mix = 0;
    osd.stat.gps_status = STATUS_NONE;
    osd.stat.ekf_status = STATUS_NONE;
    osd.stat.vibe_status = STATUS_NONE;

    osd.alt.x = 7;
    osd.alt.y = 270;
    osd.alt.mix = 0;
    osd.alt.altitude = 0;

    osd.vario.x = 165;
    osd.vario.y = 110;
    osd.vario.h = 100;
    osd.vario.w = 4;
    osd.vario.vario = 0;
    osd.vario.vario_max = 5.0f;
    osd.vario.mix = 1;
    osd.vario.num_pos = POS_BELOW;

    osd.home_w.x = 90;
    osd.home_w.y = 252;
    osd.home_w.orientation = 0;
    osd.home_w.home_distance = 0;


    osd.horizon.x = 146;
    osd.horizon.y = 145;
	osd.horizon.visible = true;


    //osd.mode.mode_x =  57;
    //osd.mode.mode_y = 270;

   osd.mode.mode_x =  90;
   osd.mode.mode_y = 5;

    osd.mode.fs_x =  90;
    osd.mode.fs_y = 23;

    osd.mode.arm_x =  90;
    osd.mode.arm_y = 36;


    
    osd.mode.mix = 0;
    osd.mode.mode = 0;


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



}