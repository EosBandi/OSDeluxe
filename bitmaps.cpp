#include "osdeluxe.h"


struct bitmap_t bitmaps[] = {
	{ 0, 300, 40, 40, COLOR_GREEN, gps_icon },					// 0 BMP_GPS_ICON_GREEN
	{ 40, 300, 40, 40, COLOR_RED, gps_icon },					// 1 BMP_GPS_ICON_RED
	{ 80, 300,32,20, COLOR_GREEN, gps_state },					// 2 BMP_GPS_STATE_GREEN
	{ 80, 320,32,20, COLOR_DARK_YELLOW, gps_state },			// 3 BMP_GPS_STATE_YELLOW
	{ 80, 340,32,20, COLOR_RED | BLINK, gps_state },			// 4 BMP_GPS_STATE_RED
	{ 80, 360,32,20, COLOR_25_WHITE, gps_state },				// 5 BMP_GPS_STATE_GRAY
	{ 112, 300,32,20, COLOR_GREEN, ekf_state },					// 6 BMP_EKF_STATE_GREEN
	{ 112, 320,32,20, COLOR_DARK_YELLOW, ekf_state },			// 7 BMP_EKF_STATE_YELLOW
	{ 112, 340,32,20, COLOR_RED | BLINK, ekf_state },			// 8 BMP_EKF_STATE_RED
	{ 112, 360,32,20, COLOR_25_WHITE, ekf_state },				// 9 BMP_EKF_STATE_GRAY
	{ 144, 300,32,20, COLOR_GREEN, vibe_state },				// 10 BMP_VIB_STATE_GREEN
	{ 144, 320,32,20, COLOR_DARK_YELLOW, vibe_state },			// 11 BMP_VIB_STATE_YELLOW
	{ 144, 340,32,20, COLOR_RED | BLINK, vibe_state },			// 12 BMP_VIB_STATE_RED
	{ 144, 360,32,20, COLOR_25_WHITE, vibe_state },				// 13 BMP_VIB_STATE_GRAY
};


void init_bitmaps()
{

	char tmp = sizeof(bitmaps) / sizeof(bitmap_t);			//get the number of bitmaps

	for (char i = 0; i < tmp; i++)
	{
		OSD256_load_bitmap(SCRATCH, bitmaps[i].startx, bitmaps[i].starty, bitmaps[i].width, bitmaps[i].height, bitmaps[i].color, bitmaps[i].image);
	}


}


//Put a perviously stored bitmap from SCRATCH to display
void OSD256_display_bitmap(bitmap_names_t bitmap_idx, unsigned int startx, unsigned int starty)
{

	OSD256_Block_Transfer(SCRATCH, DISPLAY,	bitmaps[bitmap_idx].startx, bitmaps[bitmap_idx].starty,
		startx, starty, startx + bitmaps[bitmap_idx].width - 1, starty + bitmaps[bitmap_idx].height - 1);

}