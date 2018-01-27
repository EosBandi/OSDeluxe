#ifndef _OSD_WIDGETS_H_
#define _OSD_WIDGETS_H_

enum bartype {
BAR_MULTICOLOR = 0,
BAR_SINGLE_COLOR = 1,
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
    unsigned char cells;
    float min_cell_voltage;
    float max_cell_voltage;
    float red_cell_voltage;
    float yellow_cell_voltage;
    bartype bar_type;
};

extern struct battery_widget bw;
extern struct gps_widget g;
extern struct bar b;





void osd_bar_prerender(struct bar *b);
void osd_bar(struct bar *b);
void osd_gps_prerender(struct gps_widget *g);
void osd_gps_render(struct gps_widget *g);

void osd_battery_prerender( struct battery_widget *bw);
void osd_battery_render( struct battery_widget *bw);


#endif