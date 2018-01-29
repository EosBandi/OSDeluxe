#include "OSDeluxe.h"


unsigned char cnt, count, data_buf[20];
unsigned int ADDR_buf, count_TW2835;
unsigned char OSD_work_field, OSD_display_field;
unsigned char OSD_path = 0;
unsigned char rec_color_shadow, rec_color, rec_color_background;
unsigned char disp_color_shadow, disp_color, disp_color_background;

FontType font_type; // Global variable to hold current font_type

void tw_init ()
{

    tw_write_register (0x100, 0x80);
    tw_write_register (0x17e, 0x89);
    tw_write_register (0x17f, 0x84);

    cnt = 0;
    data_buf[cnt++] = 0x60; // a0 volt 77
    data_buf[cnt++] = 0x11; // a1
    data_buf[cnt++] = 0x90; // a2
    data_buf[cnt++] = 0x01; // a3
    data_buf[cnt++] = 0x80; // a4
    data_buf[cnt++] = 0x20; // a5  //0x20
    data_buf[cnt++] = 0x00; // a6
    data_buf[cnt++] = 0x0C; // a7
    data_buf[cnt++] = 0x20; // a8
    data_buf[cnt++] = 0x4d; // a9  4c
    tw_write_buf (0x1a0, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = 0x20;
    data_buf[cnt++] = 0x28;

    tw_write_buf (0x005, data_buf, cnt);
    tw_write_buf (0x015, data_buf, cnt);
    tw_write_buf (0x025, data_buf, cnt);
    tw_write_buf (0x035, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = 0x90;
    data_buf[cnt++] = 0x02;
    data_buf[cnt++] = 0x00;
    data_buf[cnt++] = 0x91;
    data_buf[cnt++] = 0x02;
    data_buf[cnt++] = 0x00;
    data_buf[cnt++] = 0x93;
    data_buf[cnt++] = 0x02;
    data_buf[cnt++] = 0x00;
    data_buf[cnt++] = 0x94;
    data_buf[cnt++] = 0x02;
    data_buf[cnt++] = 0x00;
    tw_write_buf (0x160, data_buf, cnt);

    tw_write_register (0x001, 0x00);
    tw_write_register (0x080, 0x00);
    tw_write_register (0x110, 0x00);
    //tw_write_register (0x111, 0x00);
    tw_write_register (0x111, 0b00001010);
    tw_write_register (0x112, 0x00); //00
    tw_write_register (0x116, 0x00);

    tw_write_register (0x011, 0x08);
    tw_write_register (0x090, 0x40);    //input was 0x40
    tw_write_register (0x118, 0x01);
    tw_write_register (0x119, 0x0a);
    tw_write_register (0x11A, 0x00);
    tw_write_register (0x11e, 0x01);

    tw_write_register (0x021, 0x08);
    tw_write_register (0x0a0, 0x80);   //inpur was 0x80
    tw_write_register (0x120, 0x02);
    tw_write_register (0x121, 0x0a);
    tw_write_register (0x122, 0x00);
    tw_write_register (0x126, 0x02);

    tw_write_register (0x031, 0x08);
    tw_write_register (0x0b0, 0xc0);   //input was 0xc0
    tw_write_register (0x128, 0x03);
    tw_write_register (0x129, 0x0a);
    tw_write_register (0x12a, 0x00);
    tw_write_register (0x12e, 0x03);

    tw_write_register (0x20F, 0x0F);

    for (char i = 0; i < 14; i++)
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
    tw_write_register (0x001, 0b00001000);
    tw_write_register (0x011, 0b00001000);
    tw_write_register (0x021, 0b00001000);
    tw_write_register (0x031, 0b00001000);

    tw_write_register (0x0c3, 0b11110000);
    tw_write_register (0x0c4, 0x00);

    tw_write_register (0x21f, 0b00010011); // bitmap opacity
    tw_write_register (0x221, 0b01010000); // Red for box 1
    tw_write_register (0x222, 0);
    tw_write_register (0x223, 180);
    tw_write_register (0x225, 144);

    //tw_write_register (0x220, 0b00111000);

    tw_write_register (0x105, 0b00000100); // bypass mode when video loss is detected

    tw_write_register (0x00d, 0x03);
    tw_write_register (0x01d, 0x03);
    tw_write_register (0x02d, 0x03);
    tw_write_register (0x03d, 0x03);

    // tw_write_register(0x047,0b01100010); //Reduce disp_color noise

    // tw_write_register(0x1aa,0b10001000); //Output bandwith Narrow filter
    tw_write_register(0x1aa,0b10101010); //Output bandwith Middle filter



    // Enable overlays dual mode high priority
    tw_write_register (0x238, 0b00000101);


    // tw_write_register(0x1ab,0b00001010);
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

void tw_puts (char *str, char posx, unsigned short posy)
{

    char inc  = 0; //Character width in posx units (OSD PATH:4pixel, REC PATH:8pixel)
    char kf_a = 0; //Addtional krening in posx units (see above);
    char _posX = posx;




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
    
    }

    for (char a = 0; a < strlen (str); a++)
    {
        tw_osd_out_char (_posX, posy, str[a]);
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


void tw_osd_set_display(char rd_page, char dp_field, char rec_field )
{
 
  char reg20f;
  
  reg20f = rd_page << 4;
  reg20f = reg20f + (dp_field << 2);
  reg20f = reg20f + (rec_field);
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

          /*
          if (OSD_path == OSD_PATH_REC)
          {
              x = x / 2;
              w = w / 2;
          }

      */

    if ((m16 == 10) || (m16 == 12) || (m16 == 13))
    {
        tw_osd_fill_region(x, y, x + w - 5, y + h, color, OSD_work_field, OSD_path);
        tw_osd_fill_region(x + w - 5, y, x + w, y + h, color, OSD_work_field, OSD_path);
    }
    else { tw_osd_fill_region(x, y, x + w, y + h, color, OSD_work_field, OSD_path); }
}

void tw_osd_fill_region (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char disp_color, unsigned char _field, unsigned char path)
{
    unsigned char reg209;
    unsigned char m16;

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
        data_buf[cnt++] = 0b11100000;
    else
        data_buf[cnt++] = 0b11000000;

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

void tw_display_logo()
{

    int a = 0;

    for (int y = 0; y < 100; y++)
    {
        for (int i = 0; i < 50; i++)
        {
            tw_set_osd_buf(logo[a++], logo[a++], logo[a++], logo[a++]);
            tw_wr_display_from_buffer(50 + i, y+50, 0);
            

        }
    }
}