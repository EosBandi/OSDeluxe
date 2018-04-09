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

char sVoltFormat[] = "%5.2fV";
char sCapFormat[] =  "%5.0f\x5b";


void osd_bar_render(struct bar *b)
{

    float val;      //value to display, constrained to max and min
    int iw, rw, yw, tw;
	char color, mix;


	if (b->mix) mix = MIX;

    val = b->val;
    if (b->val > b->max) val = b->max;
    if (b->val < b->min) val = b->min;

    iw = b->w - 4;              //internal width in pixels(quartets)
    rw = (float)iw * ((b->warn_red - b->min) / (b->max - b->min));
    yw = (float)iw * ((b->warn_yellow - b->min) / (b->max - b->min));
    tw = (float)iw * ((val - b->min) / (b->max - b->min));


    //Outside
	OSD256_box(PTH_X, b->x, b->y, b->w, b->h, COLOR_WHITE | mix);

    if (b->bar_type == BAR_MULTICOLOR)
    {
        // Colored markers;
		OSD256_box(PTH_X, b->x + 2, b->y + 2, iw, b->h - 4, COLOR_GREEN | mix);
		OSD256_box(PTH_X, b->x + 2, b->y + 2, yw, b->h - 4, COLOR_YELLOW | mix);
		OSD256_box(PTH_X, b->x + 2, b->y + 2, rw, b->h - 4, COLOR_RED | mix);
        if (tw > 0) OSD256_box(PTH_X, b->x + 2 + tw, b->y + 2, iw - tw, b->h - 4, COLOR_BLACK | mix);
    }
    else
    {
        char bar_color = COLOR_GREEN;

        if (val <= b->warn_yellow) bar_color = COLOR_YELLOW | mix;
        if (val <= b->warn_red) bar_color = COLOR_RED | mix | BLINK;
		OSD256_box(PTH_X, b->x + 2, b->y + 2, b->w - 4, b->h - 4, COLOR_BLACK | mix);
		OSD256_box(PTH_X, b->x + 2, b->y + 2, tw, b->h - 4, bar_color | mix);
    }

    if (b->val <= b->warn_red)
        color = OSD256_FONT_RED_BLINK;
    else
        color = OSD256_FONT_YELLOW;
    OSD256_printf(b->x-2, b->y + b->h + 2, color, 0, b->format, b->val);
}

void osd_gps_render(struct gps_widget_t *g)
{

	char color;


	if (g->sat <= g->sat_critical || g->hdop > g->hdop_critical)
	{
		OSD256_display_bitmap(BMP_GPS_ICON_RED, g->x, g->y);
		color = OSD256_FONT_RED;
	}
	else
	{
		OSD256_display_bitmap(BMP_GPS_ICON_GREEN, g->x, g->y);
		color = OSD256_FONT_YELLOW;

	}

    OSD256_printf(g->x + 42, g->y + 2,color,0, "%u", g->sat);
    OSD256_printf(g->x + 42, g->y + 22,color,0, "%2.2f", g->hdop);
}

void osd_batt_volt_render(struct batt_volt_widget_t *bw)
{

	bw->volt.x = bw->x;
	bw->volt.y = bw->y;
	bw->volt.w = 96;
	bw->volt.h = 20;

	bw->volt.max = bw->max_cell_voltage * bw->cells;
	bw->volt.min = bw->min_cell_voltage * bw->cells;
	bw->volt.val = bw->voltage;
	bw->volt.warn_red = bw->red_cell_voltage * bw->cells;
	bw->volt.warn_yellow = bw->yellow_cell_voltage * bw->cells;
	bw->volt.mix = bw->mix;
	bw->volt.bar_type = bw->bar_type;
	bw->volt.format = sVoltFormat;
	bw->volt.box = bw->box;

	osd_bar_render(&bw->volt);

}

void osd_batt_cap_render(struct batt_cap_widget_t *bw)
{


	bw->cap.x = bw->x;
	bw->cap.y = bw->y;
	bw->cap.w = 96;
	bw->cap.h = 20;

	bw->cap.max = bw->max_capacity;
	//debug("remaining:%u\n", bw->remaining_capacity);
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
	bw->cap.box = bw->box;

	osd_bar_render(&bw->cap);

}

void osd_batt_curr_render(struct batt_curr_widget_t *bw)
{
	OSD256_printf(bw->x, bw->y, OSD256_FONT_YELLOW,0,"%5.1fA", bw->current);
}

void osd_status_render( struct status_widget_t *s)
{
//debug("%u - %u - %u - %u - %u\n", s->x, s->y, s->vibe_status, s->visible, osd.visible_osd_page);




// Display notification bars
 switch (s->gps_status)
 {
 case STATUS_OK:
	 OSD256_display_bitmap(BMP_GPS_STATE_GREEN, s->x, s->y);
     break;
 case STATUS_WARNING:
	 OSD256_display_bitmap(BMP_GPS_STATE_YELLOW, s->x, s->y);
	 break;
 case STATUS_CRITICAL:
	 OSD256_display_bitmap(BMP_GPS_STATE_RED, s->x, s->y);
	 break;
 case STATUS_NONE:
	 OSD256_display_bitmap(BMP_GPS_STATE_GRAY, s->x, s->y);
	 break;
 }

 switch (s->ekf_status)
 {
 case STATUS_OK:
	 OSD256_display_bitmap(BMP_EKF_STATE_GREEN, s->x + 34, s->y);
     break;
 case STATUS_WARNING:
	 OSD256_display_bitmap(BMP_EKF_STATE_YELLOW, s->x + 34, s->y);
	 break;
 case STATUS_CRITICAL:
	 OSD256_display_bitmap(BMP_EKF_STATE_RED, s->x + 34, s->y);
	 break;
 case STATUS_NONE:
	 OSD256_display_bitmap(BMP_EKF_STATE_GRAY, s->x + 34, s->y);
	 break;
 }

switch (s->vibe_status)
 {
 case STATUS_OK:
	 OSD256_display_bitmap(BMP_VIB_STATE_GREEN, s->x + 68, s->y);
	 break;
 case STATUS_WARNING:
	 OSD256_display_bitmap(BMP_VIB_STATE_YELLOW, s->x + 68, s->y);
	 break;
 case STATUS_CRITICAL:
	 OSD256_display_bitmap(BMP_VIB_STATE_RED, s->x + 68, s->y);
	 break;
 case STATUS_NONE:
	 OSD256_display_bitmap(BMP_VIB_STATE_GRAY, s->x + 68, s->y);
	 break;
 }
 
}

void osd_pull_render(struct pull_widget_t *pw)
{
	char color;

	if (pw->pull >= pw->warning) color = OSD256_FONT_RED;
	else color = OSD256_FONT_YELLOW;

	OSD256_printf(pw->x, pw->y,color, 0, "%2.2f N", pw->pull);

}

void osd_altitude_render( struct alt_widget_t *aw)
{
 OSD256_printf(aw->x, aw->y,OSD256_FONT_YELLOW,0, "\x5c\x5d%dm", (int)aw->altitude);
} 

void osd_groundspeed_render(struct gs_widget_t *gs)
{
	OSD256_printf(gs->x, gs->y, OSD256_FONT_YELLOW, 0, "\x5f %.0f\x7b\x7c", osd.groundspeed * 3.6);
}

void osd_throttle_render(struct throttle_widget_t *t)
{
	OSD256_printf(t->x, t->y, OSD256_FONT_YELLOW, 0, "Th %u", osd.rcin[3]);
}


void osd_vario_render(struct vario_widget_t *vw)
{

    unsigned char mix;
    float val, f, fh;

    val = vw->vario;
    if (val > vw->vario_max) val = vw->vario_max;
    if (val < -vw->vario_max) val = -vw->vario_max;

    if (vw->mix)
        mix = MIX;
    else
        mix = 0;

    OSD256_box(PTH_X,vw->x, vw->y, vw->w, vw->h, COLOR_WHITE | mix);
	OSD256_box(PTH_X, vw->x + 1, vw->y + 1, vw->w - 2, vw->h - 2, BACKROUND);

    f = vw->h / 2;
    fh = abs(f * val / vw->vario_max);

    if (vw->vario >= 0)
    {
		OSD256_box(PTH_X, vw->x + 1, vw->y + f - fh, vw->w - 2, fh, COLOR_GREEN);
    }
    else
    {
		OSD256_box(PTH_X, vw->x + 1, vw->y + f, vw->w - 2, fh, COLOR_RED);
    }
    switch (vw->num_pos)
    {
    case POS_RIGHT:
        OSD256_printf(vw->x + vw->w + 1, vw->y + f - 4,OSD256_FONT_YELLOW,1 ,"% 4.1fm/s", vw->vario);
        break;
    case POS_LEFT:
		OSD256_printf(vw->x - 16, vw->y + f - 4,OSD256_FONT_YELLOW,1, "% 4.1fm/s", vw->vario);
        break;
    case POS_ABOVE:
        OSD256_printf(vw->x - 20, vw->y - 25  , OSD256_FONT_YELLOW, 1, "% 4.1", vw->vario);
        break;
    case POS_BELOW:
        OSD256_printf(vw->x - 20, vw->y+vw->h +2  , OSD256_FONT_YELLOW, 1, "% 4.1f", vw->vario);
        break;
    }
}

void osd_home_render(struct home_widget_t *hw)
{

    struct polygon home_arrow;
    struct point home_arrow_points[5] = { { 0, -20 }, { +20, +20 }, { 0, -2 }, { -20, +20 } };
    home_arrow.len = 4;

    home_arrow.points = home_arrow_points;
    transform_polygon(&home_arrow, hw->x , hw->y, hw->orientation);
    draw_polygon(&home_arrow, COLOR_WHITE);


    if (osd.home.lock == HOME_LOCKED)
        OSD256_printf(hw->x-56, hw->y+35, OSD256_FONT_YELLOW, 0 , "% 4u m", hw->home_distance);
    else
        OSD256_printf(hw->x-56, hw->y+35, OSD256_FONT_YELLOW, 0,"no home");

}


void osd_center_marker()
{

	OSD256_box(PTH_X, 340, 285, 40, 7, COLOR_BLACK | MIX);
	OSD256_box(PTH_X, 340, 287, 40, 3, COLOR_WHITE | MIX);

	OSD256_box(PTH_X, 358, 268, 5, 40, COLOR_BLACK | MIX);
	OSD256_box(PTH_X, 359, 268, 3, 40, COLOR_WHITE | MIX);

}

#define RANGE 250
#define SCALE 6
#define MINOR_TICK  5
#define MAJOR_TICK  10
#define ZERO_LINE 120
#define MAJOR_LINE  50
#define MINOR_LINE  8


void render_horizon(struct horizon_t *h)
{
    int y, i, j;
    int x0, x1, y0, y1;
    int  size, gap;
    float offset;
    float cx, cy;
    float pitchrad, rollrad;
    float cos_roll, sin_roll;

    char mix = MIX;

    FontType ft_temp;

    unsigned char c1, c2, c3, c4;

    pitchrad = DEG2RAD(h->pitch);
    rollrad  = DEG2RAD(h->roll);
    cos_roll = cos (rollrad);
    sin_roll = -1 * sin (rollrad);


    if ((abs(h->pitch) > 30) || (abs(h->roll) > 30))
    {
		c1 = COLOR_ORANGE;
    }
    else{
		c1 = COLOR_WHITE;
    }


    for (i = -RANGE / 2; i <= RANGE / 2; i++)
    {
        y = h->y - i;
        j = (h->pitch*SCALE) + i;

        if (j % (MINOR_TICK * SCALE) == 0)
        {
            if (j == 0)
            {
                size = ZERO_LINE; // Zero line
                gap = 8;
            }
            else
            {
                if (j % (MAJOR_TICK * SCALE) == 0)
                    size = MAJOR_LINE; // tick
                else
                    size = MINOR_LINE; // small tick
                gap = 20;
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
			if (size == ZERO_LINE) 
			{
				OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);
				OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0-2, x1, y1-2);
				OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0+2, x1, y1+2);
			}
			else
			{
				OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);
			}
            offset = (gap * cos_roll);
            x0 = cx - offset;
            offset = (size * cos_roll);
            x1 = x0 - offset;

            offset = (gap * sin_roll);
            y0 = cy - offset;
            offset = (size * sin_roll);
            y1 = y0 - offset;
			if (size == ZERO_LINE)
			{
				OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);
				OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0+2, x1, y1+2);
				OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0-2, x1, y1-2);
			}
			else
			{
				OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);

			}
 
				if ((j != 0) && (j % (MAJOR_TICK * SCALE) == 0))
            {
                OSD256_printf((cx-20), (cy-15), OSD256_FONT_WHITE, 1, "% 03d", j / SCALE);
                
            }
        }
    }
}

void osd_mode_render(struct mode_widget_t *mw)
{

	char mode[17];
	unsigned char mix = 0;
	unsigned char tmp_field;
	unsigned int cust_mode;

	if (mw->mix) mix = MIX;

	cust_mode = mw->mode;

	if (osd.mav_type != MAV_TYPE_FIXED_WING) cust_mode += 100;

	switch (cust_mode)
	{
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
	if (mw->mode_centered)
	{
		int l = (float)strlen(mode) * 15;
		OSD256_printf(mw->mode_x - (l / 2), mw->mode_y, OSD256_FONT_YELLOW, 0, "%s", mode);
	}
	else OSD256_printf(mw->mode_x, mw->mode_y, OSD256_FONT_YELLOW, 0, "%s", mode);


	if (osd.system_status == MAV_STATE_CRITICAL)
	{
		if (mw->fs_centered) OSD256_printf(mw->fs_x-67, mw->fs_y, OSD256_FONT_RED_BLINK, 0, "FAILSAFE!");
		else OSD256_printf(mw->fs_x, mw->fs_y, OSD256_FONT_RED_BLINK, 0, "FAILSAFE!");
	}


	if (osd.arming_status == 0)
	{
		if (mw->arm_centered) OSD256_printf(mw->arm_x - 60 , mw->arm_y, OSD256_FONT_RED, 0, "Disarmed");
		else OSD256_printf(mw->arm_x, mw->arm_y, OSD256_FONT_RED, 0, "Disarmed");

		osd.armed_start_time == 0;
	}
	else  //Arming status 1 ARMED
	{
		if (osd.armed_start_time == 0)
		{
			osd.armed_start_time = millis();
			osd.home.lock = HOME_WAIT;
			mavlink_seen[MAVLINK_MSG_ID_MISSION_ITEM] = 0xffff;
		}
		else if (millis() < (osd.armed_start_time + 8000))
		{
			if (mw->arm_centered) OSD256_printf(mw->arm_x - 37, mw->arm_y, OSD256_FONT_RED_BLINK, 0, "ARMED");
			else OSD256_printf(mw->arm_x, mw->arm_y, OSD256_FONT_RED_BLINK, 0, "ARMED");
		}
	}

}

void rc_control()
{



}



// line 0 is a display line
void message_buffer_add_line(char *message, char severity)
{
	// Check if we are standing at the last line of the buffer.
	if (osd.message_buffer_line == MESSAGE_BUFFER_LINES - 1)
	{
		for (int i = 2; i < MESSAGE_BUFFER_LINES; i++)
		{
			strcpy(osd.message_buffer[i - 1], osd.message_buffer[i]); // roll to 1
			osd.message_severity[i - 1] = osd.message_severity[i];
		}
	}
	else
	{
		osd.message_buffer_line++;
	}

	strcpy(osd.message_buffer[osd.message_buffer_line], message);
	osd.message_severity[osd.message_buffer_line] = severity;

	//Add line to the archive as well, so we can always display the last 20 messages if needed
	if (osd.message_archive_line == MESSAGE_BUFFER_LINES - 1)
	{
		for (int i = 2; i < MESSAGE_BUFFER_LINES; i++)
		{
			strcpy(osd.message_archive[i - 1], osd.message_archive[i]); // roll to 1
			osd.message_archive_severity[i - 1] = osd.message_archive_severity[i];
		}
	}
	else {
		osd.message_archive_line++;
	}

	strcpy(osd.message_archive[osd.message_archive_line], message);
	osd.message_archive_severity[osd.message_archive_line] = severity;

}


void message_buffer_render()
{
	long display_time = 5000;
	long now;
	char color;

	now = millis();

	if (osd.message_buffer_line > 0) display_time = 3000; // 3sec if there are more messages in the buffer

	// buffer line 0 is the actual displayed, message buffer starts from 1. 
	// message buffer line contains the last line of the buffer.

	
	//Check if the display time is ellapsed
	if ( now > (osd.message_buffer_display_time + display_time))
	{
		//Time is ellapsed for the current message, check if we have new message(s) in the buffer
		if (osd.message_buffer_line > 0)
		{
			//Prepare message to be displayed at the next cycle
			osd.message_buffer_display_time = now;
			for (int i = 1; i < MESSAGE_BUFFER_LINES; i++)
			{
				strcpy(osd.message_buffer[i - 1], osd.message_buffer[i]); // roll to 0
				osd.message_severity[i - 1] = osd.message_severity[i];
			}
			osd.message_buffer_line--;
		}
		else
		{
			osd.message_buffer_display_time = 0; //Nothing to display
		}

	}
	else //time is not up yet, so display the message at the top of the buffer
	{

		color = OSD256_FONT_WHITE;
		if (osd.message_severity[0] <= 3) color = OSD256_FONT_RED;

		OSD256_printf(osd.msg_widget.x, osd.msg_widget.y,color, 1, "%s", osd.message_buffer[0]);
	}

}


void message_list_render()
{

	for (char i = 0; i < MESSAGE_BUFFER_LINES; i++)
	{
		OSD256_printf(osd.msg_list_widget.x, osd.msg_list_widget.y + i * 24, OSD256_FONT_WHITE, 1, "%s", osd.message_archive[i]);
	}
}



void movement_render(move_widget_t *m)
{

	// osd.vx x movement in 100*m/s
	// osd.vy y movement in 100*m/s
	// osd.move.max maximum in normal m/s -> must convert to *100

	U16 s;
	U16 centerx, centery;
	int offsetx, offsety;
	float maximum;
	bitmap_names_t bitmap = BMP_BALL_WHITE;
	float v_pitch, v_roll;

	//Convert Global Frame Vx Vy to vehicle frame v_pitch and v_roll;

	v_roll  = osd.vy * osd.cos_yaw - osd.vx * osd.sin_yaw; // body roll vel
	v_pitch = osd.vy * osd.sin_yaw + osd.vx * osd.cos_yaw; // body pitch vel


//	debug("yaw: %f cos y:%f sin y:%f\n",osd.yaw, osd.cos_yaw, osd.sin_yaw);
//	debug("vx: %f, vy: %f\n", osd.vx, osd.vy);
//	debug("vroll: %f, vpitch: %f\n", v_roll, v_pitch);

	s = m->size - 1;
	centerx = m->x + (s + 1) / 2 - 7;
	centery = m->y + (s + 1) / 2 - 7;

	maximum = osd.move.max;
	


	offsety = (v_pitch / maximum) * (s + 1) / 2;
	offsetx = (v_roll / maximum) * (s + 1) / 2;

	if ((offsetx == 0) && (offsety == 0)) bitmap = BMP_BALL_GREEN;

	if (offsetx > (m->size / 2))
	{
		offsetx = m->size / 2;
		bitmap = BMP_BALL_BLUE;
	}
	else if (offsetx < (-(m->size / 2)))
	{
		offsetx = -(m->size / 2);
		bitmap = BMP_BALL_BLUE;
	}

	if (offsety >(m->size / 2))
	{
		offsety = m->size / 2;
		bitmap = BMP_BALL_BLUE;
	}
	else if (offsety < (-(m->size / 2)))
	{
		offsety = -(m->size / 2);
		bitmap = BMP_BALL_BLUE;
	}


	OSD256_drawline(PTH_X, COLOR_WHITE, m->x, m->y, m->x + s, m->y);
	OSD256_drawline(PTH_X, COLOR_WHITE, m->x, m->y + s, m->x + s, m->y + s);

	OSD256_drawline(PTH_X, COLOR_WHITE, m->x, m->y, m->x, m->y + s);
	OSD256_drawline(PTH_X, COLOR_WHITE, m->x + s, m->y, m->x + s, m->y + s);

	OSD256_drawline(PTH_X, COLOR_WHITE, m->x + (s+1)/2, m->y, m->x + (s+1)/2, m->y + s);
	OSD256_drawline(PTH_X, COLOR_WHITE, m->x, m->y + (s+1)/2, m->x + s, m->y + (s+1)/2);
	OSD256_Circle(PTH_X, COLOR_75_WHITE, m->x+s/2, m->y+s/2, m->size / 4);

	//reverse y offset
	OSD256_display_bitmap(bitmap, centerx+offsetx, centery-offsety);


}