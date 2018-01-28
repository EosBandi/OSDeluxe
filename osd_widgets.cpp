#include "OSDeluxe.h"

struct bar b;
struct gps_widget g;
struct battery_widget bw;
struct status_widget status;
struct alt_widget aw;
struct vario_widget vw;
struct home_widget hw;

char sVoltFormat[] = "%5.1f\x82\x83";
char sCapFormat[] =  "%5.0f\x80\x81";



void osd_bar_prerender(struct bar *b)
{
    //Nothing ?
}

void osd_bar_render(struct bar *b)
{

    float val;      //value to display, constrained to max and min
    
    unsigned char mix; // Transparent background value
    int iw, rw, yw, tw;


    val = b->val;
    if (b->val > b->max) val = b->max;
    if (b->val < b->min) val = b->min;

    iw = b->w - 4;              //internal width in pixels(quartets)
    rw = (float)iw * ((b->warn_red - b->min) / (b->max - b->min));
    yw = (float)iw * ((b->warn_yellow - b->min) / (b->max - b->min));
    tw = (float)iw * ((val - b->min) / (b->max - b->min));

    if (b->mix)
        mix = MIX;
    else
        mix = 0;


    OSD_path = OSD_PATH_DISP;
    //Outside
    tw_osd_rectangle(b->x, b->y, b->w, b->h + 10, COLOR_BLACK | MIX);
    tw_osd_rectangle(b->x + 1, b->y + 1, b->w - 2, b->h - 2, COLOR_WHITE | mix);

    if (b->bar_type == BAR_MULTICOLOR)
    {
        // Colored markers;
        tw_osd_rectangle(b->x + 2, b->y + 2, iw, b->h - 4, COLOR_GREEN | mix);
        tw_osd_rectangle(b->x + 2, b->y + 2, yw, b->h - 4, COLOR_YELLOW | mix);
        tw_osd_rectangle(b->x + 2, b->y + 2, rw, b->h - 4, COLOR_RED | mix);
        if (tw > 0) tw_osd_rectangle(b->x + 2 + tw, b->y + 2, iw - tw, b->h - 4, COLOR_BLACK | mix);
    }
    else
    {
        unsigned char bar_color = COLOR_GREEN;
        if (val <= b->warn_yellow) bar_color = COLOR_YELLOW | mix;
        if (val <= b->warn_red) bar_color = COLOR_RED | mix | BLINK;
        tw_osd_rectangle(b->x + 2, b->y + 2, b->w - 4, b->h - 4, COLOR_BLACK | mix);
        tw_osd_rectangle(b->x + 2, b->y + 2, tw, b->h - 4, bar_color | mix);
    }

    disp_color_background = COLOR_BLACK | MIX;
    if (b->val <= b->warn_red)
        disp_color = COLOR_RED | BLINK;
    else
        disp_color = COLOR_YELLOW;
    font_type = FONT_16x8;
    tw_printf(b->x + 2, b->y + b->h + 2, b->format, b->val);
}

void osd_gps_prerender(struct gps_widget *g) {}

void osd_gps_render(struct gps_widget *g)
{

    char q;

    char _buf[32];

    disp_color_background = COLOR_BLACK | MIX;
    if (g->sat <= g->sat_warn || g->hdop < g->hdop_warn)
        disp_color = COLOR_RED | BLINK;
    else
        disp_color = g->color;

    tw_osd_rectangle(g->x, g->y, 22, 22, COLOR_BLACK | MIX);

    for (int y = 0; y < 32; y = y + 2)
    {
        for (int x = 0; x < 32; x++)
        {
            q = gps_image[y * 32 + x];
            switch (q)
            {
            case 0xdd:
                q = disp_color_background;
                break;
            case 0xff:
                q = disp_color;
                break;
            case 0x00:
                q = disp_color_shadow;
                break;
            }
            _buf[x] = q;
        }
        tw_wr_osd_buffer(&_buf[0], 0);
        tw_wr_osd_buffer(&_buf[4], 1);
        tw_wr_osd_buffer(&_buf[8], 2);
        tw_wr_osd_buffer(&_buf[12], 3);
        tw_wr_osd_buffer(&_buf[16], 4);
        tw_wr_osd_buffer(&_buf[20], 5);
        tw_wr_osd_buffer(&_buf[24], 6);
        tw_wr_osd_buffer(&_buf[28], 7);
        tw_wr_display_from_buffer(g->x+1, g->y +2 + y / 2, 7);
    }

    font_type = FONT_16x8;
    tw_printf(g->x + 9, g->y+2, "%u", g->sat);
    font_type = FONT_8x8;
    tw_printf(g->x + 9, g->y + 12, "%2.2f", g->hdop);
}

void osd_battery_prerender( struct battery_widget *bw)
{

bw->volt.x = bw->x;
bw->volt.y = bw->y;
bw->volt.w = 30;
bw->volt.h = 10;

bw->volt.max = bw->max_cell_voltage * bw->cells;
bw->volt.min = bw->min_cell_voltage * bw->cells;
bw->volt.val = bw->voltage;
bw->volt.warn_red = bw->red_cell_voltage * bw->cells;
bw->volt.warn_yellow = bw->yellow_cell_voltage * bw->cells;
bw->volt.mix = bw->mix;
bw->volt.bar_type = bw->bar_type;
bw->volt.format = sVoltFormat;

bw->cap.x = bw->x;
bw->cap.y = bw->y + 20;
bw->cap.w = 30;
bw->cap.h = 10;

bw->cap.max = bw->max_capacity;
bw->cap.min = 500;
bw->cap.val = bw->remaining_capacity;
bw->cap.warn_yellow = bw->max_capacity * 0.2f;      //Yellow warning at 20%
bw->cap.warn_red = bw->max_capacity * 0.1f;         //Red warning at 10%
bw->cap.mix = bw->mix;
bw->cap.bar_type = bw->bar_type;
bw->cap.format = sCapFormat;

}

void osd_battery_render( struct battery_widget *bw)
{

  osd_bar_render( &bw->volt);
  osd_bar_render( &bw->cap);
  tw_osd_rectangle(bw->x, bw->y+40,30,10,COLOR_BLACK|MIX);
  disp_color = COLOR_YELLOW;
  tw_printf(bw->x + 2, bw->y+42,"%5.1f\x84\x85", &bw->current);
}


void osd_status_prerender( struct status_widget *s) {}

void osd_status_render( struct status_widget *s)
{

#define BAR_W 7
#define BAR_H 16

unsigned char bar_color;
unsigned char mix;

  if (s->mix)
        mix = MIX;
    else
        mix = 0;

 tw_osd_rectangle(s->x,s->y, 3*BAR_W + 8 , BAR_H, COLOR_BLACK| MIX); //background
 tw_osd_rectangle(s->x+1, s->y+1, 3*BAR_W + 6,BAR_H-2, COLOR_WHITE | mix); //White border

 font_type = FONT_SHADOW_8x8;

 // Display notification bars
 switch (s->gps_status)
 {
 case STATUS_OK:
     bar_color = COLOR_GREEN | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_GREEN | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_WARNING:
     bar_color = COLOR_DARK_YELLOW | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_DARK_YELLOW | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_CRITICAL:
     bar_color = COLOR_RED | mix | BLINK;
     disp_color = COLOR_YELLOW | mix;
     disp_color_background = COLOR_RED | mix | BLINK;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_NONE:
     bar_color = COLOR_BLACK | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_BLACK | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 }
 tw_osd_rectangle(s->x + 2, s->y + 2, BAR_W, BAR_H - 4, bar_color);
 tw_printf(s->x + 2 + (BAR_W / 2) - 2, s->y + 5, "GPS");

 switch (s->ekf_status)
 {
 case STATUS_OK:
     bar_color = COLOR_GREEN | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_GREEN | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_WARNING:
     bar_color = COLOR_DARK_YELLOW | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_DARK_YELLOW | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_CRITICAL:
     bar_color = COLOR_RED | mix | BLINK;
     disp_color = COLOR_YELLOW | mix;
     disp_color_background = COLOR_RED | mix | BLINK;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_NONE:
     bar_color = COLOR_BLACK | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_BLACK | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 }

 tw_osd_rectangle(s->x+2 + BAR_W + 2, s->y+2, BAR_W,BAR_H-4, bar_color);
 tw_printf(s->x+2 + BAR_W + 2+ (BAR_W/2)-2, s->y+5,"EKF");

switch (s->vibe_status)
 {
 case STATUS_OK:
     bar_color = COLOR_GREEN | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_GREEN | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_WARNING:
     bar_color = COLOR_DARK_YELLOW | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_DARK_YELLOW | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_CRITICAL:
     bar_color = COLOR_RED | mix | BLINK;
     disp_color = COLOR_YELLOW | mix;
     disp_color_background = COLOR_RED | mix | BLINK;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_NONE:
     bar_color = COLOR_BLACK | mix;
     disp_color = COLOR_WHITE | mix;
     disp_color_background = COLOR_BLACK | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 }
 tw_osd_rectangle(s->x+2 + BAR_W + 2 + BAR_W + 2, s->y+2, BAR_W,BAR_H-4, bar_color);
 tw_printf(s->x+6 + 2*BAR_W+ (BAR_W/2)-2, s->y+5,"VIB");
 
}

void osd_altitude_prerender( struct alt_widget *aw){}

void osd_altitude_render( struct alt_widget *aw)
{

unsigned char mix;

  if (aw->mix)
        mix = MIX;
    else
        mix = 0;

 font_type = FONT_16x8;
 disp_color = COLOR_YELLOW | mix;
 disp_color_background = COLOR_BLACK | MIX;
 disp_color_shadow = COLOR_BLACK | mix;

 tw_osd_rectangle(aw->x, aw->y, 29,10, COLOR_BLACK|MIX);
 tw_printf(aw->x+1, aw->y+2, "\x86\x87%4um", (int)aw->altitude);

} 

void osd_vario_prerender(struct vario_widget *vw){}

void osd_vario_render(struct vario_widget *vw)
{

    unsigned char mix;
    float val, f, fh;

    if (vw->vario > vw->vario_max) val = vw->vario_max;
    if (vw->vario < -vw->vario_max) val = -vw->vario_max;

    if (vw->mix)
        mix = MIX;
    else
        mix = 0;

    // tw_osd_rectangle(vw->x, vw->y, vw->w, vw->h, COLOR_WHITE | mix);
    tw_osd_rectangle(vw->x + 1, vw->y + 1, vw->w - 2, vw->h - 1, COLOR_BLACK | mix);

    f = vw->h / 2;
    fh = abs(f * vw->vario / vw->vario_max);

    if (vw->vario >= 0)
    {
        tw_osd_rectangle(vw->x + 1, vw->y + f - fh, vw->w - 2, fh, COLOR_GREEN);
    }
    else
    {
        tw_osd_rectangle(vw->x + 1, vw->y + f, vw->w - 2, fh, COLOR_RED);
    }
    font_type = FONT_8x8;

    switch (vw->num_pos)
    {
    case POS_RIGHT:
        tw_printf(vw->x + vw->w + 1, vw->y + f - 4, "% 5.1fm/s", vw->vario);
        break;
    case POS_LEFT:
        tw_printf(vw->x - 16, vw->y + f - 4, "% 5.1fm/s", vw->vario);
        break;
    case POS_ABOVE:
        tw_printf(vw->x - 5, vw->y -8  , "% 5.1fm/s", vw->vario);
        break;
    case POS_BELOW:
        tw_printf(vw->x - 5, vw->y+vw->h +2  , "% 5.1fm/s", vw->vario);
        break;
    }
}


void osd_home_prerender(struct home_widget *hw){}

void osd_home_render(struct home_widget *hw)
{

    struct polygon home_arrow;
    struct point home_arrow_points[5] = { { 0, -10 }, { +10, +10 }, { 0, -2 }, { -10, +10 } };
    home_arrow.len = 4;
    home_arrow.points = home_arrow_points;

    transform_polygon(&home_arrow, hw->x*SCREEN_SCALE , hw->y, hw->orientation);
    tw_osd_rectangle(hw->x-10, hw->y-20 ,20 ,40 , COLOR_NONE);
    tw_set_osd_buf(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK, COLOR_WHITE);
    draw_polygon(&home_arrow);

    font_type = FONT_16x8;
    disp_color = COLOR_YELLOW;
    disp_color_background = COLOR_BLACK | MIX;
    disp_color_shadow = COLOR_BLACK;
    tw_printf(hw->x-12, hw->y+21,"% 4u m", hw->home_distance);

}

void osd_center_marker()
{
    unsigned char temp_path;
    unsigned char reg209;

    temp_path = OSD_path;
    OSD_path = OSD_PATH_REC;

    //tw_osd_rectangle(40,140,10,3,COLOR_REC_WHITE);

    tw_set_osd_buf(0xff,0x01,0x10,0xff);
    cnt = 0;
    data_buf[cnt++] = 45; // start HPOS
    data_buf[cnt++] = 45; // end   HPOS
    data_buf[cnt++] = 135; // start VPOS
    data_buf[cnt++] = 155; // end VPOS
    reg209 = 0x00;
    if (OSD_work_field == FLD_EVEN) reg209 = 0b00001000;
    data_buf[cnt++] = reg209;
    data_buf[cnt++] = 0b11100000;
    tw_write_buf(0x205, data_buf, cnt);
    tw_wait_for_osd_write(50);
    
    tw_set_osd_buf(0x00,0x00,0x00,0x00);
    cnt = 0;
    data_buf[cnt++] = 43; // start HPOS
    data_buf[cnt++] = 47; // end   HPOS
    data_buf[cnt++] = 144; // start VPOS
    data_buf[cnt++] = 146; // end VPOS
    reg209 = 0x00;
    if (OSD_work_field == FLD_EVEN) reg209 = 0b00001000;
    data_buf[cnt++] = reg209;
    data_buf[cnt++] = 0b11100000;
    tw_write_buf(0x205, data_buf, cnt);
    tw_wait_for_osd_write(50);

    tw_set_osd_buf(0x11,0x11,0x11,0x11);
    cnt = 0;
    data_buf[cnt++] = 43; // start HPOS
    data_buf[cnt++] = 47; // end   HPOS
    data_buf[cnt++] = 145; // start VPOS
    data_buf[cnt++] = 145; // end VPOS
    reg209 = 0x00;
    if (OSD_work_field == FLD_EVEN) reg209 = 0b00001000;
    data_buf[cnt++] = reg209;
    data_buf[cnt++] = 0b11100000;
    tw_write_buf(0x205, data_buf, cnt);
    tw_wait_for_osd_write(50);

    tw_osd_setpixel(45 * SCREEN_SCALE,144,0x00,0x01,0x10,0x00);
    tw_osd_setpixel(45 * SCREEN_SCALE,146,0x00,0x01,0x10,0x00);
    tw_osd_setpixel(45 * SCREEN_SCALE,145,0x11,0x11,0x11,0x11);
    
    OSD_path = temp_path;


}