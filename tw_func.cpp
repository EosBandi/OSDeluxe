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

uint8_t cnt, data_buf[20];          // Generic buffer for writing data bursts to tw2837
uint8_t OSD256_wr_page;             // Actual page to be written by OSD256 functions
boundary_t osd_draw_boundary;       // Dynamically defined boundary for restrict drawing via setpixel_fast
static uint8_t current_page = 0xff; // Actual selected TW2837 data page (to avoid additional page selection)

#ifdef TW2837_I2C

#define TW_RESET 14

#else

#define TW_RESET 23

#define TW_CS 22
#define TW_WR 12
#define TW_RD 11
#define TW_AEN 15

#define TW_DATA GPIOD_PDOR

#endif

void tw_init()
{

    // Output video format selection
    // bit 7 - 1-50Hz, 625 line
    tw_write_register(0x100, 0x80);

    // Start memory init (SDRAM)
    tw_write_register(0x17f, 0x80);

    // Enable black background for channels with no video
    tw_write_register(0x0c3, 0xf0);

    static uint8_t pg1_a0_video_path[] = {
        0x60, // a0  bit67-INX - bit54-INY
        0x11, // a1  1 CBVS output for both VAOYX and VAOCX DAV
        0x90, // a2  1 CVBS display path for VAOYY (bit7 must be 1) //90
        0x00, // a3  Digital output setting (0 disable)
        0x80, // a4  Enable Master mode
        0x20, // a5  Vertical sync delay (0x20 - default)
        0x00, // a6  Horizontal sync delay (0x00 - default)
        0x0c, // a7  Active video delay (0x0c - default)
        0x20, // a8  ACTIVE_HDELAY (0x20 - default)
        0x4d, // a9  bit7-6 color subcarrier (1 for pal), bi54-0, bit3-1,
        0xaa, // aa  Video encode chrominance bandwidth
        0x00  // ab  No color kill and no test bar
    };

    tw_write_buf(0x1a0, pg1_a0_video_path, sizeof(pg1_a0_video_path));

    // video decoder settings Same for all channels
    static uint8_t pg0_00_decoder_settings[] = {
        0x00, // 00	Read only, no need
        0x07, // 01	Read only, no need ? Last three bit Vertical peaking level, 0 default
        0x02, // 02	Hdelay    //0x0a
        0xd0, // 03    HActive
        0x05, // 04    Vdelay
        0x20, // 05    VActive
        0x28, // 06    Extra bits for the four above
        0x00, // 07    Hue for NTSC system (no need on PAL)
        0x3f, // 08    Bit7 NTSC SCURVE, bit6 Internal, bit5-4 CTI level (1h) bit3-0 sharpnedd
        0x64, // 09	Contrast 0x80 is default
        0x00, // 0a    Brightness 0x80 is no changes
        0x50, // 0b    Saturation Cb
        0x50, // 0c    Saturation Cr
        0x00, // 0d    Read Only
        0x07  // 0e    Standard Auto detection
    };

    tw_write_buf(0x000, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));
    tw_write_buf(0x010, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));
    tw_write_buf(0x020, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));
    tw_write_buf(0x030, pg0_00_decoder_settings, sizeof(pg0_00_decoder_settings));

    //    |       ch1      ||      ch2       ||     ch3        ||     c4
    //	   0x60  0x61  0x62  0x63  0x64  0x65  0x66  0x67  0x68  0x69  0x6a  0x6b
    static uint8_t pg1_60_channel_settings[] = { 0xc0, 0x02, 0x00, 0xc1, 0x02, 0x00, 0xc2, 0x02, 0x00, 0xc3, 0x02, 0x00, 0x00, 0xe4 };

    tw_write_buf(0x160, pg1_60_channel_settings, sizeof(pg1_60_channel_settings));

    tw_write_register(0x105, 0b00000100); // bypass mode when video loss is detected

    // Input selection for each scaler in the display path
    tw_write_register(0x080, 0x00); // ch0 scaler - VIN0 pin
    tw_write_register(0x090, 0x40); // ch1 scaler - VIN1 pin
    tw_write_register(0x0a0, 0x80); // ch2 scaler - VIN2 pin
    tw_write_register(0x0b0, 0xc0); // ch3 scaler - VIN3 pin

    // Channel mode selection
    tw_write_register(0x110, 0x00); // channel 0 disable, no popup   bit7-enable, bit6-popup, bit1-0 As see below
    tw_write_register(0x118, 0x01); // channel 1 disable, no popup
    tw_write_register(0x120, 0x02); // channel 2 disable, no popup
    tw_write_register(0x128, 0x03); // channel 3 disable, no popup

    // Channel settings
    // bit7 - Recall_en, bit6-freeze, bit5-H_mirror, bit4-v_mirror,bit3-enhance, bit2-blank, bit1-boundary, bit0-boundary blink
    tw_write_register(0x111, 0x0a); // enhance and boundary
    tw_write_register(0x119, 0x0a);
    tw_write_register(0x121, 0x0a);
    tw_write_register(0x129, 0x0a);

    // Channel conversion registers
    // bi7-0, bit6-0, bit5-field to frame mode, bit4-DVR to normal mode, bit3-0 Recall address (must be 0)
    tw_write_register(0x112, 0x00);
    tw_write_register(0x11A, 0x00);
    tw_write_register(0x122, 0x00);
    tw_write_register(0x12a, 0x00);

    // Channel strobe, do not change
    tw_write_register(0x116, 0x00);
    tw_write_register(0x11e, 0x01);
    tw_write_register(0x126, 0x02);
    tw_write_register(0x12e, 0x03);

    // Power down audio dac and adc
    tw_write_register(0x04c, 0b00110000);

    // Select OSD reading page and OSD overlay mode
    tw_write_register(0x20F, 0x0f);

    for (uint8_t i = 0; i < 18; i++)
    {
        // ADDR_buf = 0x20B;
        cnt = 0;
        data_buf[cnt] = colortable[i][0];
        cnt++;
        data_buf[cnt] = colortable[i][1];
        cnt++;
        data_buf[cnt] = colortable[i][2];
        cnt++;
        data_buf[cnt] = 0x80 + i;
        cnt++;
        tw_write_buf(0x20b, data_buf, cnt);
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        // ADDR_buf = 0x20B;
        cnt = 0;
        data_buf[cnt] = colortable_rec[i][0];
        cnt++;
        data_buf[cnt] = colortable_rec[i][1];
        cnt++;
        data_buf[cnt] = colortable_rec[i][2];
        cnt++;
        data_buf[cnt] = 0x80 + 64 + i;
        cnt++;
        tw_write_buf(0x20b, data_buf, cnt);
    }

    tw_write_register(0x283, 0x05);

    tw_write_register(0x21f, 0b00010011); // bitmap opacity
    tw_write_register(0x221, 0b01010000); // Red for box 1
    tw_write_register(0x222, 0);
    tw_write_register(0x223, 180);
    tw_write_register(0x225, 144);

    uint8_t tbl_pal_pg0_sfr3[] = {
        //...
        //	0x00,0x00,0x00,0x07,		//...		0xc0~0xc3	//... AUTOBGNDxx on
        0x00, 0x00, 0x00, 0x00, //...		0xc0~0xc3	//... AUTOBGNDxx off
        0x00, 0xff, 0xf0, 0xff, //...		0xc4~0xc7	//... recommend:full table
                                //	0x00,0x00,0xf0,0xff,		//...		0xc4~0xc7	//... temporal:quad table
        0x00, 0x3c, 0x4f        //...		0xc8~0xca	//... update:060315
    };

    tw_write_buf(0x0c0, tbl_pal_pg0_sfr3, sizeof(tbl_pal_pg0_sfr3));

    uint8_t tbl_pal_pg0_sfr1[] = {
        //... other decoder part
        0x00, 0x77, 0x07, 0x45, //...		0x40~0x43	//... update:060626
        0xa0, 0xd0, 0x0,  0xf0, //...		0x44~0x47
        0xf0, 0xf0, 0xf0, 0x02, //...		0x48~0x4b
        0x00, 0x0f, 0x05, 0x05, //...		0x4c~0x4f
        0x00, 0x00, 0x10, 0x00, //...		0x50~0x53
        0x00, 0x00, 0x00, 0x30  //...		0x54~0x57
    };

    tw_write_buf(0x040, tbl_pal_pg0_sfr1, sizeof(tbl_pal_pg0_sfr1));

    // Enable overlays dual mode high priority
    tw_write_register(0x238, 0b00000101);
}

uint8_t tw_read_register(unsigned int rdADDR)
{
    uint8_t page, reg;
    uint8_t ret;

    page = rdADDR >> 8;
    reg = rdADDR & 0x00ff;

#ifdef TW2837_I2C

    Wire.beginTransmission(0x42);
    Wire.write(page);
    Wire.write(reg);
    Wire.endTransmission(I2C_NOSTOP);
    Wire.requestFrom(0x42, 1, I2C_STOP);
    ret = Wire.read();

#else

    page = 0x80 | page; // Set the page address upper bit for auto increment

    if (page != current_page)
    {
        digitalWriteFast(TW_CS, 0);                // Chip Select
        *(volatile uint8_t *)(&GPIOD_PDOR) = 0xff; // Page select address
        digitalWriteFast(TW_AEN, 1);
        asm("nop");
        digitalWriteFast(TW_AEN, 0);
        *(volatile uint8_t *)(&GPIOD_PDOR) = page; // Page select address
        digitalWriteFast(TW_WR, 0);
        asm("nop");
        digitalWriteFast(TW_WR, 1);
        digitalWriteFast(TW_CS, 1); // clear Chip Select
        current_page = page;
    }

    digitalWriteFast(TW_CS, 0);               // Chip Select
    *(volatile uint8_t *)(&GPIOD_PDOR) = reg; // Page select address
    digitalWriteFast(TW_AEN, 1);
    asm("nop");
    digitalWriteFast(TW_AEN, 0);

    GPIOD_PDDR = 0;
    PORTD_GPCLR = 0x000F0100;
    P05;	
    digitalWriteFast(TW_RD, 0);
    P05;
    ret = *(volatile uint8_t *)(&GPIOD_PDIR);
    digitalWriteFast(TW_RD, 1);
    digitalWriteFast(TW_CS, 1); // clear Chip Select
    GPIOD_PDDR = 255;
    PORTD_GPCLR = 0x000F0144;

#endif

    return (ret);
}

void tw_write_register(unsigned int wrADDR, uint8_t content)
{
    uint8_t page, reg;

    page = wrADDR >> 8;
    reg = wrADDR & 0x00ff;

#ifdef TW2837_I2C

    Wire.beginTransmission(0x42);
    Wire.write(page);
    Wire.write(reg);
    Wire.write(content);
    Wire.endTransmission();

#else

    page = 0x80 | page; // Set the page address upper bit for auto increment

    if (page != current_page)
    {
        digitalWriteFast(TW_CS, 0);                // Chip Select
        *(volatile uint8_t *)(&GPIOD_PDOR) = 0xff; // Page select address
        digitalWriteFast(TW_AEN, 1);
        asm("nop");
        digitalWriteFast(TW_AEN, 0);
        *(volatile uint8_t *)(&GPIOD_PDOR) = page; // Page select address
        digitalWriteFast(TW_WR, 0);
        asm("nop");
        digitalWriteFast(TW_WR, 1);
        digitalWriteFast(TW_CS, 1); // clear Chip Select
        current_page = page;
    }

    digitalWriteFast(TW_CS, 0);               // Chip Select
    *(volatile uint8_t *)(&GPIOD_PDOR) = reg; // Page select address
    digitalWriteFast(TW_AEN, 1);
    asm("nop");
    digitalWriteFast(TW_AEN, 0);
    *(volatile uint8_t *)(&GPIOD_PDOR) = content; // Page select address
    digitalWriteFast(TW_WR, 0);
    asm("nop");
    digitalWriteFast(TW_WR, 1);
    digitalWriteFast(TW_CS, 1); // clear Chip Select

#endif
}

uint8_t tw_read_register_bit(unsigned int rdADDR, uint8_t _flg)
{
    uint8_t val;

    val = tw_read_register(rdADDR);
    return (val & _flg);
}

void tw_write_register_bit(unsigned int wrADDR, uint8_t _flg, uint8_t _data)
{
    uint8_t val;

    val = tw_read_register(wrADDR);
    val = (val & ~_flg) | _data;
    tw_write_register(wrADDR, val);
}

void tw_write_buf(uint16_t wrADDR, uint8_t *wrBUF, uint8_t wrCNT)
{
    uint8_t page, reg, i;

    page = wrADDR >> 8;
    reg = wrADDR & 0x00ff;

#ifdef TW2837_I2C

    Wire.beginTransmission(0x42);
    Wire.write(page);
    Wire.write(reg);

    for (i = 0; i < wrCNT; i++)
    {
        Wire.write(wrBUF[i]);
    }
    Wire.endTransmission();
#else

	page = 0x80 | page;			//Set the page address upper bit for auto increment
	if (page != current_page)
    {
        digitalWriteFast(TW_CS, 0);                // Chip Select
        *(volatile uint8_t *)(&GPIOD_PDOR) = 0xff; // Page select address
        digitalWriteFast(TW_AEN, 1);
        asm("nop");
        digitalWriteFast(TW_AEN, 0);
        *(volatile uint8_t *)(&GPIOD_PDOR) =  page; // Page select address
        digitalWriteFast(TW_WR, 0);
        asm("nop");
        digitalWriteFast(TW_WR, 1);
        digitalWriteFast(TW_CS, 1); // clear Chip Select
        current_page = page;
    }

	digitalWriteFast(TW_CS, 0);               // Chip Select
    *(volatile uint8_t *)(&GPIOD_PDOR) = reg; // Page select address
    digitalWriteFast(TW_AEN, 1);
    asm("nop");
    digitalWriteFast(TW_AEN, 0);

	for (i = 0; i < wrCNT; i++)
    {
		*(volatile uint8_t *)(&GPIOD_PDOR) = wrBUF[i]; // Page select address
		digitalWriteFast(TW_WR, 0);
		asm("nop");
		digitalWriteFast(TW_WR, 1);
	}
    digitalWriteFast(TW_CS, 1); // clear Chip Select


#endif
}

void OSD256_set_display_page(char rd_page)
{
    char reg20f = 0;

    reg20f = tw_read_register(0x20f);
    reg20f = reg20f & 0x0f;
    reg20f = reg20f + (rd_page << 4);
    tw_write_register(0x20f, reg20f);
}

void tw_ch_set_window(uint8_t _ch, unsigned int _pos_H, unsigned int _pos_V, unsigned int _len_H)
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

    tw_write_register(_ADDR_1++, _SCALE >> 8);
    tw_write_register(_ADDR_1++, _SCALE);
    tw_write_register(_ADDR_1++, _SCALE >> 8);
    tw_write_register(_ADDR_1++, _SCALE);
    tw_write_register(_ADDR_2++, _HL);
    tw_write_register(_ADDR_2++, _HR);
    tw_write_register(_ADDR_2++, _VT);
    tw_write_register(_ADDR_2++, _VB);

    if ((_SCALE >> 8) > 191)
    {
    }
}

void tw_ch_settings(uint8_t _ch, uint8_t _on_off, uint8_t _popup)
{

    if (_ch < 1 || _ch > 4) return;

    uint8_t value = _ch - 1;
    if (_on_off) value |= 0x80;
    if (_popup) value |= 0x40;
    tw_write_register(0x110 + (_ch - 1) * 0x08, value);
}

void tw_ch_set_input(char ch, char input)
{

    uint8_t inp;
    switch (input)
    {
    case 1:
        inp = INPUT_CH_1;
        break;
    case 2:
        inp = INPUT_CH_2;
        break;
    case 3:
        inp = INPUT_CH_3;
        break;
    case 4:
        inp = INPUT_CH_4;
        break;
    default:
        inp = INPUT_CH_1;
    }

    switch (ch)
    {
    case 1:
        tw_write_register(0x080, inp);
        break;
    case 2:
        tw_write_register(0x090, inp);
        break;
    case 3:
        tw_write_register(0x0a0, inp);
        break;
    case 4:
        tw_write_register(0x0b0, inp);
        break;
    }
}

void OSD256_OSG_Mode_Selection(uint8_t mode) { tw_write_register(0x241, mode & 0x3); }

void OSD256_write_font0(uint8_t dst, uint8_t _pos_x, uint16_t _pos_y, uint8_t _indx, uint8_t color, uint8_t attrib)
{
    uint8_t reg40, tmp;
    const uint8_t *ptr;
    uint16_t tmp1;

    tw_write_register(0x20a, 0x0);          //... y path 0x20, x Path 0x00
    tmp1 = _pos_x << 4;                     //... (_pos_x)*4 -> 4 pixel * 4 -> 16 pixel char
    _pos_y = (_pos_y << 4) + (_pos_y << 2); //... (_pos_y)*(16+4)

    reg40 = tw_read_register(0x240);
    tw_write_register(0x240, (reg40 | 0x1)); // Enable extend OSD feature

    OSD256_OSG_Mode_Selection(1); // Set OSG operation mode to BMP operation

    tw_write_register(0x205, tmp1);

    tmp1 = tmp1 + FONT0_X - 1;
    tw_write_register(0x206, tmp1);

    tmp = ((tmp1 & 0xff00) >> 8 << 6) | ((tmp1 & 0xff00) >> 8 << 4);
    tw_write_register(0x24e, tmp);

    tw_write_register(0x207, _pos_y);

    tmp1 = _pos_y + FONT0_Y - 1;

    tw_write_register(0x208, tmp1);
    tmp = ((_pos_y & 0xff00) >> 8 << 2) | ((tmp1 & 0xff00) >> 8);
    tw_write_register(0x209, tmp);

    tmp1 = (80 * _indx);
    ptr = (ufont_16x20 + tmp1);

    // Set OSG_DSTCTL           0 scratch 1 display
    if (dst == 1)
        tmp = 0x4; // To Display buffer
    else
        tmp = 0; // scratch

    tw_write_register(0x24f, tmp | 0x1);

    for (tmp = 0; tmp < FONT0_Y; tmp++)
    {
        uint8_t pix, pix_data;
        uint8_t i, j, mask;

        for (i = 0; i < 4; i++)
        {
            mask = 0xC0;

            for (j = 0; j < 4; j++)
            {
                pix = (mask & (*ptr)) >> ((3 - j) << 1);
                // debug("0x%02x ", pix);
                // 0 - empty, 1 - outline, 2- ???, 3-main color
                if (!pix)
                    pix_data = 0xff;
                else
                {
                    if (pix == 1)
                        pix_data = COLOR_BLACK | attrib;
                    else if (pix == 2)
                        pix_data = 2 | attrib;
                    else if (pix == 3)
                        pix_data = color | attrib;
                    else
                        pix_data = 0xff; // Any unknown color renders to transparent
                }
                tw_write_register(0x200, pix_data);
                mask >>= 2;
            }
            ptr++;
        }

        WAIT_OSG_WRSTALL;
    }
    WAIT_OSG_IDLE;
    tw_write_register(0x240, reg40); // Restore the saved register 2x40 value
}

void OSD256_create_scratch_font_table(uint8_t dst, uint8_t color, uint8_t attrib, uint8_t font)
{
    uint8_t i, j;

    if (font == 0)
    {
        for (i = 0; i < 14; i++)
        {
            for (j = 0; j < 8; j++)
            {
                uint8_t tmp = i * 8 + j;

                OSD256_write_font0(dst, (tmp % 38), (tmp / 38), tmp, color, attrib);
            }
        }
    }
    else
    {
        for (i = 0; i < 96; i++)
        {
            OSD256_write_font1(dst, (i % 16), (i / 16), i, color, attrib);
        }
    }
}

void OSD256_block_fill(uint8_t _pth, uint8_t dst, uint16_t start_X, uint16_t start_Y, uint16_t end_X, uint16_t end_Y, uint8_t color)
{
    uint8_t tmp;
    uint8_t reg205, reg206, reg207, reg208, reg209, reg20a, reg24e, reg24f;
    uint8_t reg241, reg242, reg243;

    // check boundaries, if dst=1 then destination is display, so check screen boundaries

    if (dst == 1)
    {
        if (start_X >= SCR_X_SIZE) start_X = SCR_X_SIZE;
        if (start_Y >= SCR_Y_SIZE) start_Y = SCR_Y_SIZE;
        if (end_X >= SCR_X_SIZE) end_X = SCR_X_SIZE;
        if (end_Y >= SCR_Y_SIZE) end_Y = SCR_Y_SIZE;
    }
    else
    {
        if (start_X >= 1023) start_X = 1023;
        if (start_Y >= 431) start_Y = 431;
        if (end_X >= 1023) end_X = 1023;
        if (end_Y >= 431) end_Y = 431;
    }

    if (BitSet(_pth, PTH_X))
    {
        reg20a = (OSD256_wr_page & 0x7) << 2;
    }
    else
    {
        reg20a = 0x20;
        start_X >>= 1;
        end_X >>= 1;
    }

    if (start_X >= end_X) start_X = end_X;

    if (start_Y >= end_Y) start_Y = end_Y;

    if ((start_X == end_X) && (start_Y == end_Y)) // Since we can't fill only 1 pixel, just return.
        return;

    reg241 = 0x02;
    reg242 = 0xca;
    reg243 = color;

    reg205 = start_X;
    reg206 = end_X;
    reg24e = ((start_X & 0xff00) >> 8 << 6) | ((end_X & 0xff00) >> 8 << 4);

    reg207 = start_Y;
    reg208 = end_Y;
    reg209 = ((start_Y & 0xff00) >> 8 << 2) | ((end_Y & 0xff00) >> 8);

    // Set OSG_DSTCTL           0 scratch 1 display
    if (dst == 1)
        tmp = 0x4; // To Display buffer
    else
        tmp = 0; // scratch

    reg24f = tmp | 0x01;

    cnt = 0;
    data_buf[cnt++] = reg241;
    data_buf[cnt++] = reg242;
    data_buf[cnt++] = reg243;
    tw_write_buf(0x241, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = reg205;
    data_buf[cnt++] = reg206;
    data_buf[cnt++] = reg207;
    data_buf[cnt++] = reg208;
    data_buf[cnt++] = reg209;
    data_buf[cnt++] = reg20a;
    tw_write_buf(0x205, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = reg24e;
    data_buf[cnt++] = reg24f;
    tw_write_buf(0x24e, data_buf, cnt);

    WAIT_OSG_IDLE;
}

// src/dst 0 - sratch, 1 - display
void OSD256_block_transfer(uint8_t src, uint8_t dst, uint16_t src_start_x, uint16_t src_start_y, uint16_t dst_start_x, uint16_t dst_start_y, uint16_t dst_end_x, uint16_t dst_end_y)
{

    uint8_t reg40, tmp;

    tw_write_register(0x20a, (OSD256_wr_page & 0x7) << 2); //... y path 0x20, x Path 0x00

    reg40 = tw_read_register(0x240);
    tw_write_register(0x240, (reg40 | 0x1)); // Enable extend OSD feature

    OSD256_OSG_Mode_Selection(3); // Set OSG operation mode to Block transfer

    tw_write_register(0x207, dst_start_y);
    tw_write_register(0x208, dst_end_y);
    tmp = ((dst_start_y & 0xff00) >> 8 << 2) | ((dst_end_y & 0xff00) >> 8);
    tw_write_register(0x209, tmp);

    tw_write_register(0x205, dst_start_x);
    tw_write_register(0x206, dst_end_x);
    tmp = ((dst_start_x & 0xff00) >> 8 << 6) | ((dst_end_x & 0xff00) >> 8 << 4);

    tw_write_register(0x24c, src_start_x);
    tw_write_register(0x24d, src_start_y);
    tmp |= ((src_start_x & 0xff00) >> 8) | ((src_start_y & 0xff00) >> 8 << 2);

    tw_write_register(0x24e, tmp);

    tmp = (dst << 2) + (src << 1);
    tw_write_register(0x24f, (tmp | 0x1));

    WAIT_OSG_IDLE;

    tw_write_register(0x240, reg40); // Restore the saved register 2x40 value
}

void OSD256_putc(uint16_t _pos_x, uint16_t _pos_y, uint8_t _indx, uint8_t color, uint8_t font)
{

    uint16_t src_start_x, src_start_y, dst_start_x, dst_end_x, dst_end_y, color_x, color_y;

    uint8_t fnt_tbl_row_size = 38;
    uint8_t fnt_height = 20;
    uint8_t fnt_width = 16;

    if (font == 1)
    {
        fnt_tbl_row_size = 16;
        fnt_height = 24;
        fnt_width = 16;
    }

    src_start_x = _indx % fnt_tbl_row_size;

    dst_start_x = _pos_x;
    dst_end_x = dst_start_x + fnt_width - 1;

    //... (_pos_x)*4 -> 4 pixel * 4 -> 16 pixel char
    src_start_x <<= 4;                                     // This could be changed later
    tw_write_register(0x20a, (OSD256_wr_page & 0x7) << 2); //... y path 0x20, x Path 0x00

    src_start_y = _indx / fnt_tbl_row_size;

    src_start_y = src_start_y * fnt_height;

    dst_end_y = _pos_y + fnt_height - 1;

    if (font == 0)
    {
        switch (color)
        {
        case OSD256_FONT_WHITE:
            color_x = 0;
            color_y = 0;
            break;
        case OSD256_FONT_RED:
            color_x = 0;
            color_y = 60;
            break;
        case OSD256_FONT_RED_BLINK:
            color_x = 0;
            color_y = 120;
            break;
        case OSD256_FONT_YELLOW:
            color_x = 0;
            color_y = 180;
            break;
        case OSD256_FONT_GREEN:
            color_x = 0;
            color_y = 240;
            break;
        default:
            color_x = 0;
            color_y = 0;
        }
    }
    else
    {
        switch (color)
        {
        case OSD256_FONT_WHITE:
            color_x = 610;
            color_y = 0;
            break;
        case OSD256_FONT_RED:
            color_x = 610;
            color_y = 144;
            break;
        case OSD256_FONT_YELLOW:
            color_x = 610;
            color_y = 288;
            break;
        default:
            color_x = 610;
            color_y = 0;
        }
    }
    src_start_x = src_start_x + color_x;
    src_start_y = src_start_y + color_y;

    OSD256_block_transfer(SCRATCH, DISPLAY, src_start_x, src_start_y, dst_start_x, _pos_y, dst_end_x, dst_end_y);
}

void OSD256_puts(char *str, uint16_t posx, uint16_t posy, uint8_t color, uint8_t font)
{

    uint16_t _posX = posx;

    for (uint8_t a = 0; a < strlen(str); a++)
    {
        OSD256_putc(_posX, posy, str[a] - 32, color, font);
        if (font == 0)
            _posX = _posX + 15;

        else
        {
            if (str[a] == '.')
                _posX = _posX + 9;
            else
                _posX = _posX + 13;
        }
    }
}

void OSD256_printf(uint16_t posx, uint16_t posy, char color, char font, const char *format, ...)
{
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format,
              args); // does not overrun sizeof(buf) including null terminator
    va_end(args);

    OSD256_puts(buf, posx, posy, color, font);
}

void OSD256_printf_slow(uint16_t posx, uint16_t posy, char color, char font, const char *format, ...)
{
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format,
              args); // does not overrun sizeof(buf) including null terminator
    va_end(args);

    byte _posX = posx;

    for (uint8_t a = 0; a < strlen(buf); a++)
    {
        if (font)
            OSD256_write_font1(DISPLAY, _posX, posy, buf[a] - 32, color, 0);
        else
            OSD256_write_font0(DISPLAY, _posX, posy, buf[a] - 32, color, 0);
        _posX++;
    }
}

void OSD256_write_font1(uint8_t dst, uint8_t _pos_x, uint16_t _pos_y, uint8_t _indx, uint8_t color, uint8_t attrib)
{
    uint8_t reg40, tmp;
    const uint8_t *ptr;
    uint16_t tmp1;

    tw_write_register(0x20a, 0x0); //... y path 0x20, x Path 0x00
    tmp1 = _pos_x << 4;            //... (_pos_x)*4 -> 4 pixel * 4 -> 16 pixel char

    _pos_y = _pos_y * FONT1_Y;

    reg40 = tw_read_register(0x240);
    tw_write_register(0x240, (reg40 | 0x1)); // Enable extend OSD feature

    OSD256_OSG_Mode_Selection(1); // Set OSG operation mode to BMP operation

    tw_write_register(0x205, tmp1);

    tmp1 = tmp1 + FONT1_X - 1;

    tw_write_register(0x206, tmp1);

    tmp = ((tmp1 & 0xff00) >> 8 << 6) | ((tmp1 & 0xff00) >> 8 << 4);
    tw_write_register(0x24e, tmp);

    tw_write_register(0x207, _pos_y);

    tmp1 = _pos_y + FONT1_Y - 1;

    tw_write_register(0x208, tmp1);
    tmp = ((_pos_y & 0xff00) >> 8 << 2) | ((tmp1 & 0xff00) >> 8);
    tw_write_register(0x209, tmp);

    tmp1 = (96 * _indx);
    ptr = (ufont_16x24 + tmp1);

    // Set OSG_DSTCTL           0 scratch 1 display
    if (dst == 1)
        tmp = 0x4; // To Display buffer
    else
        tmp = 0; // scratch

    tw_write_register(0x24f, tmp | 0x1);

    for (tmp = 0; tmp < 24; tmp++)
    {
        uint8_t pix, pix_data;
        uint8_t i, j, mask;

        for (i = 0; i < 4; i++)
        {
            mask = 0xC0;

            for (j = 0; j < 4; j++)
            {
                pix = (mask & (*ptr)) >> ((3 - j) << 1);
                pix_data = 0xff; // Any unknown color renders to transparent
                // 0 - empty, 1 - outline, 2- ???, 3-main color
                if (pix == 0) pix_data = COLOR_BLACK | attrib;
                if (pix == 2) pix_data = color | attrib;
                if (pix == 3) pix_data = 0xff;
                tw_write_register(0x200, pix_data);
                mask >>= 2;
            }
            ptr++;
        }

        WAIT_OSG_WRSTALL;
    }
    WAIT_OSG_IDLE;
    tw_write_register(0x240, reg40); // Restore the saved register 2x40 value
}

void OSD256_clear_screen(uint8_t _pth, uint8_t page)
{

    uint8_t tmp;
    tmp = OSD256_wr_page;

    OSD256_wr_page = page;
    OSD256_block_fill(_pth, DISPLAY, 0, 0, 719, 575, 0xff);
    OSD256_wr_page = tmp;
}

void OSD256_setpixel(uint8_t _pth, uint8_t color, uint16_t start_X, uint16_t start_Y)
{
    uint8_t tmp;
    uint16_t end_X, end_Y;

    uint8_t reg205, reg206, reg207, reg208, reg209, reg20a;
    uint8_t reg24e, reg24f;

    end_X = start_X + 1;
    end_Y = start_Y + 1;

    // check boundaries, if dst=1 then destination is display, so check screen boundaries
    if (start_X >= SCR_X_SIZE) start_X = SCR_X_SIZE;
    if (start_Y >= SCR_Y_SIZE) start_Y = SCR_Y_SIZE;
    if (end_X >= SCR_X_SIZE) end_X = SCR_X_SIZE;
    if (end_Y >= SCR_Y_SIZE) end_Y = SCR_Y_SIZE;

    if (BitSet(_pth, PTH_X))
    {
        reg20a = (OSD256_wr_page & 0x7) << 2; //... y path 0x20, x Path 0x00
    }
    else
    {
        reg20a = 0x20; //... y path 0x20, x Path 0x00
        start_X >>= 1;
        end_X >>= 1;
    }

    tw_write_register(0x243, COLOR_WHITE); // Filling color
    tw_write_register(0x241, 0x02);        // Set OSG operation mode to Block fill

    reg205 = start_X;
    reg206 = end_X;
    tmp = ((start_X & 0xff00) >> 8 << 6) | ((end_X & 0xff00) >> 8 << 4);
    reg24e = tmp;

    reg207 = start_Y;
    reg208 = end_Y;
    tmp = ((start_Y & 0xff00) >> 8 << 2) | ((end_Y & 0xff00) >> 8);
    reg209 = tmp;

    reg24f = 0x4 | 0x1; // start

    cnt = 0;
    data_buf[cnt++] = reg205;
    data_buf[cnt++] = reg206;
    data_buf[cnt++] = reg207;
    data_buf[cnt++] = reg208;
    data_buf[cnt++] = reg209;
    data_buf[cnt++] = reg20a;
    tw_write_buf(0x205, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = reg24e;
    data_buf[cnt++] = reg24f;
    tw_write_buf(0x24e, data_buf, cnt);

    WAIT_OSG_IDLE; // do we really need this ?
}

void OSD256_setpixel_fast(uint8_t pth, uint16_t start_X, uint16_t start_Y)
{
    //	uint8_t tmp;
    uint16_t end_X, end_Y;

    uint8_t reg205, reg206, reg207, reg208, reg209;
    uint8_t reg24e;

    if (start_X < osd_draw_boundary.x0 || start_X > osd_draw_boundary.x1) return;
    if (start_Y < osd_draw_boundary.y0 || start_Y > osd_draw_boundary.y1) return;

    if (pth == PTH_Y)
    {
        start_X >>= 1;
    }

    end_X = start_X + 1;
    end_Y = start_Y + 1;

    reg205 = start_X;
    reg206 = end_X;
    reg24e = ((start_X & 0xff00) >> 8 << 6) | ((end_X & 0xff00) >> 8 << 4);

    reg207 = start_Y;
    reg208 = end_Y;
    reg209 = ((start_Y & 0xff00) >> 8 << 2) | ((end_Y & 0xff00) >> 8);

    cnt = 0;
    data_buf[cnt++] = reg205;
    data_buf[cnt++] = reg206;
    data_buf[cnt++] = reg207;
    data_buf[cnt++] = reg208;
    data_buf[cnt++] = reg209;
    tw_write_buf(0x205, data_buf, cnt);

    cnt = 0;
    data_buf[cnt++] = reg24e;
    data_buf[cnt++] = 0x4 | 0x1; // start and to display buffer
    tw_write_buf(0x24e, data_buf, cnt);

    WAIT_OSG_IDLE; // do we really need this ?
}

void OSD256_drawline(uint8_t _pth, uint8_t color, int x, int y, int x2, int y2)
{

    // First check for special cases such as vertical or horizontal lines
    if (x == x2) // Vertical line !
    {
        if (y <= y2)
        {
            OSD256_block_fill(_pth, DISPLAY, x, y, x2, y2, color);
            return;
        }
        else
        {
            OSD256_block_fill(_pth, DISPLAY, x, y2, x2, y, color);
            return;
        }
    }

    if (y == y2) // Horizontal line!
    {
        if (x <= x2)
        {
            OSD256_block_fill(_pth, DISPLAY, x, y, x2, y2 + 1, color); // A line must be at least 2 pixel tall to shown on both FIELD
            return;
        }
        else
        {
            OSD256_block_fill(_pth, DISPLAY, x2, y, x, y2 + 1, color);
            return;
        }
    }

    bool yLonger = false;
    int shortLen = y2 - y;
    int longLen = x2 - x;

    uint8_t reg20a;

    OSD256_set_drawcolor(color);
    tw_write_register(0x241, 0x02);

    if (BitSet(_pth, PTH_X))
    {
        reg20a = (OSD256_wr_page & 0x7) << 2; //... y path 0x20, x Path 0x00
    }
    else
    {
        reg20a = 0x20; //... y path 0x20, x Path 0x00
    }

    tw_write_register(0x20a, reg20a);

    if (abs(shortLen) > abs(longLen))
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
                OSD256_setpixel_fast(_pth, j >> 16, y);
                j += decInc;
            }
            return;
        }
        longLen += y;
        for (int j = 0x8000 + (x << 16); y >= longLen; --y)
        {
            OSD256_setpixel_fast(_pth, j >> 16, y);
            j -= decInc;
        }
        return;
    }

    if (longLen > 0)
    {
        longLen += x;
        for (int j = 0x8000 + (y << 16); x <= longLen; ++x)
        {
            OSD256_setpixel_fast(_pth, x, j >> 16);
            j += decInc;
        }
        return;
    }
    longLen += x;
    for (int j = 0x8000 + (y << 16); x >= longLen; --x)
    {
        OSD256_setpixel_fast(_pth, x, j >> 16);
        j -= decInc;
    }
}

void OSD256_circle(uint8_t _pth, uint8_t color, int xCenter, int yCenter, int radius)
{
    int tSwitch, y, x = 0;
    int d;
    uint8_t reg20a;

    OSD256_set_drawcolor(color);
    tw_write_register(0x241, 0x02);

    if (BitSet(_pth, PTH_X))
    {
        reg20a = (OSD256_wr_page & 0x7) << 2; //... y path 0x20, x Path 0x00
    }
    else
    {
        reg20a = 0x20; //... y path 0x20, x Path 0x00
    }
    tw_write_register(0x20a, reg20a);

    d = yCenter - xCenter;
    y = radius;
    tSwitch = 3 - 2 * radius;

    while (x <= y)
    {

        OSD256_setpixel_fast(_pth, xCenter + x, yCenter + y);
        OSD256_setpixel_fast(_pth, xCenter + x, yCenter - y);
        OSD256_setpixel_fast(_pth, xCenter - x, yCenter + y);
        OSD256_setpixel_fast(_pth, xCenter - x, yCenter - y);

        OSD256_setpixel_fast(_pth, yCenter + y - d, yCenter + x);
        OSD256_setpixel_fast(_pth, yCenter + y - d, yCenter - x);

        OSD256_setpixel_fast(_pth, yCenter - y - d, yCenter + x);
        OSD256_setpixel_fast(_pth, yCenter - y - d, yCenter - x);

        if (tSwitch < 0)
        {
            tSwitch += (4 * x + 6);
        }
        else
        {
            tSwitch += (4 * (x - y) + 10);
            y--;
        }
        x++;
    }
}
void OSD256_set_drawcolor(uint8_t color) { tw_write_register(0x243, color); }

void OSD256_box(uint8_t _pth, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color) { OSD256_block_fill(_pth, DISPLAY, x, y, x + w - 1, y + h - 1, color); };

void OSD256_angled_line(uint8_t _pth, uint8_t _color, uint16_t center_x, uint16_t center_y, uint16_t length, uint16_t angle)
{ 
	uint16_t sx, sy, ex, ey;
    float angle_radian;
	
	angle_radian = DEG2RAD(angle);

    sx = center_x + cos(angle_radian) * -length;
    sy = center_y + sin(angle_radian) * -length;
    ex = center_x + cos(angle_radian) * length;
    ey = center_y + sin(angle_radian) * length;

	OSD256_drawline(_pth, _color, sx, sy, ex, ey);
}


void OSD256_transform_polygon(struct polygon_t *p, int x, int y, int rot)
{
    struct point_t *pt = p->points;
    uint8_t i;
    float angle = DEG2RAD(rot);
    float cos_rot = cos(angle) * 10000;
    float sin_rot = sin(angle) * 10000;
    long cos_l = (long)cos_rot;
    long sin_l = (long)sin_rot;

    long xr, yr;

    for (i = 0; i < p->len; i++)
    {
        xr = ((cos_l * pt->x) - (sin_l * pt->y));
        xr = (xr + 5000) / 10000;
        yr = ((cos_l * pt->y) + (sin_l * pt->x));
        yr = (yr + 5000) / 10000;
        pt->x = (int)xr;
        pt->y = (int)yr;
        pt->x += x;
        pt->y += y;
        pt++;
    }
}

void OSD256_move_polygon(struct polygon_t *p, int x, int y)
{
    struct point_t *pt = p->points;
    uint8_t i;

    for (i = 0; i < p->len; i++)
    {
        pt->x += x;
        pt->y += y;
        pt++;
    }
}

void OSD256_scale_polygon(struct polygon_t *p, float scale)
{

    struct point_t *pt = p->points;
    uint8_t i;

    for (i = 0; i < p->len; i++)
    {
        pt->x = pt->x * scale;
        pt->y = pt->y * scale;
        pt++;
    }
}

void OSD256_draw_polygon(struct polygon_t *p, char color)
{
    uint8_t i;
    struct point_t *pt1 = p->points;
    struct point_t *pt2 = pt1 + 1;

    for (i = 1; i < p->len; i++)
    {
        OSD256_drawline(PTH_X, color, pt1->x, pt1->y, pt2->x, pt2->y);
        pt1++;
        pt2++;
    }
    pt2 = p->points;
    OSD256_drawline(PTH_X, color, pt1->x, pt1->y, pt2->x, pt2->y);
}

// Display scratch memory (check fonts and bitmaps) and stops program
void OSD256_displayscratch()

{
    OSD256_wr_page = 0;
    OSD256_set_display_page(0);
    OSD256_block_transfer(SCRATCH, DISPLAY, 1023-719, 0, 0, 0, 719, 431);
    while (1)
        ;
}