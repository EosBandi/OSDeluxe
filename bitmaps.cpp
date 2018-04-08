#include "osdeluxe.h"


struct bitmap_t bitmaps[] = {
	{ 865, 0, 40, 40, COLOR_GREEN, gps_icon },					// 0 BMP_GPS_ICON_GREEN
	{ 905, 0, 40, 40, COLOR_RED, gps_icon },					// 1 BMP_GPS_ICON_RED
	{ 865, 40,32,20, COLOR_GREEN, gps_state },					// 2 BMP_GPS_STATE_GREEN
	{ 865, 60,32,20, COLOR_DARK_YELLOW, gps_state },			// 3 BMP_GPS_STATE_YELLOW
	{ 865, 80,32,20, COLOR_RED | BLINK, gps_state },			// 4 BMP_GPS_STATE_RED
	{ 865, 100,32,20, COLOR_25_WHITE, gps_state },				// 5 BMP_GPS_STATE_GRAY
	{ 897, 40,32,20, COLOR_GREEN, ekf_state },					// 6 BMP_EKF_STATE_GREEN
	{ 897, 60,32,20, COLOR_DARK_YELLOW, ekf_state },			// 7 BMP_EKF_STATE_YELLOW
	{ 897, 80,32,20, COLOR_RED | BLINK, ekf_state },			// 8 BMP_EKF_STATE_RED
	{ 897, 100,32,20, COLOR_25_WHITE, ekf_state },				// 9 BMP_EKF_STATE_GRAY
	{ 929, 40,32,20, COLOR_GREEN, vibe_state },				// 10 BMP_VIB_STATE_GREEN
	{ 929, 60,32,20, COLOR_DARK_YELLOW, vibe_state },			// 11 BMP_VIB_STATE_YELLOW
	{ 929, 80,32,20, COLOR_RED | BLINK, vibe_state },			// 12 BMP_VIB_STATE_RED
	{ 929, 100,32,20, COLOR_25_WHITE, vibe_state },				// 13 BMP_VIB_STATE_GRAY
};


void init_bitmaps()
{
	char tmp = sizeof(bitmaps) / sizeof(bitmap_t);			//get the number of bitmaps

	for (char i = 0; i < tmp; i++)
	{
		OSD256_load_bitmap(SCRATCH, bitmaps[i].startx, bitmaps[i].starty, bitmaps[i].width, bitmaps[i].height, bitmaps[i].color, bitmaps[i].image);
		debug("Bitmap %u init..\n", i);
	}
}


void init_font_tables()
{


	OSD256_printf_slow(18, 10, COLOR_WHITE, 0, "OSDeluxe");
	OSD256_printf_slow(9, 13, COLOR_WHITE, 0, "C");
	OSD256_printf_slow(10, 13, COLOR_BLUE, 0, "O");
	OSD256_printf_slow(11, 13, COLOR_GREEN, 0, "L");
	OSD256_printf_slow(12, 13, COLOR_RED, 0, "O");
	OSD256_printf_slow(13, 13, COLOR_YELLOW, 0, "R");

	OSD256_printf_slow(15, 13, COLOR_WHITE, 0, "PIP OSD for ARDU");
	OSD256_printf_slow(31, 13, COLOR_YELLOW, 0, "PILOT");

	OSD256_printf_slow(6, 15, COLOR_WHITE, 0, "firmware version 1.0 build 12345");


	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 1/8...");
	CreateScrathFntTab(SCRATCH, COLOR_WHITE, 0, 1);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 610, 0, 865, 143);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 2/8...");
	CreateScrathFntTab(SCRATCH, COLOR_RED, 0, 1);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 610, 144, 865, 287);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 3/8...");
	CreateScrathFntTab(SCRATCH, COLOR_YELLOW, 0, 1);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 610, 288, 865, 431);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 4/8...");
	CreateScrathFntTab(SCRATCH, COLOR_RED, 0, 0);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 0, 60, 609, 119);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 5/8...");
	CreateScrathFntTab(SCRATCH, COLOR_RED, BLINK, 0);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 0, 120, 609, 179);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 6/8...");
	CreateScrathFntTab(SCRATCH, COLOR_YELLOW, 0, 0);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 0, 180, 609, 239);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 7/8...");

	CreateScrathFntTab(SCRATCH, COLOR_GREEN, 0, 0);
	OSD256_Block_Transfer(SCRATCH, SCRATCH, 0, 0, 0, 240, 609, 299);

	//This must be the last one
	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 8/8...");
	CreateScrathFntTab(SCRATCH, COLOR_WHITE, 0, 0);




}


//Put a perviously stored bitmap from SCRATCH to display
void OSD256_display_bitmap(bitmap_names_t bitmap_idx, unsigned int startx, unsigned int starty)
{

	//debug("%u, %u, %u, %u, %u\n", bitmap_idx, startx, starty, bitmaps[bitmap_idx].startx, bitmaps[bitmap_idx].starty);

	OSD256_Block_Transfer(SCRATCH, DISPLAY,	bitmaps[bitmap_idx].startx, bitmaps[bitmap_idx].starty,
		startx, starty, startx + bitmaps[bitmap_idx].width - 1, starty + bitmaps[bitmap_idx].height - 1);

}