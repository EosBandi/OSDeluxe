
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

//#define TW2837_I2C
#include "cli.h"
#define TW2837_MPP

#include "OSDeluxe.h"

#ifdef TW2837_I2C

#define TW_RESET 14

#else

#define TW_RESET 23

#define TW_CS 22
#define TW_WR 12
#define TW_RD 11
#define TW_AEN 15

#endif

#define LED_PIN 13

global_variables_t g;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(TW_RESET, OUTPUT);

#ifndef TW2837_I2C

    pinMode(TW_CS, OUTPUT);
    pinMode(TW_WR, OUTPUT);
    pinMode(TW_RD, OUTPUT);
    pinMode(TW_AEN, OUTPUT);

    digitalWriteFast(TW_CS, 1);
    digitalWriteFast(TW_WR, 1);
    digitalWriteFast(TW_RD, 1);
    digitalWriteFast(TW_AEN, 0);

    setWriteDirInline();
#endif

    // Reset TW2837
    delay(1000);
    digitalWrite(TW_RESET, LOW);
    delay(500);
    digitalWrite(TW_RESET, HIGH);
    delay(500);

    // Setup for Master mode, pins 18/19, internal pullups, 4Mhz, 10ms default timeout
    // It is not a mistake TW2837 works fine on 4Mhz I2C speed

#ifndef TW2837_I2C
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 4000000, I2C_OP_MODE_IMM);
    Wire.setDefaultTimeout(1000); // 10ms
#endif

    // Open USB serial port for debug and UI
    Serial.begin(115200);

    // Open Serial1 port for MavLink communication
    // TODO: make it configurable
    Serial1.begin(115200);
}

void loop()
{
    // Led ON
    digitalWrite(LED_PIN, HIGH);

    // Initn TW2837 registers
    tw_init();

    g.pip_page = 0;

    tw_write_register(0x0c8, 0x03);
    tw_write_register(0x057, 0x00); // Extra coring for sharepning

    tw_write_register(0x240, 0x01);

    // We will use these screens
    // Don't clear all since pages above 0 share memory with scratch table (Not mentioned in the datasheet)

    OSD256_clear_screen(PTH_X, 0); // Main Path page 0
    OSD256_clear_screen(PTH_X, 1); // Main Path page 1

    OSD256_clear_screen(PTH_Y, 0); // Secondary Path page 0 - draw static elements on this page

    OSD256_set_display_page(0); // Display page 0

    load_settings();
    if ((osd.eeprom_version != EEPROM_VERSION) || (osd.settings_size != sizeof(osd)))
    {
        default_settings();
        save_settings();
    }

    update_vout_settings(); // Apply vout settings from saved values
    update_vin_settings();  // Apply vin settings from saved values
    update_pip();           // Set up PIP state

    g.displayed_mode = -1; // Signal startup

    g.mode = 0;

    // Clear buffers for mavlink messages
    memset(&g.message_buffer, 0, sizeof(g.message_buffer));
    memset(&g.message_archive, 0, sizeof(g.message_archive));

    g.message_buffer_line = 0;
    g.message_buffer_display_time = 0;

    // Start with osd_page 1 and pip page 0
    g.visible_osd_page = 0x01; // bit coded
    g.pip_page = 0x00;

    init_home();

    get_parameter_count();
    param_send_index = total_params;

    // Display both fields !!!!
    tw_write_register(0x20f, 0x0f);

    // OSD write will use page0
    OSD256_wr_page = 0;

    g.arming_status = false;
    g.displayed_arming_status = true;
    g.armed_start_time = 0;
    g.last_capacity_query = 0;
    g.debug_looptime = 0;
    memset(&g.vario_graph, 0, sizeof(g.vario_graph));
    g.vgraph_idx = 0;
    osd_draw_boundary = { 0, 0, SCR_X_SIZE, SCR_Y_SIZE };
    g.pthy_redraw = true;
    g.launch_heading = NO_HEADING;
    g.detected_cell_count = 0; // 0 means not detected yet

    // Commented out for quick start, need to run at every powerup

    OSD256_init_font_tables();
    OSD256_init_bitmaps();

    OSD256_clear_screen(PTH_X, 0);
    OSD256_clear_screen(PTH_X, 1);

    // From now on we assume that extended OSD functions are enabled...
    tw_write_register(0x240, 0x01);

    unsigned long now;

    // As start we display page 0 and write to page 1 (to enusure flicker free display refresh)
    // OSD256_wr_page and set_display_page only influence PTH_X display path
    OSD256_wr_page = 1;
    OSD256_set_display_page(0);

    init_cli();

    // Main loop
    while (1)
    {
        now = millis();

        read_mavlink();
        read_cli();

        // Clear actual write page (not the one that is displayed)
        OSD256_clear_screen(PTH_X, OSD256_wr_page);

        // OSD256_displayscratch();

        // g.visible_osd_page = 0x01;
        // g.pip_page = 0x01;

        // Render OSD elements
        osd_boxes_render();

        if (osd.radar1.visible & g.visible_osd_page) osd_render_radar(&osd.radar1);
        if (osd.radar2.visible & g.visible_osd_page) osd_render_radar(&osd.radar2);
        if (osd.radar3.visible & g.visible_osd_page) osd_render_radar(&osd.radar3);
        if (osd.vgraph.visible & g.visible_osd_page) osd_render_vgraph(&osd.vgraph);
        if (osd.compass.visible & g.visible_osd_page) osd_render_compass(&osd.compass);
        if (osd.center_cross_visible & g.visible_osd_page) osd_render_center_marker();
        if (osd.move.visible & g.visible_osd_page) osd_render_move(&osd.move);
        if (osd.horizon.visible & g.visible_osd_page) osd_render_horizona(&osd.horizon);
        if (osd.gps.visible & g.visible_osd_page) osd_render_gps(&osd.gps);
        if (osd.stat.visible & g.visible_osd_page) osd_render_status(&osd.stat);
        if (osd.batt1_v.visible & g.visible_osd_page) osd_render_batt_volt(&osd.batt1_v, g.b1_voltage);
        if (osd.batt2_v.visible & g.visible_osd_page) osd_render_batt_volt(&osd.batt2_v, g.b2_voltage);
        if (osd.batt1_cap.visible & g.visible_osd_page) osd_render_batt_cap(&osd.batt1_cap, g.b1_remaining_capacity);
        if (osd.batt1_curr.visible & g.visible_osd_page) osd_render_batt_curr(&osd.batt1_curr, g.b1_current);
        if (osd.batt2_curr.visible & g.visible_osd_page) osd_render_batt_curr(&osd.batt2_curr, g.b2_current);
        if (osd.batt1_power.visible & g.visible_osd_page) osd_render_batt_power(&osd.batt1_power, g.b1_power);
        if (osd.batt2_power.visible & g.visible_osd_page) osd_render_batt_power(&osd.batt2_power, g.b2_power);
        if (osd.alt.visible & g.visible_osd_page) osd_render_altitude(&osd.alt);
        if (osd.vario.visible & g.visible_osd_page) osd_render_vario(&osd.vario);
        if (osd.home_w.visible & g.visible_osd_page) osd_render_home(&osd.home_w);
        if (osd.mode.visible & g.visible_osd_page) osd_render_flmode(&osd.mode);
        if (osd.pull.visible & g.visible_osd_page) osd_render_pull_force(&osd.pull);
        if (osd.msg_widget.visible & g.visible_osd_page) osd_render_message_buffer();
        if (osd.gs.visible & g.visible_osd_page) osd_render_groundspeed(&osd.gs);
        if (osd.thr.visible & g.visible_osd_page) osd_render_throttle(&osd.thr);
        if (osd.msg_list_widget.visible & g.visible_osd_page) osd_render_message_list();
        if (osd.rssi.visible & g.visible_osd_page) osd_render_rssi(&osd.rssi);
        if (osd.pt_widget.visible & g.visible_osd_page) osd_render_pt_indicator(&osd.pt_widget);
        if (osd.ekf_detail.visible & g.visible_osd_page) osd_render_ekf_detail(&osd.ekf_detail);
        if (osd.vibe_detail.visible & g.visible_osd_page) osd_render_vibe_detail(&osd.vibe_detail);
		if (osd.rc_out.visible & g.visible_osd_page) osd_render_rcout(&osd.rc_out);

        // Renders done, switch working page for smooth redraw
        OSD256_set_display_page(OSD256_wr_page);
        if (OSD256_wr_page == 0)
            OSD256_wr_page = 1;
        else
            OSD256_wr_page = 0;

        // The full redraw cycle has ended, we assume all PTHY redraws were done as well
        g.pthy_redraw = false;

        // Control channel 1 - Control PIP mode
        if (g.ctr_saved_state[0] != g.ctr_state[0])
        {
            // There is a change in ctr1 state
            g.ctr_saved_state[0] = g.ctr_state[0]; // Save it, to prevent unneccessary state changes in the main loop
            g.pip_page = g.ctr_state[0];

            for (uint8_t i = 1; i < 5; i++)
            {
                tw_ch_set_input(i, osd.video_channels[g.pip_page][i].input);

                tw_ch_settings(i, osd.video_channels[g.pip_page][i].enable, osd.video_channels[g.pip_page][i].popup);

                tw_ch_set_window(i, osd.video_channels[g.pip_page][i].pos_h, osd.video_channels[g.pip_page][i].pos_v, osd.video_channels[g.pip_page][i].len_h);
            }
        }

        // Control channel 2 - OSD widgets page

        if (g.ctr_saved_state[1] != g.ctr_state[1])
        {
            // There is a change in ctr1 state
            g.ctr_saved_state[1] = g.ctr_state[1]; // Save it, to prevent unneccessary state changes in the main loop
            // visible_osd_page is a bit coded value bit represents the one page (1 - page 1, 2-page 2, 4-page 3, 8-page 4.... up to page 5)
            g.visible_osd_page = 0x01 << g.ctr_state[1];
            g.pthy_redraw = true;          // force redraw of pth_y elements as well.
            OSD256_clear_screen(PTH_Y, 0); // And clean the pthy display
        }

        // check heartbeat
        heartbeat_validation();

        // Send eight params at once, to speed up parameter download time
        if (param_send_index != total_params)
        {
            for (int i = 0; i < 8; i++) send_param_list();
        }

        // Send out mavlink heartbeat
        if ((millis() - last_heartbeat_sent) >= 1000)
        {
            heartbeat_out();
            last_heartbeat_sent = millis();
        }

        if (millis() > (g.home.last_calc + HOME_CALC_INTERVAL)) calc_home();
        find_launch_heading();
        if ((osd.batt1_cap.max_capacity == 0) && (millis() > (g.last_capacity_query + 5000))) request_mavlink_battery_capacity();

        g.debug_looptime = false;
        if (g.debug_looptime) debug("Looptime : %lu\r\n", millis() - now);
    }
}
