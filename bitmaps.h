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
	BMP_VIB_STATE_GRAY
};


void init_font_tables();
void init_bitmaps();
void OSD256_display_bitmap(bitmap_names_t bitmap_idx, unsigned int startx, unsigned int starty);

#endif
