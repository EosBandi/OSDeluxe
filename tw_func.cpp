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

#include "OSDeluxe.h"


unsigned char cnt, count, data_buf[20];
unsigned int ADDR_buf, count_TW2835;
unsigned char OSD_work_field, OSD_display_field;
unsigned char OSD_path = 0;
unsigned char rec_color_shadow, rec_color, rec_color_background;
unsigned char disp_color_shadow, disp_color, disp_color_background;



FontType font_type; // Global variable to hold current font_type

void tw_init()
{


	//Output video format selection
	//bit 7 - 1-50Hz, 625 line
	tw_write_register(0x100, 0x80);

	//Start memory init (SDRAM)
	tw_write_register(0x17f, 0x80);

	//Enable black background for channels with no video
	tw_write_register(0x0c3, 0xf0);





	static unsigned char pg1_a0_video_path[] =
	{
		0x60,		// a0  bit67-INX - bit54-INY 
		0x11,		// a1  1 CBVS output for both VAOYX and VAOCX DAV
		0x90,		// a2  1 CVBS display path for VAOYY (bit7 must be 1)
		0x00,		// a3  Digital output setting (0 disable)
		0x80,		// a4  Enable Master mode 
		0x20,		// a5  Vertical sync delay (0x20 - default)
		0x00,		// a6  Horizontal sync delay (0x00 - default)
		0x0c,		// a7  Active video delay (0x0c - default)
		0x20,		// a8  ACTIVE_HDELAY (0x20 - default)
		0x4d,		// a9  bit7-6 color subcarrier (1 for pal), bi54-0, bit3-1, 
		0xaa,		// aa  Video encode chrominance bandwidth
		0x00		// ab  No color kill and no test bar
	};

	tw_write_buf(0x1a0, pg1_a0_video_path, sizeof(pg1_a0_video_path));

	//video decoder settings Same for all channels
	static unsigned char pg0_00_decoder_settings[] =
	{   0x00,		//00	Read only, no need   
		0x00,		//01	Read only, no need ? Last three bit Vertical peaking level, 0 default
		0x0a,		//02	Hdelay
		0xd0,		//03    HActive
		0x05,		//04    Vdelay
		0x20,		//05    VActive
		0x28,		//06    Extra bits for the four above
		0x00,		//07    Hue for NTSC system (no need on PAL)
		0x3f,       //08    Bit7 NTSC SCURVE, bit6 Internal, bit5-4 CTI level (1h) bit3-0 sharpnedd 
		0x64,		//09	Contrast 0x80 is default
		0x00,		//0a    Brightness 0x80 is no changes
		0x50,		//0b    Saturation Cb
		0x50,		//0c    Saturation Cr
		0x00,		//0d    Read Only
		0x07		//0e    Standard Auto detection
	};


	tw_write_buf(0x000, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));
	tw_write_buf(0x010, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));
	tw_write_buf(0x020, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));
	tw_write_buf(0x030, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));


												//    |       ch1      ||      ch2       ||     ch3        ||     c4
												//	   0x60  0x61  0x62  0x63  0x64  0x65  0x66  0x67  0x68  0x69  0x6a  0x6b
	static unsigned char pg1_60_channel_settings[] = { 0x90, 0x02, 0x00, 0x91, 0x02, 0x00, 0x93, 0x02, 0x00, 0x94, 0x02, 0x00 };
	
	//tw_write_buf(0x160, pg1_60_channel_settings, sizeof(pg1_60_channel_settings));

	tw_write_register(0x105, 0b00000100); // bypass mode when video loss is detected


	//Input selection for each scaler in the display path
	tw_write_register(0x080, 0x00);   //ch0 scaler - VIN0 pin
	tw_write_register(0x090, 0x40);   //ch1 scaler - VIN1 pin
	tw_write_register(0x0a0, 0x80);   //ch2 scaler - VIN2 pin
	tw_write_register(0x0b0, 0xc0);   //ch3 scaler - VIN3 pin
		
	//Channel mode selection
	tw_write_register(0x110, 0x00);	  //channel 0 disable, no popup   bit7-enable, bit6-popup, bit1-0 As see below
	tw_write_register(0x118, 0x01);   //channel 1 disable, no popup
	tw_write_register(0x120, 0x02);   //channel 2 disable, no popup
	tw_write_register(0x128, 0x03);   //channel 3 disable, no popup

	//Channel settings
	// bit7 - Recall_en, bit6-freeze, bit5-H_mirror, bit4-v_mirror,bit3-enhance, bit2-blank, bit1-boundary, bit0-boundary blink 
	tw_write_register(0x111, 0x0a); //enhance and boundary
	tw_write_register(0x119, 0x0a);
	tw_write_register(0x121, 0x0a);
	tw_write_register(0x129, 0x0a);

    //Channel conversion registers
	// bi7-0, bit6-0, bit5-field to frame mode, bit4-DVR to normal mode, bit3-0 Recall address (must be 0)
	tw_write_register(0x112, 0x00); 
	tw_write_register(0x11A, 0x00);
	tw_write_register(0x122, 0x00);
	tw_write_register(0x12a, 0x00);

	//Channel strobe, do not change
    tw_write_register (0x116, 0x00);
    tw_write_register (0x11e, 0x01);
    tw_write_register (0x126, 0x02);
    tw_write_register (0x12e, 0x03);


	//Power down audio dac and adc
	tw_write_register(0x04c, 0b00110000);



	//Select OSD reading page and OSD overlay mode
    tw_write_register (0x20F, 0x0F);


    for (char i = 0; i < 18; i++)
    {
        ADDR_buf = 0x20B;
        cnt = 0;
        data_buf[cnt] = colortable[i][0];
        cnt++;
        data_buf[cnt] = colortable[i][1];
        cnt++;
        data_buf[cnt] = colortable[i][2];
        cnt++;
        data_buf[cnt] = 0x80 + i;
        cnt++;
        tw_write_buf (ADDR_buf, data_buf, cnt);
    }

    for (char i = 0; i < 4; i++)
    {
        ADDR_buf = 0x20B;
        cnt = 0;
        data_buf[cnt] = colortable_rec[i][0];
        cnt++;
        data_buf[cnt] = colortable_rec[i][1];
        cnt++;
        data_buf[cnt] = colortable_rec[i][2];
        cnt++;
        data_buf[cnt] = 0x80 + 64 +i;
        cnt++;
        tw_write_buf (ADDR_buf, data_buf, cnt);
    }



    tw_write_register (0x283, 0x05);


    tw_write_register (0x21f, 0b00010011); // bitmap opacity
    tw_write_register (0x221, 0b01010000); // Red for box 1
    tw_write_register (0x222, 0);
    tw_write_register (0x223, 180);
    tw_write_register (0x225, 144);


    // Enable overlays dual mode high priority
    tw_write_register (0x238, 0b00000101);

//    OSD_display_field = 1;
//    TW2823_OSD_display_on (OSD_display_field); // this switch OSD off
//    OSD_work_field = 1;
//    TW2823_OSD_fill_region (0, 0, 719, 287, 0xff, 1); // work

//    disp_color = COLOR_YELLOW;
//    disp_color_background = COLOR_BLACK | MIX;
}

void tw_set_attrib (char _color, char _bgcolor, char _fontsize)
{
    disp_color = _color;
    disp_color_background = _bgcolor;
    font_type = (FontType)_fontsize;
}

void tw_puts (char *str, unsigned short posx, unsigned short posy)
{

    char inc  = 0; //Character width in posx units (OSD PATH:4pixel, REC PATH:8pixel)
    char kf_a = 0; //Addtional krening in posx units (see above);
	unsigned short _posX = posx;




    switch (font_type)
    {
    case FONT_8x8:
    case FONT_SHADOW_8x8:
    case FONT_OUTLINE_8x12:
    case FONT_8x8b:
        if (OSD_path == 0) inc = 2; else inc = 1;
        break;
    case FONT_16x8:
    case FONT_SHADOW_16x8:
    case FONT_OUTLINE_16x12:
        if (OSD_path == 0) inc = 4; else inc = 2;
        break;
    case FONT_16x16:
    case FONT_SHADOW_16x16:
        if (OSD_path == 0) inc = 4; else inc = 2;
        break;
    case FONT_OUTLINE_16x12c:
        if (OSD_path == 0) inc = 3; else inc = 2;
        break;
	case FONT_QUICK:
		inc = 14;
		break;
    
    }

    for (char a = 0; a < strlen (str); a++)
    {
        if (font_type!=FONT_QUICK) tw_osd_out_char (_posX, posy, str[a]);
		else tw_ext_putchar(_posX, posy, str[a]);
        _posX = _posX + inc;
    }
}

void tw_printf (char posx, unsigned short posy, const char *format, ...)
{
    char buf[128];
    va_list args;
    va_start (args, format);
    vsnprintf (buf, sizeof (buf), format,
                args); // does not overrun sizeof(buf) including null terminator
    va_end (args);

    byte _posX = posx;
    tw_puts (buf, posx, posy);
}


/*****************************************************************************/

unsigned char tw_read_register (unsigned int rdADDR)
{
    unsigned char page, reg;
    unsigned char ret;

    page = rdADDR >> 8;
    reg = rdADDR & 0x00ff;
    Wire.beginTransmission (0x42);
    Wire.write (page);
    Wire.write (reg);
    Wire.endTransmission (I2C_NOSTOP);
    Wire.requestFrom (0x42, 1, I2C_STOP);
    ret = Wire.read ();
    return (ret);
}

void tw_write_register (unsigned int wrADDR, unsigned char content)
{
    unsigned char page, reg;

    page = wrADDR >> 8;
    reg = wrADDR & 0x00ff;
    Wire.beginTransmission (0x42);
    Wire.write (page);
    Wire.write (reg);
    Wire.write (content);
    Wire.endTransmission ();
}

void tw_write_buf (unsigned int wrADDR, unsigned char *wrBUF, unsigned char wrCNT)
{

    unsigned char page;
    unsigned char reg;
    unsigned char i;

    page = wrADDR >> 8;
    reg = wrADDR & 0x00FF;

    Wire.beginTransmission (0x42);
    Wire.write (page);
    Wire.write (reg);
    for (i = 0; i < wrCNT; i++)
    {
        Wire.write (wrBUF[i]);
    }
    Wire.endTransmission ();
}


void tw_wait_for_osd_write(int timeout)
{
 
 unsigned char reg;
 int count = 0 ;

_rep:
 reg = tw_read_register(0x20A);
 count++;
 if((reg & 0x80) && (count < timeout)) goto _rep ;


}


void tw_osd_set_display_field( char dp_field)
{
  char reg20f;
  
  reg20f = tw_read_register(0x20f);
  reg20f = reg20f & 0b11110011;
  reg20f = reg20f + (dp_field << 2);
  //reg20f = reg20f + (rec_field);
  tw_write_register(0x20f,reg20f);

}

void tw_osd_set_rec_field(char rec_field)
{
  char reg20f;
  
  reg20f = tw_read_register(0x20f);
  reg20f = reg20f & 0b11111100;
//  reg20f = reg20f + (dp_field << 2);
  reg20f = reg20f + (rec_field);
  tw_write_register(0x20f,reg20f);

}

/*
void tw_osd_set_display( char dp_field, char rec_field )
{
 
  char reg20f;
  
  reg20f = tw_read_register(0x20f);

  reg20f = reg20f & 0xf0;
  reg20f = reg20f + (dp_field << 2);
  reg20f = reg20f + (rec_field);
  tw_write_register(0x20f,reg20f);

}
*/

void tw_osd_set_display_page(char rd_page)
{
 char reg20f = 0;

 reg20f = tw_read_register(0x20f);
 reg20f = reg20f & 0x0f;
 reg20f = reg20f + (rd_page << 4);
 tw_write_register(0x20f,reg20f);
}

void tw_switch_display_field()
{
    if (OSD_work_field == FLD_EVEN) OSD_work_field = FLD_ODD;
    else OSD_work_field = FLD_EVEN;
}

void tw_set_osd_buf(char b1, char b2, char b3, char b4)
{
    cnt = 0;
    data_buf[cnt++] = b1;
    data_buf[cnt++] = b2;
    data_buf[cnt++] = b3;
    data_buf[cnt++] = b4;
    data_buf[cnt++] = 0x80;
    tw_write_buf (0x200, data_buf,cnt);

}

// Put four byte from the buf to the OSD_BUFFER starting at address
void tw_wr_osd_buffer (char *buf, char address)
{
    cnt = 0;
    data_buf[cnt++] = buf[0];
    data_buf[cnt++] = buf[1];
    data_buf[cnt++] = buf[2];
    data_buf[cnt++] = buf[3];
    data_buf[cnt++] = 0x80 + address;
    tw_write_buf (0x200, data_buf, cnt);
}

// copy length 32bit quartetts from osd_buf to display
void tw_wr_display_from_buffer (unsigned char _pos_X, unsigned int _pos_Y, char length)
{
    cnt = 0;
    data_buf[cnt++] = _pos_X;   //205
    data_buf[cnt++] = _pos_X;   //206
    data_buf[cnt++] = _pos_Y;   //207
    data_buf[cnt++] = _pos_Y;   //208

    if (OSD_work_field == FLD_EVEN)
        data_buf[cnt] = (( _pos_Y & 0x0f00) >> 6) + 0x08; // field 1
    else
        data_buf[cnt] = (( _pos_Y & 0x0f00) >> 6); // field 0

    data_buf[cnt++] += (length << 4);   //209

    if (OSD_path == OSD_PATH_DISP)
        data_buf[cnt++] = 0x80; // 20A
    else
        data_buf[cnt++] = 0b10100000;
    tw_write_buf (0x205, data_buf, cnt);
}


void tw_osd_setpixel (unsigned int x, unsigned int y, unsigned char color1,unsigned char color2,unsigned char color3,unsigned char color4)
{

    unsigned char temp_reg;

    //y = (float)y * SCREEN_Y_SCALE;
    x = (float)x / SCREEN_SCALE;
    tw_set_osd_buf(color1, color2, color3, color4);
    tw_wr_display_from_buffer(x, y, 0);
}

//copy the content of the first four byte from osd buffer to display, keeps the osd buffer intact.
void tw_osd_qsetpixel (unsigned int x, unsigned int y)
{

    unsigned char reg209;

    //y = (float)y*SCREEN_Y_SCALE;
    x = (float)x/SCREEN_SCALE;
    cnt = 0;
    data_buf[cnt++] = x; // start HPOS                 //0
    data_buf[cnt++] = x; // end   HPOS                 //1    
    data_buf[cnt++] = y; // start VPOS                      
    data_buf[cnt++] = y; // end VPOS

    reg209 = 0x00;

    if (OSD_work_field == FLD_EVEN) reg209 = 0b00001000;
    if (y > 255) reg209 = reg209 +  0b00000100;

    data_buf[cnt++] = reg209;
    if (OSD_path == OSD_PATH_DISP)
        data_buf[cnt++] = 0b11000000;
    else
        data_buf[cnt++] = 0b11100000;

    tw_write_buf (0x205, data_buf, cnt);

}

void tw_osd_drawline (int x, int y, int x2, int y2)
{

    bool yLonger = false;
    int shortLen = y2 - y;
    int longLen = x2 - x;
    if (abs (shortLen) > abs (longLen))
    {
        int swap = shortLen;
        shortLen = longLen;
        longLen = swap;
        yLonger = true;
    }
    int decInc;
    if (longLen == 0)
        decInc = 0;
    else
        decInc = (shortLen << 16) / longLen;

    if (yLonger)
    {
        if (longLen > 0)
        {
            longLen += y;
            for (int j = 0x8000 + (x << 16); y <= longLen; ++y)
            {
                tw_osd_qsetpixel (j >> 16, y);
                j += decInc;
            }
            return;
        }
        longLen += y;
        for (int j = 0x8000 + (x << 16); y >= longLen; --y)
        {
            tw_osd_qsetpixel (j >> 16, y);
            j -= decInc;
        }
        return;
    }

    if (longLen > 0)
    {
        longLen += x;
        for (int j = 0x8000 + (y << 16); x <= longLen; ++x)
        {
            tw_osd_qsetpixel (x, j >> 16);
            j += decInc;
        }
        return;
    }
    longLen += x;
    for (int j = 0x8000 + (y << 16); x >= longLen; --x)
    {
        tw_osd_qsetpixel (x, j >> 16);
        j -= decInc;
    }
}


void tw_osd_rectangle(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned char color)
{

    unsigned char m16;

    m16 = w % 16;

    if ((m16 == 10) || (m16 == 12) || (m16 == 13))
    {
        tw_osd_fill_region(x, y, x + w - 5, y + h, color, OSD_work_field, OSD_path,0);
        tw_osd_fill_region(x + w - 5, y, x + w, y + h, color, OSD_work_field, OSD_path,0);
    }
    else { tw_osd_fill_region(x, y, x + w, y + h, color, OSD_work_field, OSD_path,0); }
}


void tw_osd_fill_region (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char disp_color, unsigned char _field, unsigned char path, unsigned char rd_page)
{
    unsigned char reg209;
    unsigned char reg20a;


    cnt = 0;
    data_buf[cnt++] = disp_color;
    data_buf[cnt++] = disp_color;
    data_buf[cnt++] = disp_color;
    data_buf[cnt++] = disp_color;
    data_buf[cnt++] = 0x80;
    tw_write_buf(0x200, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = x1; // start HPOS
    data_buf[cnt++] = x2; // end   HPOS
    data_buf[cnt++] = y1; // start VPOS
    data_buf[cnt++] = y2; // end VPOS

    reg209 = 0x00;

    if (_field == FLD_EVEN) reg209 = 0b00001000;
    if (y1 > 255) reg209 = reg209 + 0b00000100;
    if (y2 > 255) reg209 = reg209 + 0b00000001;

    data_buf[cnt++] = reg209;

    if (path == OSD_PATH_REC)
        reg20a = 0b11100000;
    else
        reg20a = 0b11000000;

    rd_page = rd_page << 2;
    rd_page = rd_page & 0b00011100;
    reg20a = reg20a + rd_page;

    data_buf[cnt++] = reg20a;

    tw_write_buf(0x205, data_buf, cnt);

    tw_wait_for_osd_write(50);
}

void tw_osd_out_char (unsigned char _pos_X, unsigned int _pos_Y, unsigned char _chr)
{

    unsigned char i, _i, _y;
    unsigned int _y_line;
    unsigned char _uch;
    unsigned int _shift;
    unsigned char x, y;
    char _buf[32];
    char s[32];
    unsigned char reg209;
    //char _field = 1;
    
    _y_line = _pos_Y;

    unsigned int offs;
    char col, row;
    char ch;
    char tmp;
    unsigned char lo, hi;


    if (font_type == FONT_OUTLINE_8x12)
    {

        ch = _chr-32; // Get char from translation map //First char is space
        row = ch / 16; // First line (16x6 table)
        col = ch % 16;

        for (_i = 0; _i < 12; _i++) //12 line per character
        {
            s[0] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4)];
            s[1] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4) + 1];
            s[2] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4) + 2];
            s[3] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4) + 3];
            if (OSD_path == OSD_PATH_DISP)
            {
                _buf[0] = (s[0] >> 4) & 0x0f;
                _buf[1] = (s[0]) & 0x0f;
                _buf[2] = (s[1] >> 4) & 0x0f;
                _buf[3] = (s[1]) & 0x0f;
                for (i = 0; i < 4; i++)
                {
                    switch (_buf[i])
                    {
                    case 0x00:
                        _buf[i] = disp_color_shadow;
                        break;
                    case 0x0d:
                        _buf[i] = disp_color_background;
                        break;
                    case 0x0f:
                        _buf[i] = disp_color;
                        break;
                    }
                }
                tw_wr_osd_buffer (_buf, 0);

                _buf[0] = (s[2] >> 4) & 0x0f;
                _buf[1] = (s[2]) & 0x0f;
                _buf[2] = (s[3] >> 4) & 0x0f;
                _buf[3] = (s[3]) & 0x0f;
                for (i = 0; i < 4; i++)
                {
                    switch (_buf[i])
                    {
                    case 0x00:
                        _buf[i] = disp_color_shadow;
                        break;
                    case 0x0d:
                        _buf[i] = disp_color_background;
                        break;
                    case 0x0f:
                        _buf[i] = disp_color;
                        break;
                    }
                }
                tw_wr_osd_buffer (_buf, 1);
                tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
            } 
            else 
            {
                for (i=0;i<4;i++)
                {
                    
                    tmp = 0;
                    if ( (s[i] & 0xf0) == 0x00) tmp = rec_color_shadow<<4;
                    if ( (s[i] & 0xf0) == 0xf0) tmp = rec_color<<4;
                    if ( (s[i] & 0xf0) == 0xd0) tmp = rec_color_background <<4 ;

                    if ( (s[i] & 0x0f) == 0x00) tmp += rec_color_shadow;
                    if ( (s[i] & 0x0f) == 0x0f) tmp += rec_color;
                    if ( (s[i] & 0x0f) == 0x0d) tmp += rec_color_background;

                    s[i]=tmp;
                }              
                tw_wr_osd_buffer (s, 0);
                tw_wr_display_from_buffer (_pos_X, _y_line, 0); // 0 is 4 bytes, 1 is 8 bytes
            }
            _y_line++;
        }
        return;

    }


    if (font_type == FONT_SHADOW_8x8)
    {
        ch = chmap[_chr]; // Get char from translation map
        row = ch / 16; // First line (16x6 table)
        col = ch % 16;

        for (_i = 0; _i < 8; _i++)
        {

            s[0] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4)];
            s[1] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4) + 1];
            s[2] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4) + 2];
            s[3] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4) + 3];

            if (OSD_path == OSD_PATH_DISP)
            {
                _buf[0] = (s[0] >> 4) & 0x0f;
                _buf[1] = (s[0]) & 0x0f;
                _buf[2] = (s[1] >> 4) & 0x0f;
                _buf[3] = (s[1]) & 0x0f;
                _buf[4] = (s[2] >> 4) & 0x0f;
                _buf[5] = (s[2]) & 0x0f;
                _buf[6] = (s[3] >> 4) & 0x0f;
                _buf[7] = (s[3]) & 0x0f;

                for (i = 0; i < 8; i++)
                {
                    switch (_buf[i])
                    {
                    case 0x00:
                        _buf[i] = disp_color_shadow;
                        break;
                    case 0x0f:
                        _buf[i] = disp_color_background;
                        break;
                    case 0x01:
                        _buf[i] = disp_color;
                        break;
                    }
                }


                tw_wr_osd_buffer (_buf, 0);
                tw_wr_osd_buffer (&_buf[4], 1);
                tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
            } 
            else 
            {
                for (i=0;i<4;i++)
                {
                    
                    tmp = 0;
                    if ( (s[i] & 0xf0) == 0x00) tmp = rec_color_shadow<<4;
                    if ( (s[i] & 0xf0) == 0x10) tmp = rec_color<<4;
                    if ( (s[i] & 0xf0) == 0xf0) tmp = rec_color_background <<4 ;

                    if ( (s[i] & 0x0f) == 0x00) tmp += rec_color_shadow;
                    if ( (s[i] & 0x0f) == 0x01) tmp += rec_color;
                    if ( (s[i] & 0x0f) == 0xf) tmp += rec_color_background;

                    s[i]=tmp;
                    
                }              
                tw_wr_osd_buffer (s, 0);
                tw_wr_display_from_buffer (_pos_X, _y_line, 0); // 0 is 4 bytes, 1 is 8 bytes
            }

            _y_line++;
        }
        return;
    }


    if (font_type == FONT_SHADOW_16x8)
    {
        ch = chmap[_chr]; // Get char from translation map
        row = ch / 16; // First line (16x6 table)
        col = ch % 16;

        for (_i = 0; _i < 8; _i++)
        {

            s[0] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4)];
            s[1] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4) + 1];
            s[2] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4) + 2];
            s[3] = fontxx[  (row*64*8) + ( _i* 64) + (col * 4) + 3];

        if (OSD_path == OSD_PATH_DISP)
        {
            _buf[0] = (s[0] >> 4) & 0x0f;
            _buf[1] = (s[0]) & 0x0f;
            _buf[2] = (s[1] >> 4) & 0x0f;
            _buf[3] = (s[1]) & 0x0f;
            _buf[4] = (s[2] >> 4) & 0x0f;
            _buf[5] = (s[2]) & 0x0f;
            _buf[6] = (s[3] >> 4) & 0x0f;
            _buf[7] = (s[3]) & 0x0f;

            for (i = 8; i > 0; i--)
            {
                switch (_buf[i-1])
                {
                case 0x00:
                    _buf[i-1] = disp_color_shadow;
                    break;
                case 0x0f:
                    _buf[i-1] = disp_color_background;
                    break;
                case 0x01:
                    _buf[i-1] = disp_color;
                    break;
                }
                _buf[(i-1)*2+1] = _buf[i-1];
                _buf[(i-1)*2]   = _buf[i-1];

            }

            tw_wr_osd_buffer (_buf, 0);
            tw_wr_osd_buffer (&_buf[4], 1);
            tw_wr_osd_buffer (&_buf[8], 2);
            tw_wr_osd_buffer (&_buf[12], 3);
            tw_wr_display_from_buffer (_pos_X, _y_line, 3); // 0 is 4 bytes, 1 is 8 bytes
        }
        else
        {

            for (i = 0; i < 4; i++)
            {
                tmp = 0;
                if ((s[i] & 0xf0) == 0x00) tmp = rec_color_shadow << 4;
                if ((s[i] & 0xf0) == 0x10) tmp = rec_color << 4;
                if ((s[i] & 0xf0) == 0xf0) tmp = rec_color_background << 4;

                if ((s[i] & 0x0f) == 0x00) tmp += rec_color_shadow;
                if ((s[i] & 0x0f) == 0x01) tmp += rec_color;
                if ((s[i] & 0x0f) == 0xf) tmp += rec_color_background;

                s[i] = tmp;
            }

            for (i=0;i<8;i=i+2)
            {

            _buf[i] = s[i/2] & 0xf0;
            _buf[i] = _buf[i] + (s[i/2] >> 4);

            _buf[i+1] = s[i/2] << 4;
            _buf[i+1] = _buf[i+1] + (s[i/2] & 0x0f);

            }
            tw_wr_osd_buffer(_buf, 0);
            tw_wr_osd_buffer(&_buf[4],1);
            tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
            
        }
            _y_line++;
        }
        return;
    }

 if ((font_type == FONT_OUTLINE_16x12) || (font_type == FONT_OUTLINE_16x12c) )
    {
        ch = _chr - 32; // First char is space
        row = ch / 16; // First line (16x6 table)
        col = ch % 16;

        for (_i = 0; _i < 12; _i++)
        {

            s[0] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4)];
            s[1] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4) + 1];
            s[2] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4) + 2];
            s[3] = fontoutline[  (row*64*12) + ( _i* 64) + (col * 4) + 3];

        if (OSD_path == OSD_PATH_DISP)
        {
            _buf[0] = (s[0] >> 4) & 0x0f;
            _buf[1] = (s[0]) & 0x0f;
            _buf[2] = (s[1] >> 4) & 0x0f;
            _buf[3] = (s[1]) & 0x0f;


            for (i = 0; i < 4; i++)
            {
                switch (_buf[i])
                {
                case 0x00:
                    _buf[i] = disp_color_shadow;
                    break;
                case 0x0d:
                    _buf[i] = disp_color_background;
                    break;
                case 0x0f:
                    _buf[i] = disp_color;
                    break;
                }
            }
            _buf[7] = _buf[3];
            _buf[6] = _buf[3];
            _buf[5] = _buf[2];
            _buf[4] = _buf[2];
            _buf[3] = _buf[1];
            _buf[2] = _buf[1];
            _buf[1] = _buf[0];


            tw_wr_osd_buffer (_buf, 0);
            tw_wr_osd_buffer (&_buf[4], 1);

            _buf[0] = (s[2] >> 4) & 0x0f;
            _buf[1] = (s[2]) & 0x0f;
            _buf[2] = (s[3] >> 4) & 0x0f;
            _buf[3] = (s[3]) & 0x0f;

            for (i = 0; i < 4; i++)
            {
                switch (_buf[i])
                {
                case 0x00:
                    _buf[i] = disp_color_shadow;
                    break;
                case 0x0d:
                    _buf[i] = disp_color_background;
                    break;
                case 0x0f:
                    _buf[i] = disp_color;
                    break;
                }
            }
            _buf[7] = _buf[3];
            _buf[6] = _buf[3];
            _buf[5] = _buf[2];
            _buf[4] = _buf[2];
            _buf[3] = _buf[1];
            _buf[2] = _buf[1];
            _buf[1] = _buf[0];

            tw_wr_osd_buffer (_buf, 2);
            tw_wr_osd_buffer (&_buf[4], 3);
            tw_wr_display_from_buffer (_pos_X, _y_line, 3); // 0 is 4 bytes, 1 is 8 bytes
        }
        else
        {

            // Redo values in s for colors

            for (i = 0; i < 4; i++)
            {

                tmp = 0;
                if ((s[i] & 0xf0) == 0x00) tmp = rec_color_shadow << 4;
                if ((s[i] & 0xf0) == 0xf0) tmp = rec_color << 4;
                if ((s[i] & 0xf0) == 0xd0) tmp = rec_color_background << 4;

                if ((s[i] & 0x0f) == 0x00) tmp += rec_color_shadow;
                if ((s[i] & 0x0f) == 0x0f) tmp += rec_color;
                if ((s[i] & 0x0f) == 0x0d) tmp += rec_color_background;

                s[i] = tmp;
            }
            for (i = 0; i < 8; i = i + 2)
            {

            _buf[i] = s[i/2] & 0xf0;
            _buf[i] = _buf[i] + (s[i/2] >> 4);

            _buf[i+1] = s[i/2] << 4;
            _buf[i+1] = _buf[i+1] + (s[i/2] & 0x0f);

            }
            tw_wr_osd_buffer(_buf, 0);
            tw_wr_osd_buffer(&_buf[4],1);
            tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
            
        }
            _y_line++;
        }
        return;
    }

    if (font_type == FONT_8x8 || font_type == FONT_8x8b)
    {
        for (_i = 0; _i < 8; _i++)
        {
            if (font_type == FONT_8x8)
                _uch = font8x8_basic[_chr][_i];
            else
            {
                _shift = _i << 8;
                _uch = font_8x8[_chr + _shift];
            }

            for (i = 0; i < 8; i++)
            {
                if (font_type == FONT_8x8)
                {
                    if (_uch & (1 << i))
                        s[i] = disp_color;
                    else
                        s[i] = disp_color_background;
                }
                else
                {
                    if (_uch & (0x80 >> i))
                        s[i] = disp_color;
                    else
                        s[i] = disp_color_background;
                }
            }
            if (OSD_path == OSD_PATH_DISP)
            {
                tw_wr_osd_buffer (s, 0);
                tw_wr_osd_buffer (&s[4], 1);
                tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
            }
            else
            {
                for (i = 0; i < 4; i++)
                {
                    if (s[i*2] == disp_color)
                        _buf[i] = rec_color << 4;
                    else
                        _buf[i] = rec_color_background << 4;

                    if (s[i*2+1] == disp_color)
                        _buf[i] += rec_color;
                    else
                        _buf[i] += rec_color_background;
                }
                tw_wr_osd_buffer (_buf, 0);
                tw_wr_display_from_buffer (_pos_X, _y_line, 0); // 0 is 4 bytes, 1 is 8 bytes
            }


            _y_line++;
        }
    }

    if (font_type == FONT_16x8)
    {
        for (_i = 0; _i < 8; _i++)
        {
            _shift = _i << 8;
//            _uch = font_8x8[_chr + _shift];
            _uch = font8x8_basic[_chr][_i];
            for (i = 0; i < 8; i++)
            {
                //if (_uch & (0x80 >> i))
                if (_uch & (1 << i))
                {
                    s[i * 2] = disp_color;
                    s[i * 2 + 1] = disp_color;
                }
                else
                {
                    s[i * 2] = disp_color_background;
                    s[i * 2 + 1] = disp_color_background;
                }
            }
            if (OSD_path == OSD_PATH_DISP)
            {
                tw_wr_osd_buffer (s, 0);
                tw_wr_osd_buffer (&s[4], 1);
                tw_wr_osd_buffer (&s[8], 2);
                tw_wr_osd_buffer (&s[12], 3);
                tw_wr_display_from_buffer (_pos_X, _y_line, 3); // 0 is 4 bytes, 1 is 8 bytes
            }
            else
            {
                for (i = 0; i < 8; i++)
                {
                    if (s[i * 2] == disp_color)
                        _buf[i] = rec_color << 4;
                    else
                        _buf[i] = rec_color_background << 4;

                    if (s[i * 2 + 1] == disp_color)
                        _buf[i] += rec_color;
                    else
                        _buf[i] += rec_color_background;
                }
                tw_wr_osd_buffer (_buf, 0);
                tw_wr_osd_buffer (&_buf[4], 1);
                tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
            }
            _y_line++;
        }
    }

    if (font_type == FONT_16x16)
    {
        for (_i = 0; _i < 8; _i++)
        {
            _shift = _i << 8;
            _uch = font_8x8[_chr + _shift];

            for (i = 0; i < 8; i++)
            {
                if (_uch & (0x80 >> i))
                {
                    s[i * 2] = disp_color;
                    s[i * 2 + 1] = disp_color;
                }
                else
                {
                    s[i * 2] = disp_color_background;
                    s[i * 2 + 1] = disp_color_background;
                }
            }
            if (OSD_path == OSD_PATH_DISP)
            {
                tw_wr_osd_buffer (s, 0);
                tw_wr_osd_buffer (&s[4], 1);
                tw_wr_osd_buffer (&s[8], 2);
                tw_wr_osd_buffer (&s[12], 3);
                tw_wr_display_from_buffer (_pos_X, _y_line, 3); // 0 is 4 bytes, 1 is 8 bytes
                tw_wr_osd_buffer (s, 0);
                tw_wr_osd_buffer (&s[4], 1);
                tw_wr_osd_buffer (&s[8], 2);
                tw_wr_osd_buffer (&s[12], 3);
                tw_wr_display_from_buffer (_pos_X, _y_line + 1, 3); // 0 is 4 bytes, 1 is 8 bytes
            }
            else
            {
                for (i = 0; i < 8; i++)
                {
                    if (s[i * 2] == disp_color)
                        _buf[i] = rec_color << 4;
                    else
                        _buf[i] = rec_color_background << 4;

                    if (s[i * 2 + 1] == disp_color)
                        _buf[i] += rec_color;
                    else
                        _buf[i] += rec_color_background;
                }
                tw_wr_osd_buffer (_buf, 0);
                tw_wr_osd_buffer (&_buf[4], 1);
                tw_wr_display_from_buffer (_pos_X, _y_line, 1); // 0 is 4 bytes, 1 is 8 bytes
                tw_wr_osd_buffer (_buf, 0);
                tw_wr_osd_buffer (&_buf[4], 1);
                tw_wr_display_from_buffer (_pos_X, _y_line + 1, 1); // 0 is 4 bytes, 1 is 8 bytes
            }
            _y_line++;
            _y_line++;
        }
    }
}



void tw_ch_set_window (unsigned char _ch, unsigned int _pos_H, unsigned int _pos_V, unsigned int _len_H)
{
    unsigned int _ADDR_1, _ADDR_2;

    unsigned int _HL, _HR, _VT, _VB, _SCALE;
    long unsigned int luint;

    switch (_ch)
    {
    default:
    case 1:
        _ADDR_1 = 0x081;
        _ADDR_2 = 0x130;
        break;

    case 2:
        _ADDR_1 = 0x091;
        _ADDR_2 = 0x134;
        break;

    case 3:
        _ADDR_1 = 0x0A1;
        _ADDR_2 = 0x138;
        break;

    case 4:
        _ADDR_1 = 0x0B1;
        _ADDR_2 = 0x13C;
        break;
    }

    _HL = _pos_H;
    _HR = _pos_H + _len_H;
    _VT = _pos_V;
    luint = _HR - _HL;
    luint = luint * 144;
    luint = luint / 180;
    _VB = _VT + luint;

    luint = _HR - _HL;
    luint = luint * 0xFFFF;
    luint = luint / 180;
    _SCALE = luint;

    tw_write_register (_ADDR_1++, _SCALE >> 8);
    tw_write_register (_ADDR_1++, _SCALE);
    tw_write_register (_ADDR_1++, _SCALE >> 8);
    tw_write_register (_ADDR_1++, _SCALE);
    tw_write_register (_ADDR_2++, _HL);
    tw_write_register (_ADDR_2++, _HR);
    tw_write_register (_ADDR_2++, _VT);
    tw_write_register (_ADDR_2++, _VB);
}

void tw_ch_settings (unsigned char _ch, unsigned char _on_off, unsigned char _popup)
{

    if (_ch < 1 || _ch > 4) return;

    unsigned char value = _ch - 1;
    if (_on_off) value |= 0x80;
    if (_popup) value |= 0x40;
    tw_write_register (0x110 + (_ch - 1) * 0x08, value);
}

void tw_set_ch_input(char ch, input_channel input)
{
    switch (ch)
    {
    case 1:
        tw_write_register(0x080, input);
        break;
    case 2:
        tw_write_register(0x090, input);
        break;
    case 3:
        tw_write_register(0x0a0, input);
        break;
    case 4:
        tw_write_register(0x0b0, input);
        break;
    }
}


void tw_ext_set_pos_registers(unsigned int start_x, unsigned int start_y, unsigned int xw, unsigned int yw)
{
	unsigned char low, up;
	unsigned char reg209;
	unsigned char reg24e;
	unsigned int end_x, end_y;

	end_x = start_x + xw;
	end_y = start_y + yw;


	low = start_x & 0x00ff;
	tw_write_register(0x205, low);

	low = start_y & 0x00ff;
	tw_write_register(0x207, low);

	low = end_x & 0x00ff;
	tw_write_register(0x206, low);

	low = end_y & 0x00ff;
	tw_write_register(0x208, low);
	
	reg209 = (((start_y >> 8) & 0x03) << 2)  + ((end_y >> 8) & 0x03);
	if (OSD_work_field == FLD_EVEN) reg209 = reg209 | 0b00001000;
	else reg209 = reg209 & 0b11110111;


	tw_write_register(0x209, reg209);

	reg24e = (((start_x >> 8) & 0x03) << 6) + (((end_x >> 8) & 0x03) << 4);
	tw_write_register(0x24e, reg24e);
	


}


void init_scratch_memory()
{

	bool led_state = LOW;

	// 0, 0 white with transparent background
	// 256, 0 red with transparent background
	// 512, 0 white with mixed background
	// 768, 0 red with mixed background
	// 0, 144 red with blinking 


	//Upload character sets to scratch ram

	//And now from Scratch to display
	debug("table fill start - %lu\n", millis());
	unsigned long now = millis();


	tw_write_register(0x240, 0x01);  // OSD_EXTOP_EN = 1
	tw_write_register(0x241, 0x01);	 // OSD_OPMODE = 1 (Bitmap fill)
	tw_ext_set_pos_registers(0, 0, 255, 143);

	tw_write_register(0x24f, 0x01);  // OSD_DSTLOC = 1, OSD_OPSTART = 1
	for (int i = 0; i < sizeof(outline_block); i++)
	{
		unsigned char c = outline_block[i];
		if (c == 1) c = COLOR_WHITE;
		tw_write_register(0x200, c);
	}

	tw_write_register(0x240, 0x01);  // OSD_EXTOP_EN = 1
	tw_write_register(0x241, 0x01);	 // OSD_OPMODE = 1 (Bitmap fill)
	tw_ext_set_pos_registers(256, 0, 255, 143);

	tw_write_register(0x24f, 0x01);  // OSD_DSTLOC = 1, OSD_OPSTART = 1
	for (int i = 0; i < sizeof(outline_block); i++)
	{
		unsigned char c = outline_block[i];
		if (c == 1) c = COLOR_RED;
		tw_write_register(0x200, c);
	}

	tw_write_register(0x240, 0x01);  // OSD_EXTOP_EN = 1
	tw_write_register(0x241, 0x01);	 // OSD_OPMODE = 1 (Bitmap fill)
	tw_ext_set_pos_registers(512, 0, 255, 143);

	tw_write_register(0x24f, 0x01);  // OSD_DSTLOC = 1, OSD_OPSTART = 1
	for (int i = 0; i < sizeof(outline_block); i++)
	{
		unsigned char c = outline_block[i];
		if (c == 1) c = COLOR_WHITE;
		if (c == 0xff) c = COLOR_50_WHITE | MIX;
		tw_write_register(0x200, c);
	}


	tw_write_register(0x240, 0x01);  // OSD_EXTOP_EN = 1
	tw_write_register(0x241, 0x01);	 // OSD_OPMODE = 1 (Bitmap fill)
	tw_ext_set_pos_registers(768, 0, 255, 143);

	tw_write_register(0x24f, 0x01);  // OSD_DSTLOC = 1, OSD_OPSTART = 1
	for (int i = 0; i < sizeof(outline_block); i++)
	{
		unsigned char c = outline_block[i];
		if (c == 1) c = COLOR_RED;
		if (c == 0xff) c = COLOR_50_WHITE | MIX;
		tw_write_register(0x200, c);
	}

	tw_write_register(0x240, 0x01);  // OSD_EXTOP_EN = 1
	tw_write_register(0x241, 0x01);	 // OSD_OPMODE = 1 (Bitmap fill)
	tw_ext_set_pos_registers(0, 144, 255, 143);

	tw_write_register(0x24f, 0x01);  // OSD_DSTLOC = 1, OSD_OPSTART = 1
	for (int i = 0; i < sizeof(outline_block); i++)
	{
		unsigned char c = outline_block[i];
		if (c == 1) c = COLOR_RED | BLINK;
		if (c == 0xff) c = COLOR_50_WHITE | BLINK;
		if (c == 0x00) c = COLOR_BLACK | BLINK;
		tw_write_register(0x200, c);
	}
	tw_write_register(0x240, 0x00);  // OSD_EXTOP_EN = 1

}

void tw_ext_putchar(unsigned int x, unsigned int y, char chr)
{
	x = x * 4;
	y = y * 2;
	chr = chr - 32;
	tw_ext_block_move_scratch_to_osd(x, y, 15, 23, (chr % 16) * 16, ((chr / 16) * 24));
}



void tw_ext_block_move_scratch_to_osd(unsigned int hpos, unsigned int vpos, unsigned int width, unsigned int height, unsigned int src_hpos, unsigned int src_vpos)
{


	unsigned char reg205, reg206, reg207, reg208, reg209, reg24c, reg24d, reg24e, reg24f;
	unsigned int end_hpos, end_vpos;

	end_hpos = hpos + width;
	end_vpos = vpos + height;

	reg205 = hpos & 0xff;
	reg206 = end_hpos & 0xff;
	reg207 = vpos & 0xff;
	reg208 = end_vpos & 0xff;
	reg209 = (((vpos >> 8) & 0x03) << 2) + ((end_vpos >> 8) & 0x03);
	if (OSD_work_field == FLD_EVEN) reg209 = reg209 | 0b00001000;
	else reg209 = reg209 & 0b11110111;

	reg24c = src_hpos & 0xff;
	reg24d = src_vpos & 0xff;

	reg24e = (((hpos >> 8) & 0x03) << 6) +
		(((end_hpos >> 8) & 0x03) << 4) +
		(((src_vpos >> 8) & 0x03) << 2) +
		((src_hpos >> 8) & 0x03);

	cnt = 0;
	data_buf[cnt++] = 0x01;			// OSD_EXTOP_EN = 1;
	data_buf[cnt++] = 0x03;		    // OSD_OPMODE = 3 Block move
	tw_write_buf(0x240, data_buf, cnt);


	cnt = 0;
	data_buf[cnt++] = reg205;
	data_buf[cnt++] = reg206;
	data_buf[cnt++] = reg207;
	data_buf[cnt++] = reg208;
	data_buf[cnt++] = reg209;
	if (OSD_path == OSD_PATH_REC)
		data_buf[cnt++] = 0b00100000;
	else
		data_buf[cnt++] = 0b00000000;

	tw_write_buf(0x205, data_buf, cnt);

	cnt = 0;
	data_buf[cnt++] = reg24c;
	data_buf[cnt++] = reg24d;
	data_buf[cnt++] = reg24e;
	data_buf[cnt++] = 0x05;  // OSD_DSTLOC = 1, OSD_OPSTART = 1
	tw_write_buf(0x24c, data_buf, cnt);
	delay(10);

	tw_write_register(0x240, 0x00);  // OSD_EXTOP_EN = 1

}

void tw_clear_all_pages(void)
{
    // Clear OSD on record path both fields
    tw_osd_fill_region(0, 0, 89, 287, 0xff, FLD_EVEN, OSD_PATH_REC,0);
    tw_wait_for_osd_write(100);
    tw_osd_fill_region(0, 0, 89, 287, 0xff, FLD_ODD, OSD_PATH_REC,0);
    tw_wait_for_osd_write(100);

    for (unsigned char i = 0; i < 6; i++)
    {
        tw_osd_set_display_page(i);
        // Clear OSD on display path both fields
        tw_osd_fill_region(0, 0, 179, 287, 0xff, FLD_EVEN, OSD_PATH_DISP,i);
        tw_wait_for_osd_write(100);
        tw_osd_fill_region(0, 0, 179, 287, 0xff, FLD_ODD, OSD_PATH_DISP,i);
        tw_wait_for_osd_write(100);
    }

    tw_osd_set_display_page(0);
}



