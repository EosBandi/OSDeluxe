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

    osd.bat.x = 143;
    osd.bat.y = 230;
    osd.bat.min_cell_voltage = 3.3;
    osd.bat.max_cell_voltage = 4.2;
    osd.bat.red_cell_voltage = 3.5;
    osd.bat.yellow_cell_voltage = 3.7;
    osd.bat.bar_type = BAR_SINGLE_COLOR;
    osd.bat.mix = 0;
    osd.bat.cells = 6;

    osd.bat.voltage = 23.4;
    osd.bat.current = 0.0;
    osd.bat.max_capacity = 12000;
    osd.bat.remaining_capacity = 7262;

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

    osd.vario.x = 160;
    osd.vario.y = 110;
    osd.vario.h = 100;
    osd.vario.w = 4;
    osd.vario.vario = 5.5f;
    osd.vario.vario_max = 10.0f;
    osd.vario.mix = 1;
    osd.vario.num_pos = POS_BELOW;

    osd.home.x = 90;
    osd.home.y = 245;
    osd.home.orientation = 0;
    osd.home.home_distance = 0;
}