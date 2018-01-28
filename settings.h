#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define CHANNEL_MODES 6




struct osd_settings {

    channel_setting video_channels[CHANNEL_MODES][5]; // ch 1-ch4 (no zero)
    gps_widget gps;
    battery_widget bat;
    status_widget stat;
    alt_widget alt;
    vario_widget vario;
    home_widget home;

};


void default_settings();
void load_settings();
void save_settings();


#endif