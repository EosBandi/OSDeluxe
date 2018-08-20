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
char sCapFormat[] = "%5.0f\x5b";

const char *flight_modes_copter_long[] = {
    "Stabilize",      // STABILIZE = 0,  // manual airframe angle with manual throttle
    "Acro",           // ACRO = 1,  // manual body-frame angular rate with manual throttle
    "Altitude hold",  // ALT_HOLD = 2,  // manual airframe angle with automatic throttle
    "Auto",           // AUTO = 3,  // fully automatic waypoint control using mission commands
    "Guided",         // GUIDED = 4,  // fully automatic fly to coordinate or fly at velocity/direction using GCS immediate commands
    "Loiter",         // LOITER = 5,  // automatic horizontal acceleration with automatic throttle
    "Return to home", // RTL = 6,  // automatic return to launching point
    "Circle",         // CIRCLE = 7,  // automatic circular flight with automatic throttle
    "Unknown mode",
    "Land", // LAND = 9,  // automatic landing with horizontal position control
    "Unknown mode",
    "Drift", // DRIFT = 11,  // semi-automous position, yaw and throttle control
    "Unknown mode",
    "Sport",         // SPORT = 13,  // manual earth-frame angular rate control with manual throttle
    "Flip",          // FLIP = 14,  // automatically flip the vehicle on the roll axis
    "Auto Tune",     // AUTOTUNE = 15,  // automatically tune the vehicle's roll and pitch gains
    "Position Hold", // POSHOLD = 16,  // automatic position hold with manual override, with automatic throttle
    "Brake",         // BRAKE = 17,  // full-brake using inertial/GPS system, no pilot input
    "Throw",         // THROW = 18,  // throw to launch mode using inertial/GPS system, no pilot input
    "Avoid ADSB",    // AVOID_ADSB = 19,  // automatic avoidance of obstacles in the macro scale - e.g. full-sized aircraft
    "Guided No GPS", // GUIDED_NOGPS = 20,  // guided mode but only accepts attitude and altitude
    "Smart RTL",     // SMART_RTL = 21,  // SMART_RTL returns to home by retracing its steps
    "Flow Hold"      // FLOWHOLD = 22,  // FLOWHOLD holds position with optical flow without rangefinder
};

const char *flight_modes_copter_short[] = {
    "STAB",   // STABILIZE = 0,  // manual airframe angle with manual throttle
    "ACRO",   // ACRO = 1,  // manual body-frame angular rate with manual throttle
    "AltHLD", // ALT_HOLD = 2,  // manual airframe angle with automatic throttle
    "AUTO",   // AUTO = 3,  // fully automatic waypoint control using mission commands
    "GUIDED", // GUIDED = 4,  // fully automatic fly to coordinate or fly at velocity/direction using GCS immediate commands
    "LOITER", // LOITER = 5,  // automatic horizontal acceleration with automatic throttle
    "RTH",    // RTL = 6,  // automatic return to launching point
    "CIRCLE", // CIRCLE = 7,  // automatic circular flight with automatic throttle
    "UNKNWN",
    "LAND", // LAND = 9,  // automatic landing with horizontal position control
    "UNKNWN",
    "DRIFT", // DRIFT = 11,  // semi-automous position, yaw and throttle control
    "UNKNWN",
    "SPORT",   // SPORT = 13,  // manual earth-frame angular rate control with manual throttle
    "FLIP",    // FLIP = 14,  // automatically flip the vehicle on the roll axis
    "ATUNE",   // AUTOTUNE = 15,  // automatically tune the vehicle's roll and pitch gains
    "PosHLD",  // POSHOLD = 16,  // automatic position hold with manual override, with automatic throttle
    "BRAKE",   // BRAKE = 17,  // full-brake using inertial/GPS system, no pilot input
    "THROW",   // THROW = 18,  // throw to launch mode using inertial/GPS system, no pilot input
    "A-ADSB",  // AVOID_ADSB = 19,  // automatic avoidance of obstacles in the macro scale - e.g. full-sized aircraft
    "GdNoGPS", // GUIDED_NOGPS = 20,  // guided mode but only accepts attitude and altitude
    "SmrtRTL", // SMART_RTL = 21,  // SMART_RTL returns to home by retracing its steps
    "FlowHLD"  // FLOWHOLD = 22,  // FLOWHOLD holds position with optical flow without rangefinder
};

const char *flight_modes_plane_long[] = {

    "Manual",        // MANUAL = 0,
    "Circle",        // CIRCLE = 1,
    "Stabilize",     // STABILIZE = 2,
    "Training",      // TRAINING = 3,
    "Acro",          // ACRO = 4,
    "Fly By Wire A", // FLY_BY_WIRE_A = 5,
    "Fly By Wire B", // FLY_BY_WIRE_B = 6,
    "Cruise",        // CRUISE = 7,
    "Auto Tune",     // AUTOTUNE = 8,
    "Unknown mode",
    "Auto",             // AUTO = 10,
    "Return to Launch", // RTL = 11,
    "Loiter",           // LOITER = 12,
    "Unknown mode",
    "Avoid ADSB",        // AVOID_ADSB = 14,
    "Guided",            // GUIDED = 15,
    "Initialising",      // INITIALISING = 16,
    "Q Stabilize",       // QSTABILIZE = 17,
    "Q Hovering",        // QHOVER = 18,
    "Q Loitering",       // QLOITER = 19,
    "Q Land",            // QLAND = 20,
    "Q Return to Launch" // QRTL = 21
};

const char *flight_modes_plane_short[] = {

    "MANU",   // MANUAL = 0,
    "CIRC",   // CIRCLE = 1,
    "STAB",   // STABILIZE = 2,
    "TRNG",   // TRAINING = 3,
    "ACRO",   // ACRO = 4,
    "FBWA",   // FLY_BY_WIRE_A = 5,
    "FBWB",   // FLY_BY_WIRE_B = 6,
    "CRUISE", // CRUISE = 7,
    "ATUNE",  // AUTOTUNE = 8,
    "UNKNWN",
    "AUTO", // AUTO = 10,
    "RTL",  // RTL = 11,
    "LOIT", // LOITER = 12,
    "UNKNWN",
    "A-ADSB", // AVOID_ADSB = 14,
    "GUID",   // GUIDED = 15,
    "INIT",   // INITIALISING = 16,
    "QSTAB",  // QSTABILIZE = 17,
    "QHOVER", // QHOVER = 18,
    "QLOIT",  // QLOITER = 19,
    "QLAND",  // QLAND = 20,
    "QRTL"    // QRTL = 21
};

void osd_bar_render(struct bar *b)
{

    float val; // value to display, constrained to max and min
    int iw, rw, yw, tw;
    char color, mix;

    if (b->mix) mix = MIX;

    val = b->val;
    if (b->val > b->max) val = b->max;
    if (b->val < b->min) val = b->min;

    iw = b->w - 4; // internal width in pixels(quartets)
    rw = (float)iw * ((b->warn_red - b->min) / (b->max - b->min));
    yw = (float)iw * ((b->warn_yellow - b->min) / (b->max - b->min));
    tw = (float)iw * ((val - b->min) / (b->max - b->min));

    // Outside
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
    OSD256_printf(b->x - 2, b->y + b->h + 2, color, 0, b->format, b->val);
}

void osd_render_gps(struct gps_widget_t *g)
{

    char color;

    if (g->sat <= g->sat_critical || g->hdop > g->hdop_critical)
    {
        OSD256_display_bitmap(BMP_GPS_ICON_RED, g->x, g->y);
        color = OSD256_FONT_RED;
    }
    else if (g->sat <= g->sat_warn || g->hdop > g->hdop_critical)
    {
        OSD256_display_bitmap(BMP_GPS_ICON_ORANGE, g->x, g->y);
        color = OSD256_FONT_YELLOW;

    } else
    {
        OSD256_display_bitmap(BMP_GPS_ICON_GREEN, g->x, g->y);
        color = OSD256_FONT_YELLOW;
    }

    OSD256_printf(g->x + 42, g->y + 2, color, 0, "%u", g->sat);
    OSD256_printf(g->x + 42, g->y + 22, color, 0, "%2.2f", g->hdop);
}

void osd_render_batt_volt(struct batt_volt_widget_t *bw, float voltage)
{
    uint8_t cells = 0;

    // Autodetect cell count if necessary
    if (bw->cells == 0) // cells = 0 means autodetect
    {
        if (g.detected_cell_count == 0) // 0 detected cells means autodetect not yet run
        {
            if (voltage != 0) // We have some voltage to quess on
            {
                cells = voltage / 4;
                if (cells > 12) cells = 12; // Quick fix for 12S systems
                g.detected_cell_count = cells;
                char str[32];
                sprintf(str, STR_BATTERY_DETECTED, cells);
                message_buffer_add_line(str, 4);
            }
        }
        else
            cells = g.detected_cell_count;
    }
    else
        cells = bw->cells;

    bw->volt.x = bw->x;
    bw->volt.y = bw->y;
    bw->volt.w = 96;
    bw->volt.h = 20;

    bw->volt.max = bw->max_cell_voltage * cells;
    bw->volt.min = bw->min_cell_voltage * cells;
    bw->volt.val = voltage;
    bw->volt.warn_red = bw->red_cell_voltage * cells;
    bw->volt.warn_yellow = bw->yellow_cell_voltage * cells;
    bw->volt.mix = bw->mix;
    bw->volt.bar_type = bw->bar_type;
    bw->volt.format = sVoltFormat;
    bw->volt.box = bw->box;

    osd_bar_render(&bw->volt);
}

void osd_render_batt_cap(struct batt_cap_widget_t *bw, uint8_t remaining_capacity)
{

    bw->cap.x = bw->x;
    bw->cap.y = bw->y;
    bw->cap.w = 96;
    bw->cap.h = 20;

    bw->cap.max = bw->max_capacity;
    // debug("remaining:%u\n", bw->remaining_capacity);
    bw->cap.min = 0;
    if (remaining_capacity >= 0)
        bw->cap.val = bw->max_capacity * ((float)remaining_capacity / 100);
    else
        bw->cap.val = 0;
    bw->cap.warn_yellow = bw->max_capacity * 0.2f; // Yellow warning at 20%
    bw->cap.warn_red = bw->max_capacity * 0.1f;    // Red warning at 10%
    bw->cap.mix = bw->mix;
    bw->cap.bar_type = bw->bar_type;
    bw->cap.format = sCapFormat;
    bw->cap.box = bw->box;

    osd_bar_render(&bw->cap);
}

void osd_render_batt_curr(struct batt_curr_widget_t *bw, float current) { OSD256_printf(bw->x, bw->y, OSD256_FONT_YELLOW, 0, "%5.1fA", current); }

void osd_render_batt_power(struct batt_power_widget_t *bw, int power) { OSD256_printf(bw->x, bw->y, OSD256_FONT_YELLOW, 0, "%4uW", power); }

void osd_render_status(struct status_widget_t *s)
{

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

void osd_render_pull_force(struct pull_widget_t *pw)
{
    char color;

    if (pw->pull >= pw->warning)
        color = OSD256_FONT_RED;
    else
        color = OSD256_FONT_YELLOW;

    OSD256_printf(pw->x, pw->y, color, 0, "%2.2f N", pw->pull);
}

void osd_render_altitude(struct alt_widget_t *aw) { OSD256_printf(aw->x, aw->y, OSD256_FONT_YELLOW, 0, "\x5c\x5d%dm", (int)g.altitude); }

void osd_render_groundspeed(struct gs_widget_t *gs) { OSD256_printf(gs->x, gs->y, OSD256_FONT_YELLOW, 0, "\x5f %.0f\x7b\x7c", g.groundspeed * 3.6); }

void osd_render_throttle(struct throttle_widget_t *t) { OSD256_printf(t->x, t->y, OSD256_FONT_YELLOW, 0, "\x7d\x7e%u", g.rcin[3]); }

void osd_render_rssi(struct rssi_widget_t *r) {
	
	if (g.rssi <= osd.rssi.rssi_critical || g.remote_rssi <= osd.rssi.rssi_critical)
	{
        OSD256_printf(r->x, r->y, OSD256_FONT_RED_BLINK, 0, "\x83 %u/%u", g.rssi, g.remote_rssi); 
		return;
	}

	if (g.rssi <= osd.rssi.rssi_warning || g.remote_rssi <= osd.rssi.rssi_warning)
    {
        OSD256_printf(r->x, r->y, OSD256_FONT_RED, 0, "\x83 %u/%u", g.rssi, g.remote_rssi);
        return;
    }

	OSD256_printf(r->x, r->y, OSD256_FONT_YELLOW, 0, "\x83 %u/%u", g.rssi, g.remote_rssi); 
}



void osd_render_vario(struct vario_widget_t *vw)
{

    uint8_t mix;
    float val, f, fh;

    val = g.vario;
    if (val > vw->vario_max) val = vw->vario_max;
    if (val < -vw->vario_max) val = -vw->vario_max;

    if (vw->mix)
        mix = MIX;
    else
        mix = 0;

    OSD256_box(PTH_X, vw->x, vw->y, vw->w, vw->h, COLOR_WHITE | mix);
    OSD256_box(PTH_X, vw->x + 1, vw->y + 1, vw->w - 2, vw->h - 2, BACKROUND);

    f = vw->h / 2;
    fh = abs(f * val / vw->vario_max);

    if (g.vario >= 0)
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
        OSD256_printf(vw->x + vw->w + 1, vw->y + f - 4, OSD256_FONT_YELLOW, 1, "% 4.1fm/s", g.vario);
        break;
    case POS_LEFT:
        OSD256_printf(vw->x - 16, vw->y + f - 4, OSD256_FONT_YELLOW, 1, "% 4.1fm/s", g.vario);
        break;
    case POS_ABOVE:
        OSD256_printf(vw->x - 20, vw->y - 25, OSD256_FONT_YELLOW, 1, "% 4.1", g.vario);
        break;
    case POS_BELOW:
        OSD256_printf(vw->x - 20, vw->y + vw->h + 2, OSD256_FONT_YELLOW, 1, "% 4.1f", g.vario);
        break;
    }
}

void osd_render_home(struct home_widget_t *hw)
{

    struct polygon_t home_arrow;
    struct point_t home_arrow_points[5] = { { 0, -20 }, { +20, +20 }, { 0, -2 }, { -20, +20 } };
    home_arrow.len = 4;

    home_arrow.points = home_arrow_points;
    OSD256_transform_polygon(&home_arrow, hw->x, hw->y, hw->orientation);
    OSD256_draw_polygon(&home_arrow, COLOR_WHITE);

    if (g.home.lock == HOME_LOCKED)
        OSD256_printf(hw->x - 56, hw->y + 35, OSD256_FONT_YELLOW, 0, "% 4u m", hw->home_distance);
    else
        OSD256_printf(hw->x - 56, hw->y + 35, OSD256_FONT_YELLOW, 0, "no home");
}

void osd_render_center_marker()
{
    if (g.pthy_redraw)
    {
        OSD256_box(PTH_Y, 340, 286, 40, 6, 0x88);
        OSD256_box(PTH_Y, 340, 288, 40, 2, 0x99);

        OSD256_box(PTH_Y, 359, 268, 1, 40, 0x89);
        OSD256_box(PTH_Y, 360, 268, 1, 40, 0x98);
    }
}

#define RANGE 250
#define SCALE 2
#define MINOR_TICK 5
#define MAJOR_TICK 10
#define ZERO_LINE 120
#define MAJOR_LINE 50
#define MINOR_LINE 8

#define WIDTH 400
#define HEIGHT 300

void osd_render_horizon(struct horizon_t *h)
{
    short y, i;
    short x0, x1, y0, y1;
    short size;
    float cx, cy;
    // float pitchrad, rollrad;
    float cos_roll, sin_roll;
    uint8_t c1;

    // pitchrad = DEG2RAD(h->pitch);
    // rollrad  = DEG2RAD(h->roll);
    cos_roll = cos(DEG2RAD(h->roll));
    sin_roll = -1 * sin(DEG2RAD(h->roll));

    osd_draw_boundary = { (unsigned short)(h->x - WIDTH / 2), (unsigned short)(h->y - HEIGHT / 2), (unsigned short)(h->x + WIDTH / 2), (unsigned short)(h->y + HEIGHT / 2) };

    if ((abs(h->pitch) > 30) || (abs(h->roll) > 30))
    {
        c1 = COLOR_YELLOW;
    }
    else
    {
        c1 = COLOR_WHITE;
    }

    i = -(h->pitch * SCALE);
    y = h->y + i;

    size = ZERO_LINE; // Zero line

    // center point
    cx = h->x;
    cy = y + i;
    // start point
    x0 = cx - (size * cos_roll);
    x1 = cx + (size * cos_roll);
    // end point
    y0 = cy - (size * sin_roll);
    y1 = cy + (size * sin_roll);

    OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);
    OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0 - 2, x1, y1 - 2);

    // OSD256_printf(x0, y0, OSD256_FONT_YELLOW, 0, "%i", h->pitch);

    size = 60;
    cx = h->x;
    cy = y + i + 8;
    // start point
    x0 = cx - (size * cos_roll);
    x1 = cx + (size * cos_roll);
    // end point
    y0 = cy - (size * sin_roll);
    y1 = cy + (size * sin_roll);

    OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);
    OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0 + 2, x1, y1 + 2);

    osd_draw_boundary = { 0, 0, SCR_X_SIZE, SCR_Y_SIZE };
}

#define SCALE_A 6

void osd_render_horizona(struct horizon_t *h)
{
    int y, i, j;
    int x0, x1, y0, y1;
    int size, gap;
    float offset;
    float cx, cy;
    // float pitchrad, rollrad;
    float cos_roll, sin_roll;
    uint8_t c1;

    // pitchrad = DEG2RAD(h->pitch);
    // rollrad  = DEG2RAD(h->roll);
    cos_roll = cos(DEG2RAD(h->roll));
    sin_roll = -1 * sin(DEG2RAD(h->roll));

    if ((abs(h->pitch) > 30) || (abs(h->roll) > 30))
    {
        c1 = COLOR_ORANGE;
    }
    else
    {
        c1 = COLOR_WHITE;
    }

    for (i = -RANGE / 2; i <= RANGE / 2; i++)
    {
        y = h->y - i;
        j = (h->pitch * SCALE_A) + i;

        if (j % (MINOR_TICK * SCALE_A) == 0)
        {
            if (j == 0)
            {
                size = ZERO_LINE; // Zero line
                gap = 8;
            }
            else
            {
                if (j % (MAJOR_TICK * SCALE_A) == 0)
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
                OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0 - 2, x1, y1 - 2);
                OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0 + 2, x1, y1 + 2);
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
                OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0 + 2, x1, y1 + 2);
                OSD256_drawline(PTH_X, COLOR_BLACK, x0, y0 - 2, x1, y1 - 2);
            }
            else
            {
                OSD256_drawline(PTH_X, c1, x0, y0, x1, y1);
            }

            if ((j != 0) && (j % (MAJOR_TICK * SCALE_A) == 0))
            {
                OSD256_printf((cx - 20), (cy - 15), OSD256_FONT_WHITE, 1, "% 03d", j / SCALE_A);
            }
        }
    }
}

void osd_render_flmode(struct mode_widget_t *mw)
{

    char mode[32];

    unsigned int cust_mode;
    uint8_t mode_list_size;

    cust_mode = g.mode;

    switch (g.mav_type)
    {
    case MAV_TYPE_GENERIC:
    case MAV_TYPE_QUADROTOR:
    case MAV_TYPE_COAXIAL:
    case MAV_TYPE_HELICOPTER:
    case MAV_TYPE_HEXAROTOR:
    case MAV_TYPE_OCTOROTOR:
    case MAV_TYPE_TRICOPTER:
    case MAV_TYPE_DODECAROTOR:
        mode_list_size = sizeof(flight_modes_copter_long) / sizeof(flight_modes_copter_long[0]);
        if (g.mode > mode_list_size) cust_mode = 8;
        if (osd.mode.mode_short)
            strcpy(mode, flight_modes_copter_short[cust_mode]);
        else
            strcpy(mode, flight_modes_copter_long[cust_mode]);
        break;

    case MAV_TYPE_FIXED_WING:
    case MAV_TYPE_VTOL_DUOROTOR:
    case MAV_TYPE_VTOL_QUADROTOR:
    case MAV_TYPE_VTOL_TILTROTOR:
        mode_list_size = sizeof(flight_modes_plane_long) / sizeof(flight_modes_plane_long[0]);
        if (g.mode > mode_list_size) cust_mode = 8;
        if (osd.mode.mode_short)
            strcpy(mode, flight_modes_plane_short[cust_mode]);
        else
            strcpy(mode, flight_modes_plane_long[cust_mode]);
        break;

    default:
        strcpy(mode, "unknown");
    }
    if (g.mode == 3 && g.wp_seq > 0)
    {
        sprintf(mode, "%s %u>%u", mode, g.wp_distance, g.wp_seq);
    }

    if (mw->mode_centered)
    {
        int l = (float)strlen(mode) * 15;
        OSD256_printf(mw->mode_x - (l / 2), mw->mode_y, OSD256_FONT_YELLOW, 0, "%s", mode);
    }
    else
        OSD256_printf(mw->mode_x, mw->mode_y, OSD256_FONT_YELLOW, 0, "%s", mode);

    if (g.system_status == MAV_STATE_CRITICAL)
    {
        if (mw->fs_centered)
            OSD256_printf(mw->fs_x - 67, mw->fs_y, OSD256_FONT_RED_BLINK, 0, "FAILSAFE!");
        else
            OSD256_printf(mw->fs_x, mw->fs_y, OSD256_FONT_RED_BLINK, 0, "FAILSAFE!");
    }

    if (g.arming_status == 0)
    {
        if (mw->arm_centered)
            OSD256_printf(mw->arm_x - 60, mw->arm_y, OSD256_FONT_RED, 0, "Disarmed");
        else
            OSD256_printf(mw->arm_x, mw->arm_y, OSD256_FONT_RED, 0, "Disarmed");

        g.armed_start_time = 0;
    }
    else // Arming status 1 ARMED
    {
        if (g.armed_start_time == 0)
        {
            g.armed_start_time = millis();
            g.home.lock = HOME_WAIT;
            mavlink_seen[MAVLINK_MSG_ID_MISSION_ITEM] = 0xffff;
        }
        else if (millis() < (g.armed_start_time + 8000))
        {
            if (mw->arm_centered)
                OSD256_printf(mw->arm_x - 37, mw->arm_y, OSD256_FONT_RED_BLINK, 0, "ARMED");
            else
                OSD256_printf(mw->arm_x, mw->arm_y, OSD256_FONT_RED_BLINK, 0, "ARMED");
        }
    }
}

void rc_control() {}

// line 0 is a display line
void message_buffer_add_line(const char *message, char severity)
{
    // Check if we are standing at the last line of the buffer.
    if (g.message_buffer_line == MESSAGE_BUFFER_LINES - 1)
    {
        for (int i = 2; i < MESSAGE_BUFFER_LINES; i++)
        {
            strcpy(g.message_buffer[i - 1], g.message_buffer[i]); // roll to 1
            g.message_severity[i - 1] = g.message_severity[i];
        }
    }
    else
    {
        g.message_buffer_line++;
    }

    strcpy(g.message_buffer[g.message_buffer_line], message);
    g.message_severity[g.message_buffer_line] = severity;

    // Add line to the archive as well, so we can always display the last 20 messages if needed
    if (g.message_archive_line == MESSAGE_BUFFER_LINES - 1)
    {
        for (int i = 2; i < MESSAGE_BUFFER_LINES; i++)
        {
            strcpy(g.message_archive[i - 1], g.message_archive[i]); // roll to 1
            g.message_archive_severity[i - 1] = g.message_archive_severity[i];
        }
    }
    else
    {
        g.message_archive_line++;
    }

    strcpy(g.message_archive[g.message_archive_line], message);
    g.message_archive_severity[g.message_archive_line] = severity;
}

void osd_render_message_buffer()
{
    long display_time = 5000;
    long now;
    char color;

    now = millis();

    if (g.message_buffer_line > 0) display_time = 3000; // 3sec if there are more messages in the buffer

    // buffer line 0 is the actual displayed, message buffer starts from 1.
    // message buffer line contains the last line of the buffer.

    // Check if the display time is ellapsed
    if (now > (g.message_buffer_display_time + display_time))
    {
        // Time is ellapsed for the current message, check if we have new message(s) in the buffer
        if (g.message_buffer_line > 0)
        {
            // Prepare message to be displayed at the next cycle
            g.message_buffer_display_time = now;
            for (int i = 1; i < MESSAGE_BUFFER_LINES; i++)
            {
                strcpy(g.message_buffer[i - 1], g.message_buffer[i]); // roll to 0
                g.message_severity[i - 1] = g.message_severity[i];
            }
            g.message_buffer_line--;
        }
        else
        {
            g.message_buffer_display_time = 0; // Nothing to display
        }
    }
    else // time is not up yet, so display the message at the top of the buffer
    {

        color = OSD256_FONT_WHITE;
        if (g.message_severity[0] <= 3) color = OSD256_FONT_RED;

        OSD256_printf(osd.msg_widget.x, osd.msg_widget.y, color, 1, "%s", g.message_buffer[0]);
    }
}

void osd_render_message_list()
{

    for (uint8_t i = 0; i < MESSAGE_BUFFER_LINES; i++)
    {
        OSD256_printf(osd.msg_list_widget.x, osd.msg_list_widget.y + i * 24, OSD256_FONT_WHITE, 1, "%s", g.message_archive[i]);
    }
}

void osd_render_move(move_widget_t *m)
{

    // osd.vx x movement in 100*m/s
    // osd.vy y movement in 100*m/s
    // osd.move.max maximum in normal m/s -> must convert to *100

    uint16_t s;
    uint16_t centerx, centery;
    int offsetx, offsety;
    float maximum;
    bitmap_names_t bitmap = BMP_BALL_WHITE;
    float v_pitch, v_roll;

    // Convert Global Frame Vx Vy to vehicle frame v_pitch and v_roll;

    v_roll = g.vy * g.cos_yaw - g.vx * g.sin_yaw;  // body roll vel
    v_pitch = g.vy * g.sin_yaw + g.vx * g.cos_yaw; // body pitch vel

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
        bitmap = BMP_BALL_RED;
    }
    else if (offsetx < (-(m->size / 2)))
    {
        offsetx = -(m->size / 2);
        bitmap = BMP_BALL_RED;
    }

    if (offsety > (m->size / 2))
    {
        offsety = m->size / 2;
        bitmap = BMP_BALL_RED;
    }
    else if (offsety < (-(m->size / 2)))
    {
        offsety = -(m->size / 2);
        bitmap = BMP_BALL_RED;
    }

    if (g.pthy_redraw)
    {
        OSD256_drawline(PTH_Y, 0x11 | REC_MIX, m->x, m->y, m->x + s, m->y);
        OSD256_drawline(PTH_Y, 0x11 | REC_MIX, m->x, m->y + s, m->x + s, m->y + s);

        OSD256_drawline(PTH_Y, 0x11 | REC_MIX, m->x, m->y, m->x, m->y + s);
        OSD256_drawline(PTH_Y, 0x11 | REC_MIX, m->x + s, m->y, m->x + s, m->y + s);

        OSD256_drawline(PTH_Y, 0x11 | REC_MIX, m->x + (s + 1) / 2, m->y, m->x + (s + 1) / 2, m->y + s);
        OSD256_drawline(PTH_Y, 0x11 | REC_MIX, m->x, m->y + (s + 1) / 2, m->x + s, m->y + (s + 1) / 2);

        OSD256_circle(PTH_Y, 0x99, m->x + s / 2, m->y + s / 2, m->size / 4);
    }
    // reverse y offset
    OSD256_display_bitmap(bitmap, centerx + offsetx, centery - offsety);
}

void osd_boxes_render()
{
    uint8_t color;

    if (g.pthy_redraw)
    {
        for (int i = 0; i < 8; i++)
        {
            if ((osd.boxes[i].h > 0) && (osd.boxes[i].w > 0))
            {
                if (osd.boxes[i].visible & g.visible_osd_page)
                {

                    switch (osd.boxes[i].color)
                    {
                    case 0:
                        color = COLOR_REC_BLACK;
                        break;
                    case 1:
                        color = COLOR_REC_WHITE;
                        break;
                    case 2:
                        color = COLOR_REC_50_WHITE;
                        break;
                    case 3:
                        color = COLOR_REC_25_WHITE;
                        break;
                    default:
                        color = COLOR_REC_BLACK;
                    }
                    if (osd.boxes[i].mix) color = color | REC_MIX;

                    OSD256_box(PTH_Y, osd.boxes[i].x, osd.boxes[i].y, osd.boxes[i].w, osd.boxes[i].h, color);
                }
            }
        }
    }
}

void osd_render_compass(compass_widget_t *c)
{
    const char cardinals[] = { 'N', 'E', 'S', 'W' };
    int i, j, x;

    uint16_t x_center = 75 + c->x;
    uint16_t y_center = 25 + c->y;
    uint16_t range = 150;
    uint16_t chr_pos;

    OSD256_printf(x_center - 8, c->y + 12, OSD256_FONT_WHITE, 0, "\x8f");

    if (g.heading < 10) chr_pos = 8;
    if ((g.heading >= 10) && (g.heading < 100)) chr_pos = 16;
    if (g.heading >= 100) chr_pos = 24;

    OSD256_printf(x_center - chr_pos, c->y, OSD256_FONT_YELLOW, 0, "%u", g.heading);

    y_center += 6;
    for (i = -range / 2; i <= range / 2; i++)
    {
        x = x_center + i;
        j = (g.heading + i + 360) % 360;
        if (j == 0 || j == 90 || j == 180 || j == 270)
        {
            OSD256_box(PTH_X, x - 2, y_center, 5, 8, COLOR_BLACK);
            OSD256_box(PTH_X, x - 1, y_center, 3, 8, COLOR_WHITE);

            OSD256_printf(x - 7, y_center + 10, OSD256_FONT_YELLOW, 0, "%c", cardinals[j / 90]);
        }
        else if (j % 45 == 0)
        {
            OSD256_box(PTH_X, x - 2, y_center, 5, 8, COLOR_BLACK);
            OSD256_box(PTH_X, x - 1, y_center, 3, 8, COLOR_WHITE);
        }
        else if (j % 15 == 0)
        {
            OSD256_box(PTH_X, x - 2, y_center, 5, 4, COLOR_BLACK);
            OSD256_box(PTH_X, x - 1, y_center, 3, 4, COLOR_WHITE);
        }
    }

    OSD256_box(PTH_X, c->x - 8, c->y, 15, 60, 0xff);
    OSD256_box(PTH_X, c->x + range - 6, c->y, 16, 60, 0xff);
}

void osd_render_vgraph(vario_graph_widget_t *w)
{

    uint16_t y_center;
    uint8_t mix = 0;
    float val;

    val = g.vario;

    if (w->mix) mix = MIX;

    y_center = w->y + (w->h / 2);

    if (g.pthy_redraw)
    {
        OSD256_drawline(PTH_Y, COLOR_REC_WHITE, w->x + w->w, w->y, w->x + w->w, w->y + w->h);
        OSD256_drawline(PTH_Y, COLOR_REC_WHITE, w->x, y_center, w->x + w->w, y_center);
    }

    // for test
    // g.variot += ((float)random(20)/100.0f - 0.1f);
    // val = g.variot;

    if (val > w->vario_max) val = w->vario_max;
    if (val < -w->vario_max) val = -w->vario_max;

    memcpy(&g.vario_graph[0], &g.vario_graph[1], sizeof(short) * 79);

    g.vario_graph[79] = val * 10 * ((w->h / 2) / (w->vario_max * 10));

    for (int i = 0; i < 80; i++)
    {
        if (g.vario_graph[i] > 0)
            OSD256_drawline(PTH_X, COLOR_GREEN | mix, w->x + i, y_center, w->x + i, y_center - g.vario_graph[i]);
        else
            OSD256_drawline(PTH_X, COLOR_RED | mix, w->x + i, y_center, w->x + i, y_center - g.vario_graph[i]);
    }
}

void osd_render_radar(radar_widget_t *w)
{
    unsigned long d = (unsigned long)g.home.distance;
    unsigned int r = (w->size / 2) - 2;
    int x, y;
    int min_increment;
    long i, scale;
    struct point_t ils_points[5] = { { -4, -6 }, { -4, 6 }, { 0, 10 }, { 4, 6 }, { 4, -6 } };
    struct polygon_t ils;

    ils.len = 5;
    ils.points = ils_points;
    struct point_t uav_points[4] = { { 0, 0 }, { 6, 8 }, { 0, -8 }, { -6, 8 } };

    struct polygon_t uav;
    uav.len = 4;
    uav.points = uav_points;

    struct polygon_t *p;

    x = (w->size / 2) - 1;
    y = (w->size / 2) - 1;

    if (g.pthy_redraw)
    {

        OSD256_drawline(PTH_Y, COLOR_REC_WHITE | REC_MIX, x + w->x, 0 + w->y, x + w->x, r * 2 + w->y);
        OSD256_drawline(PTH_Y, COLOR_REC_WHITE | REC_MIX, 0 + w->x, y + w->y, r * 2 + w->x, y + w->y);
        OSD256_circle(PTH_Y, COLOR_REC_WHITE | REC_MIX, x + w->x, y + w->y, r);
        OSD256_circle(PTH_Y, COLOR_REC_25_WHITE | REC_MIX, x + w->x, y + w->y, r + 2);
    }

    /* auto scale */
    min_increment = 250;
    scale = ((d / min_increment) + 1) * min_increment;
    OSD256_printf(w->x, w->y, OSD256_FONT_WHITE, 1, "%um", (unsigned int)scale);

    i = (long)d * r;
    i /= scale;

    switch (w->type)
    {
    case 0:
        /* radar fixed at uav heading, home moves */
        x += sin(DEG2RAD(g.home.direction)) * i;
        y -= cos(DEG2RAD(g.home.direction)) * i;
        OSD256_scale_polygon(&ils, w->scale);
        OSD256_transform_polygon(&ils, x, y, g.launch_heading - g.heading - 180);
        p = &ils;
        break;
    case 1:
        /* radar always facing north, uav moves */
        x += sin(DEG2RAD(g.home.uav_bearing)) * i;
        y -= cos(DEG2RAD(g.home.uav_bearing)) * i;
        OSD256_scale_polygon(&uav, w->scale);
        OSD256_transform_polygon(&uav, x, y, g.heading);
        p = &uav;
        break;
    case 2:
        //		/* radar always facing north, uav moves with waypoints */
        if (g.wp_seq > 0)
        {
            long i_wp = (long)g.wp_distance * r;
            i_wp /= scale;
            int x_wp = x, y_wp = y;
            x_wp += sin(DEG2RAD(g.wp_target_bearing - g.heading)) * i_wp;
            y_wp -= cos(DEG2RAD(g.wp_target_bearing - g.heading)) * i_wp;
            OSD256_printf(x_wp + w->x - 6, y_wp + w->y - 10, OSD256_FONT_WHITE, 1, "%d", g.wp_seq);
        }
        x += sin(DEG2RAD(g.home.uav_bearing)) * i;
        y -= cos(DEG2RAD(g.home.uav_bearing)) * i;
        OSD256_scale_polygon(&uav, w->scale);
        OSD256_transform_polygon(&uav, x, y, g.heading);
        p = &uav;
        break;
    default:	//should not happen, just to satisfy compiller
        p = &uav;
        break;
    }
    OSD256_move_polygon(p, w->x, w->y);
    OSD256_move_polygon(p, -1, -1);
    OSD256_draw_polygon(p, COLOR_BLACK);
    OSD256_move_polygon(p, 1, 1);
    OSD256_draw_polygon(p, COLOR_WHITE);
}
