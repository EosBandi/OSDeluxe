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

#include "OSDeluxe.h"

#define X_SHIFT 32

#define X_SIZE  120
#define Y_SIZE  120

#define Y_CENTER    144
#define X_CENTER    144

#define ROLL_RANGE  90
#define MINOR_ROLL_TICK  5
#define MAJOR_ROLL_TICK  15

#define SCALE 5
float pitchrad, rollrad;
  
 #define X_POS 40
 #define Y_POS 100
 #define Y_HEIGHT 15
 #define X_WIDTH  20

/*


void render_roll_indicator()
{

    int y, i, j;
    int x0, x1, y0, y1;
    U8 size, gap;
    char buf[10];
    float offset;
    float cx, cy;
    FontType ft_temp;

    //Roll indicator scale  
    float cos_i, sin_i;
    
    tw_set_osd_buf (COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
    for (i = -ROLL_RANGE/2; i <= ROLL_RANGE/2; i++) {
     
        y = Y_CENTER - i;

        //j = priv.pitch_deg*SCALE + i;
        //j = priv.roll_deg + i;

        if (i % (MINOR_ROLL_TICK) == 0) {
            gap = 50;
            if (i == 0) {
                size = 6;
            } else {
                if (i % (MAJOR_ROLL_TICK) == 0)
                    size = 10;
                else
                    size = 3;
            }

            cos_i = cos(DEG2RAD(i));
            sin_i = sin(DEG2RAD(i));
            
            cx = X_CENTER;// + (int) (gap * cos_i);
            cy = Y_CENTER;// - (int) (gap * sin_i);


            offset = (int) gap * cos_i;
            x0 = cx + offset;
            offset = (int) size * cos_i;
            x1 = x0 + offset;


            offset = (int) gap * sin_i;
            y0 = cy - offset;
            offset = (int) size * sin_i;
            y1 = y0 - offset;

            tw_osd_drawline(x0, y0, x1, y1);

            if ((i != 0) && (i % (MAJOR_ROLL_TICK) == 0)) {
                ft_temp = font_type;
                font_type = FONT_8x8;
                tw_printf(x1+1,y1/SCREEN_SCALE-2,"%d",i);
                font_type = ft_temp;
                tw_set_osd_buf (COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);

            }

        }
    }

   //Roll indicator line

    cx = X_CENTER; // + (int) (gap * priv.sin_roll);
    cy = Y_CENTER ; //- (int) (gap * priv.cos_roll);
    size = 10;

    offset = (int) (gap-size) * priv.cos_roll;
    x0 = cx + offset;
    offset = (int) size * priv.cos_roll;
    x1 = x0 + offset;

    offset = (int) (gap-size) * priv.sin_roll;
    y0 = cy + offset;
    offset = (int) size * priv.sin_roll;
    y1 = y0 + offset;

    tw_osd_drawline(x0, y0, x1, y1);

}

*/