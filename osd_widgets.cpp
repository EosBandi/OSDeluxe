#include "OSDeluxe.h"

struct bar b;
struct gps_widget g;
struct battery_widget bw;

void osd_bar_prerender(struct bar *b)
{
    //Nothing ?
}

void osd_bar_render(struct bar *b)
{

    float val;
    unsigned char mix;

    if (b->mix)
        mix = MIX;
    else
        mix = 0;
    val = b->val;
    if (b->val > b->max) val = b->max;
    if (b->val < b->min) val = b->min;

    OSD_path = OSD_PATH_DISP;
    tw_osd_rectangle(b->x, b->y, b->w, b->h + 10, COLOR_BLACK | MIX);
    tw_osd_rectangle(b->x + 1, b->y + 1, b->w - 2, b->h - 2, COLOR_WHITE | mix);
    int iw = b->w - 4;
    int rw = (float)iw * 0.2f;
    int yw = (float)iw * 0.2f;

    tw_osd_rectangle(b->x + 2, b->y + 2, b->w - 4, b->h - 4, COLOR_GREEN | mix);
    tw_osd_rectangle(b->x + 2, b->y + 2, rw, b->h - 4, COLOR_RED | mix);
    tw_osd_rectangle(b->x + 2 + rw, b->y + 2, yw, b->h - 4, COLOR_YELLOW | mix);

    float range = b->max - b->min;
    float tick = (b->w - 4) / range;
    float tw = (b->max - val) * tick;
    if (tw > 0) tw_osd_rectangle(b->x + 2 + b->w - 4 - tw, b->y + 2, tw + 1, b->h - 4, COLOR_BLACK | mix);

    disp_color_background = COLOR_BLACK | MIX;
    if (b->val <= b->warn)
        disp_color = COLOR_RED | BLINK;
    else
        disp_color = COLOR_YELLOW;
    font_type = FONT_16x8;
    tw_printf(b->x + 2, b->y + b->h + 2, "%2.1fV", b->val);
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

    tw_osd_rectangle(g->x - 1, g->y - 2, 20, 20, COLOR_BLACK | MIX);

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
        tw_wr_display_from_buffer(g->x, g->y + y / 2, 7);
    }

    font_type = FONT_16x8;
    tw_printf(g->x + 9, g->y, "%u", g->sat);
    font_type = FONT_8x8;
    tw_printf(g->x + 9, g->y + 10, "%2.2f", g->hdop);
}

void osd_battery_prerender( struct battery_widget *bw)
{

bw->volt.x = bw->x;
bw->volt.y = bw->y;
bw->volt.w = 30;
bw->volt.h = 10;

bw->volt.max = 25.2;
bw->volt.min = 20.0;
bw->volt.val = 22.5;
bw->volt.warn = 21.0;
bw->volt.mix = true;

bw->cap.x = bw->x;
bw->cap.y = bw->y + 20;
bw->cap.w = 30;
bw->cap.h = 10;

bw->cap.max = 12000;
bw->cap.min = 1000;
bw->cap.val = 8976;
bw->cap.warn = 3000;
bw->cap.mix = true;
 



}

void osd_battery_render( struct battery_widget *bw)
{

  osd_bar_render( &bw->volt);
  osd_bar_render( &bw->cap);


}