#include "OSDeluxe.h"

struct bar b;
struct gps_widget_t g;
struct battery_widget_t bw;
struct status_widget_t status;
struct alt_widget_t aw;
struct vario_widget_t vw;
struct home_widget_t hw;
struct horizon_t hor;

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
    tw_osd_rectangle(b->x, b->y, b->w, b->h + 10, BACKROUND);
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

    disp_color_background = BACKROUND;
    if (b->val <= b->warn_red)
        disp_color = COLOR_RED | BLINK;
    else
        disp_color = COLOR_YELLOW;
    font_type = FONT_16x8;
    tw_printf(b->x + 2, b->y + b->h + 2, b->format, b->val);
}

void osd_gps_prerender(struct gps_widget_t *g) {}

void osd_gps_render(struct gps_widget_t *g)
{

    char q;

    char _buf[32];

    disp_color_background = BACKROUND;
    if (g->sat <= g->sat_warn || g->hdop < g->hdop_warn)
        disp_color = COLOR_RED | BLINK;
    else
        disp_color = g->color;

    tw_osd_rectangle(g->x, g->y, 22, 22, BACKROUND);

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
    tw_printf(g->x + 11, g->y+2, "%u", g->sat);
    font_type = FONT_8x8;
    tw_printf(g->x + 11, g->y + 12, "%2.2f", g->hdop);
}

void osd_battery_prerender( struct battery_widget_t *bw)
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
bw->cap.min = 0;
if (bw->remaining_capacity >= 0)
    bw->cap.val = bw->max_capacity * ((float)bw->remaining_capacity / 100);
else 
    bw->cap.val = 0;
bw->cap.warn_yellow = bw->max_capacity * 0.2f;      //Yellow warning at 20%
bw->cap.warn_red = bw->max_capacity * 0.1f;         //Red warning at 10%
bw->cap.mix = bw->mix;
bw->cap.bar_type = bw->bar_type;
bw->cap.format = sCapFormat;

}

void osd_battery_render( struct battery_widget_t *bw)
{

  osd_bar_render( &bw->volt);
  osd_bar_render( &bw->cap);
  tw_osd_rectangle(bw->x, bw->y+40,30,10,BACKROUND);
  disp_color = COLOR_YELLOW;
  tw_printf(bw->x + 2, bw->y+42,"%5.1f\x84\x85", bw->current);
}


void osd_status_prerender( struct status_widget_t *s) {}

void osd_status_render( struct status_widget_t *s)
{

#define BAR_W 7
#define BAR_H 16

unsigned char bar_color;
unsigned char mix;

  if (s->mix)
        mix = MIX;
    else
        mix = 0;

 tw_osd_rectangle(s->x,s->y, 3*BAR_W + 8 , BAR_H, BACKROUND); //background
 //tw_osd_rectangle(s->x+1, s->y+1, 3*BAR_W + 6,BAR_H-2, COLOR_WHITE | mix); //White border

 font_type = FONT_SHADOW_8x8;

 // Display notification bars
 switch (s->gps_status)
 {
 case STATUS_OK:
     bar_color = COLOR_GREEN | mix;
     disp_color = COLOR_75_WHITE | mix;
     disp_color_background = COLOR_GREEN | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_WARNING:
     bar_color = COLOR_DARK_YELLOW | mix;
     disp_color = COLOR_75_WHITE | mix;
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
     disp_color = COLOR_75_WHITE | mix;
     disp_color_background = COLOR_GREEN | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_WARNING:
     bar_color = COLOR_DARK_YELLOW | mix;
     disp_color = COLOR_75_WHITE | mix;
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
     disp_color = COLOR_75_WHITE | mix;
     disp_color_background = COLOR_GREEN | mix;
     disp_color_shadow = COLOR_BLACK | mix;
     break;
 case STATUS_WARNING:
     bar_color = COLOR_DARK_YELLOW | mix;
     disp_color = COLOR_75_WHITE | mix;
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

void osd_altitude_prerender( struct alt_widget_t *aw){}

void osd_altitude_render( struct alt_widget_t *aw)
{

unsigned char mix;

  if (aw->mix)
        mix = MIX;
    else
        mix = 0;

 font_type = FONT_16x8;
 disp_color = COLOR_YELLOW | mix;
 disp_color_background = BACKROUND;
 disp_color_shadow = COLOR_BLACK | mix;

 tw_osd_rectangle(aw->x, aw->y, 29,10, BACKROUND);
 tw_printf(aw->x+1, aw->y+2, "\x86\x87%4dm", (int)aw->altitude);

} 

void osd_vario_prerender(struct vario_widget_t *vw){}

void osd_vario_render(struct vario_widget_t *vw)
{

    unsigned char mix;
    float val, f, fh;

    if (vw->vario > vw->vario_max) val = vw->vario_max;
    if (vw->vario < -vw->vario_max) val = -vw->vario_max;

    if (vw->mix)
        mix = MIX;
    else
        mix = 0;

    tw_osd_rectangle(vw->x, vw->y, vw->w, vw->h, COLOR_WHITE | mix);
    tw_osd_rectangle(vw->x + 1, vw->y + 1, vw->w - 2, vw->h - 2, BACKROUND);

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
    disp_color_background = BACKROUND;
    disp_color = COLOR_YELLOW;
    
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


void osd_home_prerender(struct home_widget_t *hw){}

void osd_home_render(struct home_widget_t *hw)
{

    struct polygon home_arrow;
    struct point home_arrow_points[5] = { { 0, -10 }, { +10, +10 }, { 0, -2 }, { -10, +10 } };
    home_arrow.len = 4;
    home_arrow.points = home_arrow_points;

    transform_polygon(&home_arrow, hw->x*SCREEN_SCALE , hw->y, hw->orientation);
    //tw_osd_rectangle(hw->x-10, hw->y-20 ,20 ,40 , COLOR_NONE);
    tw_set_osd_buf(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    draw_polygon(&home_arrow);

    font_type = FONT_16x8;
    disp_color = COLOR_YELLOW;
    disp_color_background = BACKROUND;
    disp_color_shadow = COLOR_BLACK;

    if (osd.home.lock == HOME_LOCKED)
        tw_printf(hw->x-12, hw->y+21,"% 4u m", hw->home_distance);
    else
        tw_printf(hw->x-12, hw->y+21,"no home");

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

#define RANGE 120
#define SCALE 3
#define MINOR_TICK  5
#define MAJOR_TICK  10

void render_horizon(struct horizon_t *h)
{
    int y, i, j;
    int x0, x1, y0, y1;
    unsigned char size, gap;
    char buf[10];
    float offset;
    float cx, cy;
    float pitchrad, rollrad;
    float cos_roll, sin_roll;

    FontType ft_temp;

    unsigned char c1, c2, c3, c4;

    pitchrad = DEG2RAD(h->pitch);
    rollrad  = DEG2RAD(h->roll);
    cos_roll = cos (rollrad);
    sin_roll = -1 * sin (rollrad);


    if ((abs(h->pitch) > 30) || (abs(h->roll) > 30))
    {
        c1 = COLOR_RED; c2= COLOR_YELLOW; c3= c1; c4= c2;
    }
    else{
        c1 = COLOR_WHITE; c2 = COLOR_BLACK; c3 = c1; c4 = c2;
    }


    //tw_osd_fill_region (50, 78, 130, 205, COLOR_NONE, OSD_work_field, OSD_PATH_DISP);
    //tw_osd_rectangle(38,60,100,160, COLOR_NONE);

    tw_set_osd_buf (c1, c2, c3, c4);
    //tw_set_osd_buf (COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);

    for (i = -RANGE / 2; i <= RANGE / 2; i++)
    {
        y = h->y - i;
        j = (h->pitch*SCALE) + i;

        if (j % (MINOR_TICK * SCALE) == 0)
        {
            if (j == 0)
            {
                size = 50; // Zero line
                gap = 8;
            }
            else
            {
                if (j % (MAJOR_TICK * SCALE) == 0)
                    size = 20; // tick
                else
                    size = 10; // small tick
                gap = 10;
            }

            cx = h->x + (i * sin_roll);
            cy = y + i - (i * cos_roll);

            offset = (gap * cos_roll);
            x0 = cx + offset;
            offset = (size * cos_roll);
            x1 = x0 + offset;

            offset = (gap * sin_roll);
            y0 = cy + offset;
            offset = (size * sin_roll);
            y1 = y0 + offset;

            if (size == 50)
            {
                tw_set_osd_buf(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
                tw_osd_drawline(x0, y0-1, x1, y1-1);
                tw_osd_drawline(x0, y0+1, x1, y1+1);
                tw_set_osd_buf(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
                tw_osd_drawline(x0, y0, x1, y1);
                tw_set_osd_buf (c1, c2, c3, c4);
            }
            else
            {
                tw_osd_drawline(x0, y0, x1, y1);
            }
            offset = (gap * cos_roll);
            x0 = cx - offset;
            offset = (size * cos_roll);
            x1 = x0 - offset;

            offset = (gap * sin_roll);
            y0 = cy - offset;
            offset = (size * sin_roll);
            y1 = y0 - offset;

            if (size == 50)
            {
                tw_set_osd_buf(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
                tw_osd_drawline(x0, y0-1, x1, y1-1);
                tw_osd_drawline(x0, y0+1, x1, y1+1);
                tw_set_osd_buf(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
                tw_osd_drawline(x0, y0, x1, y1);
                tw_set_osd_buf (c1, c2, c3, c4);
            }
            else
            {
                tw_osd_drawline(x0, y0, x1, y1);
            }
            if ((j != 0) && (j % (MAJOR_TICK * SCALE) == 0))
            {
                ft_temp = font_type;
                disp_color = c1;
                disp_color_background = COLOR_NONE;
                disp_color_shadow = COLOR_BLACK;
                font_type = FONT_OUTLINE_8x12;
                tw_printf ((cx-6)/SCREEN_SCALE, cy  - 6, "% 03d", j / SCALE);
                font_type = ft_temp;
                tw_set_osd_buf (c1, c2, c3, c4);
                //tw_set_osd_buf(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
                
            }
        }
    }
}


void osd_mode_render( struct mode_widget_t *mw)
{

char mode[17];
unsigned char mix;
unsigned char tmp_field;

  if (mw->mix)
        mix = MIX;
    else
        mix = 0;


    unsigned int cust_mode;

    cust_mode = mw->mode;
    if (osd.mav_type !=  MAV_TYPE_FIXED_WING)
        cust_mode += 100;

    switch (cust_mode) {
    case PLANE_MODE_MANUAL:
        strcpy(mode, "Manual");
        break;
    case PLANE_MODE_CIRCLE:
    case COPTER_MODE_CIRCLE:
        strcpy(mode, "Circle");
        break;
    case PLANE_MODE_STABILIZE:
    case COPTER_MODE_STABILIZE:
        strcpy(mode, "Stabilize");
        break;
    case PLANE_MODE_TRAINING:
        strcpy(mode, "Training");
        break;
    case PLANE_MODE_ACRO:
    case COPTER_MODE_ACRO:
        strcpy(mode, "Acro");
        break;
    case PLANE_MODE_FBWA:
        strcpy(mode, "Fly-By-Wire A");
        break;
    case PLANE_MODE_FBWB:
        strcpy(mode, "Fly-By-Wire B");
        break;
    case PLANE_MODE_CRUISE:
        strcpy(mode, "Cruise");
        break;
    case PLANE_MODE_AUTOTUNE:
    case COPTER_MODE_AUTOTUNE:
        strcpy(mode, "Auto tune");
        break;
    case PLANE_MODE_AUTO:
    case COPTER_MODE_AUTO:
        strcpy(mode, "Auto");
        break;
    case PLANE_MODE_RTL:
    case COPTER_MODE_RTL:
        strcpy(mode, "RTL");
        break;
    case PLANE_MODE_LOITER:
    case COPTER_MODE_LOITER:
        strcpy(mode, "Loiter");
        break;
    case PLANE_MODE_INIT:
        strcpy(mode, "Initializing");
        break;
    case PLANE_MODE_GUIDED:
    case COPTER_MODE_GUIDED:
        strcpy(mode, "Guided");
        break;
    case COPTER_MODE_ALTHOLD:
        strcpy(mode, "Altitude hold");
        break;
    case COPTER_MODE_LAND:
        strcpy(mode, "Land");
        break;
    case COPTER_MODE_OF_LOITER:
        strcpy(mode, "OF Loiter");
        break;
    case COPTER_MODE_DRIFT:
        strcpy(mode, "Drift");
        break;
    case COPTER_MODE_SPORT:
        strcpy(mode, "Sport");
        break;
    case COPTER_MODE_FLIP:
        strcpy(mode, "Flip");
        break;
    case COPTER_MODE_POSHOLD:
        strcpy(mode, "Position hold");
        break;
    default:
        strcpy(mode, "Unknown Mode");
        break;
}


 font_type = FONT_OUTLINE_16x12c;
 disp_color = COLOR_YELLOW | mix;
 disp_color_background = COLOR_NONE | MIX;
 disp_color_shadow = COLOR_BLACK | mix;
 int l = (float)strlen(mode) * 1.5f;

 tw_printf(mw->x-(l), mw->y, "%s", mode);

if (osd.system_status == MAV_STATE_CRITICAL)
{
 disp_color = COLOR_RED | BLINK;
 disp_color_background = COLOR_NONE;
 disp_color_shadow = COLOR_BLACK | BLINK;
 tw_printf(mw->x-(13), mw->y+13, "FAILSAFE!");
    
}

if (osd.displayed_arming_status != osd.arming_status)
{
    osd.displayed_arming_status = osd.arming_status;

    tmp_field = OSD_work_field;
    OSD_path = OSD_PATH_REC;
    OSD_work_field = FLD_EVEN;

    rec_color = COLOR_REC_RED;
    rec_color_background = COLOR_REC_NONE;
    rec_color_shadow = COLOR_REC_BLACK;
    tw_osd_rectangle(mw->x / 2 - 7, mw->y + 26, 20, 13, 0xff);

    if (osd.arming_status)
    {
        rec_color = COLOR_REC_RED | REC_BLINK;
        rec_color_shadow = COLOR_REC_BLACK | REC_BLINK;
        tw_printf(mw->x / 2 - 4, mw->y + 26, "ARMED");
        osd.armed_start_time = millis();
    }
    else
    {
        tw_printf(mw->x / 2 - 7, mw->y + 26, "Disarmed");
    }

    OSD_path = OSD_PATH_DISP;
    OSD_work_field = tmp_field;
}

if (osd.armed_start_time != 0)
{
    if (millis() > (osd.armed_start_time + 10000) )
    {
        tmp_field = OSD_work_field;
        OSD_path = OSD_PATH_REC;
        OSD_work_field = FLD_EVEN;
        tw_osd_rectangle(mw->x / 2 - 7, mw->y + 26, 20, 13, 0xff);
        OSD_path = OSD_PATH_DISP;
        OSD_work_field = tmp_field;
        osd.armed_start_time = 0;
    }
}
} 