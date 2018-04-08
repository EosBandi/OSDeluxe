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


#define	BIT0		(0x0001)
#define	BIT1		(0x0002)
#define	BIT2		(0x0004)
#define	BIT3		(0x0008)
#define	BIT4		(0x0010)
#define	BIT5		(0x0020)
#define	BIT6		(0x0040)
#define	BIT7		(0x0080)

#define	SetBit(x,y)			((x) |= (y))         
#define	ClearBit(x,y)		((x) &= ~(y))        
#define	BitSet(x,y)			(((x)&(y))== (y))    
#define	BitClear(x,y)		(((x)&(y))== 0)      
#define	IfBitSet(x,y)		if(((x)&(y)) == (y)) 
#define	IfBitClear(x,y)	if(((x)&(y)) == 0)

#define	PTH_X			BIT0//BIT1//
#define	PTH_Y			BIT1//BIT5//
#define	PTH_PB			BIT2//
#define	PTH_ALL			(PTH_X|PTH_Y)

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

#define OSD_PATH_DISP 0
#define OSD_PATH_REC  1

#define SCRATCH 0
#define DISPLAY 1

#define SCREEN_SCALE 1.6f

enum FontType {
	FONT_8x8 = 0x01,
	FONT_16x8 = 0x02,
	FONT_16x16 = 0x03,
	FONT_SHADOW_8x8 = 0x04,
	FONT_SHADOW_16x8 = 0x05,
	FONT_SHADOW_16x16 = 0x06,
	FONT_OUTLINE_8x12 = 0x07,
	FONT_OUTLINE_16x12 = 0x08,
	FONT_OUTLINE_16x12c = 0x09,
	FONT_8x8b = 0x0A,
	FONT_QUICK = 0x0b
};

extern FontType font_type;      //Global variable to hold current font type

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





extern unsigned char cnt, count, data_buf[20];
extern unsigned int  ADDR_buf, count_TW2835;
extern unsigned char OSD_work_field, OSD_display_field;
extern unsigned char OSD_path; //0-display, 1-record
extern unsigned char rec_color_shadow, rec_color, rec_color_background;
extern unsigned char disp_color, disp_color_background, disp_color_shadow;

extern unsigned char OSD256_font_color;
extern unsigned char OSD256_wr_page;


extern struct osd_settings osd;  


// BUG Workaround
// TW2837 extended OSD graphic mode, does not handle colot table indexes 1,2,3,4 So 
// White colors are moved to 14-15-16-17

const unsigned char colortable[18][3] = {
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

const unsigned char colortable_rec[4][3] = {
{0x00, 0x80, 0x80},							//Black         0
{0xf0, 0x80, 0x80},							//100%white     4
{110, 97, 218},							//Red           6   
{0x3f, 0x80, 0x80}							//Green         10
};

#define COLOR_REC_BLACK 0
#define COLOR_REC_WHITE 1
#define COLOR_REC_RED   2
#define COLOR_REC_GREEN 3
#define COLOR_REC_NONE  0xf
#define REC_MIX         0x8
#define REC_BLINK       0x4


#define OSD256_FONT_WHITE     0
#define OSD256_FONT_RED       1
#define OSD256_FONT_RED_BLINK 2
#define OSD256_FONT_YELLOW    3
#define OSD256_FONT_GREEN     4
#define OSD256_FONT_BLUE	  5



void tw_init();
void tw_write_buf(unsigned int wrADDR, unsigned char *wrBUF, unsigned char wrCNT);


void tw_osd_out_char(unsigned char _pos_X, unsigned int _pos_Y, unsigned char _chr);
void tw_puts(char *str, unsigned short posx, unsigned short posy);
void tw_wr_osd_buffer(char *buf, char address);
void tw_wr_display_from_buffer(unsigned char _pos_X, unsigned int _pos_Y, char length);

void OSD_Circle(int  xCenter, int yCenter, int radius, unsigned char color, unsigned char _field);

void OSD_Line(unsigned int  x1, unsigned int y1, unsigned int  x2, unsigned int  y2, unsigned char color, unsigned char _field);


void tw_printf(char posx, unsigned short posy, const char *format, ...);

//void TW2823_OSD_fill_region_pre (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char color, unsigned char _field);
void tw_set_osd_buf(char b1, char b2, char b3, char b4);

//void tw_osd_drawline(unsigned char x1, unsigned short y1, unsigned char x2, unsigned short y2);
void tw_osd_drawline(int x, int y, int x2, int y2);
void tw_osd_drawline256(int x, int y, int x2, int y2);

void tw_write_register(unsigned int wrADDR, unsigned char content);
unsigned char tw_read_register(unsigned int rdADDR);
void tw_wait_for_osd_write(int timeout);

void tw_ch_set_window (unsigned char _ch, unsigned int _pos_H, unsigned int _pos_V, unsigned int _len_H);
void tw_ch_settings (unsigned char _ch, unsigned char _on_off, unsigned char _popup);
void tw_set_ch_input(char ch, char input);

void tw_osd_set_display_field( char dp_field);
void tw_osd_set_rec_field( char rec_field);
void tw_osd_set_display_page(char rd_page);
void tw_switch_display_field();


void tw_clear_all_pages(void);

void tw_osd_qsetpixel(unsigned int x, unsigned int y);
void tw_osd_setpixel(unsigned int x, unsigned int y, unsigned char color1, unsigned char color2, unsigned char color3, unsigned char color4);

void tw_osd_fill_region(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2, unsigned char color, unsigned char _field, unsigned char path, unsigned char rd_page);

void tw_osd_rectangle(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char color);


void tw_ext_set_pos_registers(unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y);
void tw_ext_block_move_scratch_to_osd(unsigned int hpos, unsigned int vpos, unsigned int width, unsigned int height, unsigned int src_hpos, unsigned int src_vpos);

void tw_ext_putchar(unsigned int x, unsigned int y, char chr);
void init_scratch_memory();

void WriteOSD256Fnt0(unsigned char dst, unsigned char _pos_x, unsigned int _pos_y, unsigned char _indx, U8 color, U8 attrib);
void WriteOSD256Fnt1(unsigned char dst, unsigned char _pos_x, unsigned int _pos_y, unsigned char _indx, U8 color, U8 attrib);

void OSD256_Block_fill(U8 _pth, U8 dst, U16 start_X, U16 start_Y, U16 end_X, U16 end_Y, U8 color);

void OSD256_Block_Transfer(U8 src, U8 dst, U16 src_start_x, U16 src_start_y, U16 dst_start_x, U16 dst_start_y, U16 dst_end_x, U16 dst_end_y);
void OSD256_putc(U16 _pos_x, U16 _pos_y, U8 _indx, U8 color, U8 font);

void OSD256_puts(char *str, unsigned short posx, unsigned short posy, unsigned char color);
void OSD256_printf(unsigned short posx, unsigned short posy, char color, char font, const char *format, ...);

void OSD256_printf_slow(unsigned short posx, unsigned short posy, char color, char font, const char *format, ...);

void OSD256_clear_screen(U8 page);

void OSD256_set_drawcolor(U8 color);
void OSD256_setpixel(U8 _pth, U8 color, U16 start_X, U16 start_Y);
void OSD256_setpixel_fast(U16 start_x, U16 start_Y);
void OSD256_drawline(U8 _pth, U8 color, int x, int y, int x2, int y2);
void OSD256_Circle(int  xCenter, int yCenter, int radius);
void OSD256_box(char _pth, unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char color);

void OSD256_load_bitmap(U8 dst, U16 start_x, U16 start_y, U16 width, U16 height, U8 color, const char *bitmap);

unsigned char tw_read_register_bit(unsigned int rdADDR, unsigned char _flg);
void tw_write_register_bit(unsigned int wrADDR, unsigned char _flg, unsigned char _data);

void CreateScrathFntTab(unsigned char dst, U8 color, U8 attrib, U8 font);





#endif
