#include <Arduino.h>
#include "tw_func.h"

#define X_SHIFT 32

#define X_SIZE  120
#define Y_SIZE  120

#define RANGE       Y_SIZE
#define SCALE       5
#define MINOR_TICK  5
#define MAJOR_TICK  10

#define Y_CENTER    144
#define X_CENTER    144

#define ROLL_RANGE  90
#define MINOR_ROLL_TICK  5
#define MAJOR_ROLL_TICK  15


widget_priv priv;

void render_horizon()
{
    int y, i, j;
    int x0, x1, y0, y1;
    unsigned char size, gap;
    char buf[10];
    float offset;
    float cx, cy;

    FontType ft_temp;

    tw_set_osd_buf (COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);

    for (i = -RANGE / 2; i <= RANGE / 2; i++)
    {
        y = Y_CENTER - i;
        j = priv.pitch_deg + i;

        if (j % (MINOR_TICK * SCALE) == 0)
        {
            if (j == 0)
            {
                size = 50; // Zero line
                gap = 10;
            }
            else
            {
                if (j % (MAJOR_TICK * SCALE) == 0)
                    size = 20; // tick
                else
                    size = 10; // small tick
                gap = 10;
            }

            cx = X_CENTER + (i * priv.sin_roll);
            cy = y + i - (i * priv.cos_roll);

            offset = (gap * priv.cos_roll);
            x0 = cx + offset;
            offset = (size * priv.cos_roll);
            x1 = x0 + offset;

            offset = (gap * priv.sin_roll);
            y0 = cy + offset;
            offset = (size * priv.sin_roll);
            y1 = y0 + offset;

            tw_osd_drawline(x0, y0, x1, y1);

            offset = (gap * priv.cos_roll);
            x0 = cx - offset;
            offset = (size * priv.cos_roll);
            x1 = x0 - offset;

            offset = (gap * priv.sin_roll);
            y0 = cy - offset;
            offset = (size * priv.sin_roll);
            y1 = y0 - offset;

            tw_osd_drawline(x0, y0, x1, y1);

            if ((j != 0) && (j % (MAJOR_TICK * SCALE) == 0))
            {
                ft_temp = font_type;
                font_type = FONT_8x8;
                tw_printf ((cx-1)/SCREEN_SCALE, cy  - 4, "%d", j / SCALE);
                font_type = ft_temp;
                tw_set_osd_buf (COLOR_WHITE, COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
            }
        }
    }

    tw_osd_drawline (X_CENTER - 3, Y_CENTER, X_CENTER + 4, Y_CENTER);
    tw_osd_drawline (X_CENTER, Y_CENTER - 3, X_CENTER, Y_CENTER + 3);
}


void render_roll_indicator()
{

    int y, i, j;
    int x0, x1, y0, y1;
    unsigned char size, gap;
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

