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

#ifndef BITMAPS_H

#define	BITMAPS_H

struct bitmap_t
{
	unsigned int startx;			//x start position in SCRATCH buffer
	unsigned int starty;			//y start position in SCRATCH buffer
	unsigned int width;				//width in pixels
	unsigned int height;			//height in pixels
	char         color;				//color if the changeable color part
	const char * image;				//pointer to the bitmap

};


enum bitmap_names_t {
	BMP_GPS_ICON_GREEN = 0,
	BMP_GPS_ICON_RED,
	BMP_GPS_STATE_GREEN,
	BMP_GPS_STATE_YELLOW,
	BMP_GPS_STATE_RED,
	BMP_GPS_STATE_GRAY,
	BMP_EKF_STATE_GREEN,
	BMP_EKF_STATE_YELLOW,
	BMP_EKF_STATE_RED,
	BMP_EKF_STATE_GRAY,
	BMP_VIB_STATE_GREEN,
	BMP_VIB_STATE_YELLOW,
	BMP_VIB_STATE_RED,
	BMP_VIB_STATE_GRAY,
	BMP_BALL_WHITE,
	BMP_BALL_GREEN,
	BMP_BALL_RED,
	BMP_BALL_BLUE,
	BMP_GPS_ICON_ORANGE,
};

void OSD256_load_bitmap(uint8_t dst, uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height, uint8_t color, const char *bitmap);
void OSD256_init_font_tables();
void OSD256_init_bitmaps();
void OSD256_display_bitmap(bitmap_names_t bitmap_idx, unsigned int startx, unsigned int starty);

#endif
