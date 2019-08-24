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

#ifndef _TW_FUNC_H_
#define _TW_FUNC_H_


#define WAIT_OSG_WRSTALL		while(tw_read_register(0x241)&0x40)
#define WAIT_OSG_IDLE			while(!(tw_read_register(0x241)&0x80))
#define WAIT_INDR_DONE			while(tw_read_register(0x246)&0x1)

#define	PTH_X			BIT0//BIT1//
#define	PTH_Y			BIT1//BIT5//
#define	PTH_PB			BIT2//
#define	PTH_ALL			(PTH_X|PTH_Y)

#define SCR_X_SIZE (720-1)
#define SCR_Y_SIZE (576-1)

//OSD registers

#define COLOR_BLACK      0
#define COLOR_25_WHITE   14
#define COLOR_50_WHITE   15
#define COLOR_75_WHITE   16
#define COLOR_WHITE      17
#define COLOR_BLUE       5
#define COLOR_RED        6
#define COLOR_ORANGE     7
#define COLOR_CYAN       8
#define COLOR_MAGENTA    9
#define COLOR_GREEN      10
#define COLOR_OCEAN      11
#define COLOR_YELLOW     12
#define COLOR_DARK_YELLOW 13
#define COLOR_NONE       0xff


#define MIX  0x80
#define BLINK 0x40

#define OSD_COL_BLK     0x008080
#define OSD_COL_25WHT   0x3f8080
#define OSD_COL_50WHT   0x7f8080
#define OSD_COL_75WHT   0xc08080
#define OSD_COL_100WHT  0xf08080
#define OSD_COL_BLU     0x1dff6b
#define OSD_COL_RED     0x4c54ff
#define OSD_COL_ORNG    0x954fb5
#define OSD_COL_CYN     0xb2aa00
#define OSD_COL_MGN     0x69d4ea
#define OSD_COL_GRN     0x4b554a
#define OSD_COL_OCN     0x61b53a
#define OSD_COL_YLW     0xe10094

#define OSD_color_0	OSD_COL_BLK
#define OSD_color_1	OSD_COL_100WHT
#define OSD_color_2	OSD_COL_GRN
#define OSD_color_3	OSD_COL_RED

#define CH_ON 1
#define CH_OFF 0

#define CH_POPUP 1
#define CH_NO_POPUP 0


#define FLD_NONE 0
#define FLD_EVEN 1
#define FLD_ODD  2
#define FLD_BOTH 3

#define SCRATCH 0
#define DISPLAY 1


enum input_channel {
    INPUT_CH_1 = 0x00,
    INPUT_CH_2 = 0x40,
    INPUT_CH_3 = 0x80,
    INPUT_CH_4 = 0xC0
};



struct channel_setting_t
{
    char input;
    char enable;
    char popup;
	unsigned int pos_h;
	unsigned int pos_v;
	unsigned int len_h;

};

struct vin_params_t
{
	char sharpening;
	char saturation;
	char contrast;
	char brightness;
	char enhance;
	char h_mirror;
	char v_mirror;
	char boundary;
	char peaking;
};

struct boundary_t {
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
};


struct point_t {
    int x, y;
};

struct point3d_t {
    int x, y, z;
};

struct polygon_t {
    struct point_t *points;
    uint8_t len;
};



extern boundary_t osd_draw_boundary;
extern uint8_t OSD256_wr_page;
extern struct osd_settings osd;  


// BUG Workaround
// TW2837 extended OSD graphic mode, does not handle colot table indexes 1,2,3,4 So 
// White colors are moved to 14-15-16-17

const uint8_t colortable[18][3] = {
{0x00, 0x80, 0x80},							//Black         0
{0x3f, 0x80, 0x80},							//25%white      1
{0x7f, 0x80, 0x80},							//50%white      2
{0xc0, 0x80, 0x80},							//75%white      3
{0xf0, 0x80, 0x80},							//100%white     4
{0x1d, 0xff, 0x6b},							//Blue          5
{110, 97, 218},								//Red           6   
{0x75, 0x4f, 0xb5},							//Orange        7
{0xb2, 0xaa, 0x00},							//Cyan          8
{0x69, 0xd4, 0xea},							//Magenta       9
{120,70,52},								//GREEn			10
{0x61, 0xb5, 0x3a},							//Ocean BLUE      11
{0xe1, 0x00, 0x94},							//Yellow        12
{190, 32, 168},                              //Dark Yellow   13
{0x3f, 0x80, 0x80},							//25%white      14
{ 0x7f, 0x80, 0x80 },						//50%white      15
{ 0xc0, 0x80, 0x80 },						//75%white      16
{ 0xf0, 0x80, 0x80 }						//100%white     17

};

const uint8_t colortable_rec[4][3] = {
{0x00, 0x80, 0x80},							//Black         
{0xf0, 0x80, 0x80},							//100%white     
{0x7f, 0x80, 0x80},							//50%white              
{0x3f, 0x80, 0x80}							//25%white         
};

#define COLOR_REC_BLACK  0x00
#define COLOR_REC_WHITE  0x11
#define COLOR_REC_50_WHITE   0x22
#define COLOR_REC_25_WHITE   0x33

#define COLOR_REC_NONE  0xff
#define REC_MIX         0x88
#define REC_BLINK       0x44


#define OSD256_FONT_WHITE     0
#define OSD256_FONT_RED       1
#define OSD256_FONT_RED_BLINK 2
#define OSD256_FONT_YELLOW    3
#define OSD256_FONT_GREEN     4
#define OSD256_FONT_BLUE	  5



// tw2837 low level functions
void tw_init();
void tw_write_buf(uint16_t wrADDR, uint8_t *wrBUF, uint8_t wrCNT);
void tw_write_register(unsigned int wrADDR, uint8_t content);
uint8_t tw_read_register(unsigned int rdADDR);
uint8_t tw_read_register_bit(unsigned int rdADDR, uint8_t _flg);
void tw_write_register_bit(unsigned int wrADDR, uint8_t _flg, uint8_t _data);
void tw_ch_set_window (uint8_t _ch, unsigned int _pos_H, unsigned int _pos_V, unsigned int _len_H);
void tw_ch_settings (uint8_t _ch, uint8_t _on_off, uint8_t _popup);
void tw_ch_set_input(char ch, char input);


//TW2837 256color OSD functions

void OSD256_set_display_page(char rd_page);
void OSD256_clear_screen(uint8_t _pth, uint8_t page);
void OSD256_OSG_Mode_Selection(uint8_t mode);

void OSD256_write_font0(uint8_t dst, uint8_t _pos_x, uint16_t _pos_y, uint8_t _indx, uint8_t color, uint8_t attrib);
void OSD256_write_font1(uint8_t dst, uint8_t _pos_x, uint16_t _pos_y, uint8_t _indx, uint8_t color, uint8_t attrib);

void OSD256_block_fill(uint8_t _pth, uint8_t dst, uint16_t start_X, uint16_t start_Y, uint16_t end_X, uint16_t end_Y, uint8_t color);
void OSD256_block_transfer(uint8_t src, uint8_t dst, uint16_t src_start_x, uint16_t src_start_y, uint16_t dst_start_x, uint16_t dst_start_y, uint16_t dst_end_x, uint16_t dst_end_y);

void OSD256_putc(uint16_t _pos_x, uint16_t _pos_y, uint8_t _indx, uint8_t color, uint8_t font);
void OSD256_puts(char *str, uint16_t posx, uint16_t posy, uint8_t color);
void OSD256_printf(uint16_t posx, uint16_t posy, char color, char font, const char *format, ...);
void OSD256_printf_slow(uint16_t posx, uint16_t posy, char color, char font, const char *format, ...);

void OSD256_set_drawcolor(uint8_t color);
void OSD256_setpixel(uint8_t _pth, uint8_t color, uint16_t start_X, uint16_t start_Y);
void OSD256_setpixel_fast(uint8_t _pth, uint16_t start_x, uint16_t start_Y);
void OSD256_drawline(uint8_t _pth, uint8_t color, int x, int y, int x2, int y2);
void OSD256_circle(uint8_t _pth, uint8_t color, int  xCenter, int yCenter, int radius);
void OSD256_box(uint8_t _pth, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void OSD256_angled_line(uint8_t _pth, uint16_t center_x, uint16_t center_y, uint16_t length, uint16_t angle);



void OSD256_transform_polygon(struct polygon_t *p, int x, int y, int rot);
void OSD256_move_polygon(struct polygon_t *p, int x, int y);
void OSD256_draw_polygon(struct polygon_t *p, char color);
void OSD256_scale_polygon(struct polygon_t *p, float scale);

void OSD256_create_scratch_font_table(uint8_t dst, uint8_t color, uint8_t attrib, uint8_t font);
void OSD256_displayscratch();


#endif
