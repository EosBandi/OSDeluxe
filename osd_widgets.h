#ifndef _OSD_WIDGETS_H_
#define _OSD_WIDGETS_H_

void osd_bar_prerender(struct bar *b);
void osd_bar(struct bar *b);
void osd_gps_prerender(struct gps_widget *g);
void osd_gps_render(struct gps_widget *g);

void osd_battery_prerender( struct battery_widget *bw);
void osd_battery_render( struct battery_widget *bw);


#endif