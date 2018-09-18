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

#include "osdeluxe.h"

//Bitmap definition table, gives position size and color for a bitmap, uploaded to scratch memory

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
	{ 929, 40,32,20, COLOR_GREEN, vibe_state },				    // 10 BMP_VIB_STATE_GREEN
	{ 929, 60,32,20, COLOR_DARK_YELLOW, vibe_state },			// 11 BMP_VIB_STATE_YELLOW
	{ 929, 80,32,20, COLOR_RED | BLINK, vibe_state },			// 12 BMP_VIB_STATE_RED
	{ 929, 100,32,20, COLOR_25_WHITE, vibe_state },				// 13 BMP_VIB_STATE_GRAY
	{ 0,  300,16,15,    COLOR_WHITE, small_ball },				// 14 BMP_BALL_WHITE
	{ 16, 300,16,15,    COLOR_GREEN, small_ball },				// 15 BMP_BALL_GREEN
	{ 32, 300,16,15,    COLOR_RED, small_ball },				// 16 BMP_BALL_RED
	{ 48, 300,16,15,    COLOR_BLUE, small_ball },				// 17 BMP_BALL_BLUE
    { 945, 0, 40, 40, COLOR_ORANGE, gps_icon }					// 18 BMP_GPS_ICON_YELLOW
};


void OSD256_load_bitmap(uint8_t dst, uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height, uint8_t color, const char *bitmap)
{
    uint8_t reg40, tmp;
    uint16_t tmp1;

    tw_write_register(0x20a, 0x0); //... y path 0x20, x Path 0x00

    reg40 = tw_read_register(0x240);
    tw_write_register(0x240, (reg40 | 0x1)); // Enable extend OSD feature

    OSD256_OSG_Mode_Selection(1); // Set OSG operation mode to BMP operation

    tw_write_register(0x205, start_x);
    tmp1 = (start_x + width - 1);
    tw_write_register(0x206, tmp1);

    tmp = ((start_x & 0xff00) >> 8 << 6) | ((tmp1 & 0xff00) >> 8 << 4);
    tw_write_register(0x24e, tmp);
    tw_write_register(0x207, start_y);
    tmp1 = start_y + height - 1;

    tw_write_register(0x208, tmp1);

    tmp = ((start_y & 0xff00) >> 8 << 2) | ((tmp1 & 0xff00) >> 8);

    tw_write_register(0x209, tmp);

    // Set OSG_DSTCTL           0 scratch 1 display
    if (dst == 1)
        tmp = 0x4; // To Display buffer
    else
        tmp = 0; // scratch

    tw_write_register(0x24f, tmp | 0x1);

    for (tmp1 = 0; tmp1 < (height * width / 2); tmp1++)
    {
        uint8_t pix;

        pix = (bitmap[tmp1] & 0xf0) >> 4;

        if (pix == 0)
            pix = 0xff;
        else if (pix == 3)
            pix = color;
        else if (pix == 1)
            pix = COLOR_BLACK;
        else if (pix == 2)
            pix = COLOR_25_WHITE;
        else if (pix == 4)
            pix = COLOR_WHITE;

        tw_write_register(0x200, pix);

        pix = (bitmap[tmp1] & 0xf);
        if (pix == 0)
            pix = 0xff;
        else if (pix == 3)
            pix = color;
        else if (pix == 1)
            pix = COLOR_BLACK;
        else if (pix == 2)
            pix = COLOR_25_WHITE;
        else if (pix == 4)
            pix = COLOR_WHITE;
        tw_write_register(0x200, pix);

        WAIT_OSG_WRSTALL;
    }
    WAIT_OSG_IDLE;
    tw_write_register(0x240, reg40); // Restore the saved register 2x40 value
}


//Upload bitmaps to scratch memory

void OSD256_init_bitmaps()
{
	char tmp = sizeof(bitmaps) / sizeof(bitmap_t);			//get the number of bitmaps

	for (uint8_t i = 0; i < tmp; i++)
	{
		OSD256_load_bitmap(SCRATCH, bitmaps[i].startx, bitmaps[i].starty, bitmaps[i].width, bitmaps[i].height, bitmaps[i].color, bitmaps[i].image);
	}
}


//Create font tables in scratch memory.

void OSD256_init_font_tables()
{
	OSD256_printf_slow(18, 10, COLOR_WHITE, 0, "OSDeluxe");
	OSD256_printf_slow(9, 13, COLOR_WHITE, 0, "C");
	OSD256_printf_slow(10, 13, COLOR_BLUE, 0, "O");
	OSD256_printf_slow(11, 13, COLOR_GREEN, 0, "L");
	OSD256_printf_slow(12, 13, COLOR_RED, 0, "O");
	OSD256_printf_slow(13, 13, COLOR_YELLOW, 0, "R");

	OSD256_printf_slow(15, 13, COLOR_WHITE, 0, "PIP OSD for ARDU");
	OSD256_printf_slow(31, 13, COLOR_YELLOW, 0, "PILOT");

	OSD256_printf_slow(6, 15, COLOR_WHITE, 0, "firmware version %s %s",VERSION, BUILD);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 1/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_WHITE, 0, 1);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 610, 0, 865, 143);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 2/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_RED, 0, 1);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 610, 144, 865, 287);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 3/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_YELLOW, 0, 1);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 610, 288, 865, 431);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 4/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_RED, 0, 0);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 0, 60, 609, 119);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 5/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_RED, BLINK, 0);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 0, 120, 609, 179);

	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 6/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_YELLOW, 0, 0);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 0, 180, 609, 239);


	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 7/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_GREEN, 0, 0);
	OSD256_block_transfer(SCRATCH, SCRATCH, 0, 0, 0, 240, 609, 299);


	//This must be the last one
	OSD256_printf_slow(8, 19, COLOR_YELLOW | BLINK, 0, "Initializing font table 8/8...");
	OSD256_create_scratch_font_table(SCRATCH, COLOR_WHITE, 0, 0);

	delay(1000);


}

//Put a perviously stored bitmap from SCRATCH to display
void OSD256_display_bitmap(bitmap_names_t bitmap_idx, unsigned int startx, unsigned int starty)
{
	//debug("%u, %u, %u, %u, %u\n", bitmap_idx, startx, starty, bitmaps[bitmap_idx].startx, bitmaps[bitmap_idx].starty);

	OSD256_block_transfer(SCRATCH, DISPLAY,	bitmaps[bitmap_idx].startx, bitmaps[bitmap_idx].starty,
		startx, starty, startx + bitmaps[bitmap_idx].width - 1, starty + bitmaps[bitmap_idx].height - 1);
}