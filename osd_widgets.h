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

#ifndef _OSD_WIDGETS_H_
#define _OSD_WIDGETS_H_


#define BACKROUND COLOR_25_WHITE | MIX



enum bartype {
BAR_MULTICOLOR = 0,
BAR_SINGLE_COLOR = 1,
};

enum status_info {
    STATUS_OK = 0,
    STATUS_WARNING = 1,
    STATUS_CRITICAL = 2,
    STATUS_NONE = 3,
};

enum number_pos {
    POS_RIGHT = 0,
    POS_LEFT = 1, 
    POS_ABOVE = 2,
    POS_BELOW = 3
};


 struct bar {
     U16  x;  // x position (4 pixels 0-180)
     U16 y;  // y position (field 0-288)
     U16  w;  // width in 4pixels
     U16 h;  // height
     float max;         // maximum displayable value    
     float min;         // minimum displayable value
     float val;         // value to display
     float warn_yellow; // yellow warning threshold <=
     float warn_red;    // red warning threshold <=
     bool mix;          // alpha blending enabled 
	 bool box;			// outside box
     bartype bar_type;  // bar type - multicolor or single color
     char *format;      // printf format string to diasplay value
 };

struct gps_widget_t {
    U16 x;
    U16 y;
    U8 sat;
    U8 sat_warn;			//Warning level, makes the status yellow (It belongs to the status widget, but for PARAMS it easier to put here
	U8 sat_critical;     //Critical level, makes status red, and gps indicator also red
    float         hdop;
    float         hdop_warn;		//Warning level see above (hdop above this)
	float		  hdop_critical;	//Critical level see above  (hdop above this)
    U8  fix;
    U8 color;
	U8 visible;
};


struct batt_volt_widget_t {
	U16 x;
	U16 y;
	struct bar volt;
	U8 cells;
	float min_cell_voltage;
	float max_cell_voltage;
	float red_cell_voltage;
	float yellow_cell_voltage;
	bartype bar_type;
	bool mix;
	bool box;
	U8 visible;
};


struct batt_cap_widget_t {
	U16 x;
	U16 y;
	struct bar cap;
	int max_capacity;
	bartype bar_type;
	bool mix;
	bool box;
	U8 visible;
};


struct batt_curr_widget_t {
	U16 x;
	U16 y;
	bool mix;
	bool box;
	U8 visible;
};

struct batt_power_widget_t
{
	U16 x;
	U16 y;
	U8 visible;
};

struct status_widget_t {
    U16 x;
    U16 y;

    status_info gps_status;
    status_info ekf_status;
    status_info vibe_status;

    bool mix;
	U8 visible;
};

struct alt_widget_t {
    U16 x;
    U16 y;
    bool mix;
	U8 visible;
};

struct gs_widget_t {
	U16 x;
	U16 y;
	U8 visible;
};

struct rssi_widget_t {
    U16 x;
    U16 y;
    U8 visible;
    U8 rssi_warning;
    U8 rssi_critical;

};


struct throttle_widget_t {
	U16 x;
	U16 y;
	U8  visible;
};


struct pull_widget_t {
	U16 x;
	U16 y;
	float pull;
	bool mix;
	float warning;			//Warning level in Newtons
	U8 visible;
};

struct move_widget_t {
	U16 x;
	U16 y;
	U16  size;
	float		   max;
	char		   visible;

};

struct vario_widget_t {
    U16 x;
    U16 y;
    U16 h;
    U16 w;
    number_pos num_pos;
    float vario_max;
    bool mix;
	U8 visible;
};

struct vario_graph_widget_t {
	U16 x;
	U16 y;
	U16 h;
	U16 w;

	float vario_max;
	bool  mix;
	U8 visible;
};


struct home_widget_t {
    U16 x;
    U16 y;
    float orientation;
    unsigned int home_distance;    
	U8 visible;
};

struct horizon_t {
    U16  x;
    U16  y;

    int pitch;
    int roll;

	U8 visible;
};

struct mode_widget_t {
    U16 mode_x;
    U16 mode_y;
	char		   mode_centered;
	char		   mode_short;

    U16 fs_x;
    U16 fs_y;
	char		   fs_centered;

	U16 arm_x;
    U16 arm_y;
	char		   arm_centered;

	U8 visible;

};


struct compass_widget_t {
	U16 x;
	U16 y;

	U8 visible;
};

struct message_widget_t {

	U16  x;
	U16  y;
	bool mix;
	U8 visible;

};

struct message_list_widget_t {
	U16 x;
	U16 y;
	bool mix;
	
	U8 visible;
};


struct box_t {
	U16 x;
	U16 y;
	U16 w;
	U16 h;

	U8 color;
	U8 mix;

	U8 visible;

};


struct radar_widget_t {
	U16 x;
	U16 y;
	U16 size;
	U8  scale;
	U8 type;

	U8 visible;
};



void osd_bar_render(struct bar *b);
void osd_gps_render(struct gps_widget_t *g);

void osd_batt_volt_render(struct batt_volt_widget_t *bw, float voltage);
void osd_batt_cap_render(struct batt_cap_widget_t *bw, U8 remaining_capacity);
//void osd_battery_render( struct battery_widget_t *bw);
void osd_batt_curr_render(struct batt_curr_widget_t *bw, float current);
void osd_batt_power_render(struct batt_power_widget_t *bw, int power);

void osd_status_render( struct status_widget_t *s);
void osd_altitude_render( struct alt_widget_t *aw);
void osd_groundspeed_render(struct gs_widget_t *gs);
void osd_vario_render(struct vario_widget_t *vw);
void osd_home_render(struct home_widget_t *hw);
void osd_center_marker();
void render_horizon(struct horizon_t *horizon);
void render_horizona(struct horizon_t *horizon);
void osd_mode_render( struct mode_widget_t *mw);
void osd_pull_render(struct pull_widget_t *pw);
void rc_control(void);
void osd_throttle_render(struct throttle_widget_t *t);
void movement_render(move_widget_t *m);

void message_buffer_add_line(const char *message, char severity);
void message_buffer_render(void);
void message_list_render();
void osd_boxes_render();
void osd_compass_render(compass_widget_t *c);
void osd_render_vgraph(vario_graph_widget_t *w);
void osd_render_radar(radar_widget_t *w);
void osd_rssi_render(struct rssi_widget_t *r);

void render(unsigned int r);

#endif