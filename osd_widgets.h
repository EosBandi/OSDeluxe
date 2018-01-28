#ifndef _OSD_WIDGETS_H_
#define _OSD_WIDGETS_H_

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
     unsigned char  x;  // x position (4 pixels 0-180)
     unsigned short y;  // y position (field 0-288)
     unsigned char  w;  // width in 4pixels
     unsigned short h;  // height
     float max;         // maximum displayable value    
     float min;         // minimum displayable value
     float val;         // value to display
     float warn_yellow; // yellow warning threshold <=
     float warn_red;    // red warning threshold <=
     bool mix;          // alpha blending enabled 
     bartype bar_type;  // bar type - multicolor or single color
     char *format;    // printf format string to diasplay value
 };

struct gps_widget {
    unsigned short x;
    unsigned short y;
    unsigned char sat;
    unsigned char sat_warn;
    float         hdop;
    float         hdop_warn;
    unsigned char color;
};

struct battery_widget {
    unsigned short x;
    unsigned short y;
    struct bar volt;
    struct bar cap;
    float  current;
    float  voltage;
    unsigned int remaining_capacity;
    unsigned int max_capacity;
    unsigned char cells;
    float min_cell_voltage;
    float max_cell_voltage;
    float red_cell_voltage;
    float yellow_cell_voltage;
    bartype bar_type;
    bool mix;
    unsigned char display_members; //TODO: add posibility to switch off certain elements 
};

struct status_widget {
    unsigned short x;
    unsigned short y;

    status_info gps_status;
    status_info ekf_status;
    status_info vibe_status;

    bool mix;
};

struct alt_widget {
    unsigned short x;
    unsigned short y;
    float altitude;
    bool mix;
};

struct vario_widget {
    unsigned short x;
    unsigned short y;
    unsigned short h;
    unsigned short w;
    number_pos num_pos;
    float vario;
    float vario_max;
    bool mix;
};

struct home_widget {
    unsigned short x;
    unsigned short y;
    float orientation;
    unsigned int home_distance;    
};



extern struct alt_widget aw;
extern struct battery_widget bw;
extern struct gps_widget g;
extern struct bar b;
extern struct status_widget status;
extern struct vario_widget vw;
extern struct home_widget hw;

void osd_bar_prerender(struct bar *b);
void osd_bar(struct bar *b);
void osd_gps_prerender(struct gps_widget *g);
void osd_gps_render(struct gps_widget *g);

void osd_battery_prerender( struct battery_widget *bw);
void osd_battery_render( struct battery_widget *bw);

void osd_status_prerender( struct status_widget *s);
void osd_status_render( struct status_widget *s);

void osd_altitude_prerender( struct alt_widget *aw);
void osd_altitude_render( struct alt_widget *aw);

void osd_vario_prerender(struct vario_widget *vw);
void osd_vario_render(struct vario_widget *vw);

void osd_home_prerender(struct home_widget *hw);
void osd_home_render(struct home_widget *hw);

void osd_center_marker();
#endif