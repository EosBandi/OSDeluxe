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
     uint16_t  x;  // x position (4 pixels 0-180)
     uint16_t y;  // y position (field 0-288)
     uint16_t  w;  // width in 4pixels
     uint16_t h;  // height
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
    uint16_t x;
    uint16_t y;
    uint8_t sat;
    uint8_t sat_warn;			//Warning level, makes the status yellow (It belongs to the status widget, but for PARAMS it easier to put here
	uint8_t sat_critical;     //Critical level, makes status red, and gps indicator also red
    float         hdop;
    float         hdop_warn;		//Warning level see above (hdop above this)
	float		  hdop_critical;	//Critical level see above  (hdop above this)
    uint8_t  fix;
    uint8_t color;
	uint8_t visible;
};


struct batt_volt_widget_t {
	uint16_t x;
	uint16_t y;
	struct bar volt;
	uint8_t cells;
	float min_cell_voltage;
	float max_cell_voltage;
	float red_cell_voltage;
	float yellow_cell_voltage;
	bartype bar_type;
	bool mix;
	bool box;
	uint8_t visible;
};


struct batt_cap_widget_t {
	uint16_t x;
	uint16_t y;
	struct bar cap;
	int max_capacity;
	bartype bar_type;
	bool mix;
	bool box;
	uint8_t visible;
};


struct batt_curr_widget_t {
	uint16_t x;
	uint16_t y;
	bool mix;
	bool box;
	uint8_t visible;
};

struct batt_power_widget_t
{
	uint16_t x;
	uint16_t y;
	uint8_t visible;
};

struct status_widget_t {
    uint16_t x;
    uint16_t y;

    status_info gps_status;
    status_info ekf_status;
    status_info vibe_status;

    bool mix;
	uint8_t visible;
};

struct alt_widget_t {
    uint16_t x;
    uint16_t y;
    bool mix;
	uint8_t visible;
};

struct gs_widget_t {
	uint16_t x;
	uint16_t y;
	uint8_t visible;
};

struct rssi_widget_t {
    uint16_t x;
    uint16_t y;
    uint8_t visible;
    uint8_t rssi_warning;
    uint8_t rssi_critical;

};


struct throttle_widget_t {
	uint16_t x;
	uint16_t y;
	uint8_t  visible;
};


struct pull_widget_t {
	uint16_t x;
	uint16_t y;
	float pull;
	bool mix;
	float warning;			//Warning level in Newtons
	uint8_t visible;
};

struct move_widget_t {
	uint16_t x;
	uint16_t y;
	uint16_t  size;
	float		   max;
	char		   visible;

};

struct vario_widget_t {
    uint16_t x;
    uint16_t y;
    uint16_t h;
    uint16_t w;
    number_pos num_pos;
    float vario_max;
    bool mix;
	uint8_t visible;
};

struct vario_graph_widget_t {
	uint16_t x;
	uint16_t y;
	uint16_t h;
	uint16_t w;

	float vario_max;
	bool  mix;
	uint8_t visible;
};


struct home_widget_t {
    uint16_t x;
    uint16_t y;
    float orientation;
    unsigned int home_distance;    
	uint8_t visible;
};

struct horizon_t {
    uint16_t  x;
    uint16_t  y;

    int pitch;
    int roll;

	uint8_t visible;
};

struct mode_widget_t {
    uint16_t mode_x;
    uint16_t mode_y;
	char		   mode_centered;
	char		   mode_short;

    uint16_t fs_x;
    uint16_t fs_y;
	char		   fs_centered;

	uint16_t arm_x;
    uint16_t arm_y;
	char		   arm_centered;

	uint8_t visible;

};


struct compass_widget_t {
	uint16_t x;
	uint16_t y;

	uint8_t visible;
};

struct message_widget_t {

	uint16_t  x;
	uint16_t  y;
	bool mix;
	uint8_t visible;

};

struct message_list_widget_t {
	uint16_t x;
	uint16_t y;
	bool mix;
	
	uint8_t visible;
};


struct box_t {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;

	uint8_t color;
	uint8_t mix;

	uint8_t visible;

};


struct radar_widget_t {
	uint16_t x;
	uint16_t y;
	uint16_t size;
	uint8_t  scale;
	uint8_t type;

	uint8_t visible;
};



void osd_bar_render(struct bar *b);
void osd_render_gps(struct gps_widget_t *g);

void osd_render_batt_volt(struct batt_volt_widget_t *bw, float voltage);
void osd_render_batt_cap(struct batt_cap_widget_t *bw, uint8_t remaining_capacity);
//void osd_battery_render( struct battery_widget_t *bw);
void osd_render_batt_curr(struct batt_curr_widget_t *bw, float current);
void osd_render_batt_power(struct batt_power_widget_t *bw, int power);

void osd_render_status( struct status_widget_t *s);
void osd_render_altitude( struct alt_widget_t *aw);
void osd_render_groundspeed(struct gs_widget_t *gs);
void osd_render_vario(struct vario_widget_t *vw);
void osd_render_home(struct home_widget_t *hw);
void osd_render_center_marker();
void osd_render_horizon(struct horizon_t *horizon);
void render_horizona(struct horizon_t *horizon);
void osd_render_flmode( struct mode_widget_t *mw);
void osd_render_pull_force(struct pull_widget_t *pw);
void rc_control(void);
void osd_render_throttle(struct throttle_widget_t *t);
void osd_render_move(move_widget_t *m);

void message_buffer_add_line(const char *message, char severity);
void osd_render_message_buffer(void);
void osd_render_message_list();
void osd_boxes_render();
void osd_render_compass(compass_widget_t *c);
void osd_render_vgraph(vario_graph_widget_t *w);
void osd_render_radar(radar_widget_t *w);
void osd_render_rssi(struct rssi_widget_t *r);

void render(unsigned int r);

#endif