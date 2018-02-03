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

enum flight_mode {
    PLANE_MODE_MANUAL     = 0,
    PLANE_MODE_CIRCLE     = 1,
    PLANE_MODE_STABILIZE  = 2,
    PLANE_MODE_TRAINING   = 3,
    PLANE_MODE_ACRO       = 4,
    PLANE_MODE_FBWA       = 5,
    PLANE_MODE_FBWB       = 6,
    PLANE_MODE_CRUISE     = 7,
    PLANE_MODE_AUTOTUNE   = 8,
    PLANE_MODE_AUTO       = 10,
    PLANE_MODE_RTL        = 11,
    PLANE_MODE_LOITER     = 12,
    PLANE_MODE_GUIDED     = 15,
    PLANE_MODE_INIT       = 16,
    COPTER_MODE_STABILIZE = 100,
    COPTER_MODE_ACRO      = 101,
    COPTER_MODE_ALTHOLD   = 102,
    COPTER_MODE_AUTO      = 103,
    COPTER_MODE_GUIDED    = 104,
    COPTER_MODE_LOITER    = 105,
    COPTER_MODE_RTL       = 106,
    COPTER_MODE_CIRCLE    = 107,
    COPTER_MODE_LAND      = 109,
    COPTER_MODE_OF_LOITER = 110,
    COPTER_MODE_DRIFT     = 111,
    COPTER_MODE_SPORT     = 113,
    COPTER_MODE_FLIP      = 114,
    COPTER_MODE_AUTOTUNE  = 115,
    COPTER_MODE_POSHOLD   = 116,
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

struct gps_widget_t {
    unsigned short x;
    unsigned short y;
    unsigned char sat;
    unsigned char sat_warn;
    float         hdop;
    float         hdop_warn;
    unsigned char  fix;
    unsigned char color;
};

struct battery_widget_t {
    unsigned short x;
    unsigned short y;
    struct bar volt;
    struct bar cap;
    float  current;
    float  voltage;
    int remaining_capacity;
    int max_capacity;
    unsigned char cells;
    float min_cell_voltage;
    float max_cell_voltage;
    float red_cell_voltage;
    float yellow_cell_voltage;
    bartype bar_type;
    bool mix;
    unsigned char display_members; //TODO: add posibility to switch off certain elements 
};

struct status_widget_t {
    unsigned short x;
    unsigned short y;

    status_info gps_status;
    status_info ekf_status;
    status_info vibe_status;

    bool mix;
};

struct alt_widget_t {
    unsigned short x;
    unsigned short y;
    float altitude;
    bool mix;
};

struct vario_widget_t {
    unsigned short x;
    unsigned short y;
    unsigned short h;
    unsigned short w;
    number_pos num_pos;
    float vario;
    float vario_max;
    bool mix;
};

struct home_widget_t {
    unsigned short x;
    unsigned short y;
    float orientation;
    unsigned int home_distance;    
};

struct horizon_t {
    unsigned short  x;
    unsigned short  y;

    int pitch;
    int roll;
};

struct mode_widget_t {
    unsigned short x;
    unsigned short y;
    bool mix;
    unsigned char mode; //from mavlink
};

extern struct alt_widget_t aw;
extern struct battery_widget_t bw;
extern struct gps_widget_t g;
extern struct bar b;
extern struct status_widget_t status;
extern struct vario_widget_t vw;
extern struct home_widget_t hw;
extern struct horizon_t hor;

void osd_bar_prerender(struct bar *b);
void osd_bar(struct bar *b);
void osd_gps_prerender(struct gps_widget_t *g);
void osd_gps_render(struct gps_widget_t *g);

void osd_battery_prerender( struct battery_widget_t *bw);
void osd_battery_render( struct battery_widget_t *bw);

void osd_status_prerender( struct status_widget_t *s);
void osd_status_render( struct status_widget_t *s);

void osd_altitude_prerender( struct alt_widget_t *aw);
void osd_altitude_render( struct alt_widget_t *aw);

void osd_vario_prerender(struct vario_widget_t *vw);
void osd_vario_render(struct vario_widget_t *vw);

void osd_home_prerender(struct home_widget_t *hw);
void osd_home_render(struct home_widget_t *hw);

void osd_center_marker();

void render_horizon(struct horizon_t *horizon);
void osd_mode_render( struct mode_widget_t *mw);

#endif