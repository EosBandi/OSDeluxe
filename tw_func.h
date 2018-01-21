

#define RAD2DEG(x) ((x) * (180.0 / PI))
#define DEG2RAD(x) ((x) * (PI / 180.0))

#define COLOR_BLACK      0
#define COLOR_25_WHITE   1
#define COLOR_50_WHITE   2
#define COLOR_75_WHITE   3
#define COLOR_WHITE      4
#define COLOR_BLUE       5
#define COLOR_RED        6
#define COLOR_ORANGE     7
#define COLOR_CYAN       8
#define COLOR_MAGENTA    9
#define COLOR_GREEN      10
#define COLOR_OCEAN      11
#define COLOR_YELLOW     12
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

#define CH_POPUP 1
#define CH_NOPOPUP 0


#define FLD_NONE 0
#define FLD_EVEN 1
#define FLD_ODD  2
#define FLD_BOTH 3

#define OSD_PATH_DISP 0
#define OSD_PATH_REC  1


#define SCREEN_SCALE 1.6f

enum FontType {
    FONT_8x8	          = 0x01,
    FONT_16x8	          = 0x02,
    FONT_16x16	          = 0x03,
    FONT_SHADOW_8x8       = 0x04,
    FONT_SHADOW_16x8      = 0x05,
    FONT_SHADOW_16x16     = 0x06,
};

extern FontType font_type;      //Global variable to hold current font type

extern unsigned char cnt, count, data_buf[20];
extern unsigned int  ADDR_buf, count_TW2835;
extern unsigned char color, back_color;
extern unsigned char OSD_work_field;
extern unsigned char OSD_path; //0-display, 1-record
extern unsigned char rec_shadow_color, rec_color, rec_back_color;


const unsigned char colortable[13][3] = {
{0x00, 0x80, 0x80},							//Black         0
{0x3f, 0x80, 0x80},							//25%white      1
{0x7f, 0x80, 0x80},							//50%white      2
{0xc0, 0x80, 0x80},							//75%white      3
{0xf0, 0x80, 0x80},							//100%white     4
{0x1d, 0xff, 0x6b},							//Blue          5
{0x4c, 0x54, 0xff},							//Red           6   
{0x75, 0x4f, 0xb5},							//Orange        7
{0xb2, 0xaa, 0x00},							//Cyan          8
{0x69, 0xd4, 0xea},							//Magenta       9
{0x4b, 0x55, 0x4a},							//Green         10
{0x61, 0xb5, 0x3a},							//Ocean ??      11
{0xe1, 0x00, 0x94}							//Yellow        12
};

const unsigned char colortable_rec[4][3] = {
{0x00, 0x80, 0x80},							//Black         0
{0xf0, 0x80, 0x80},							//100%white     4
{0x4c, 0x54, 0xff},							//Red           6   
{0x4b, 0x55, 0x4a}							//Green         10
};

#define COLOR_REC_BLACK 0
#define COLOR_REC_WHITE 1
#define COLOR_REC_RED   2
#define COLOR_REC_GREEN 3
#define COLOR_REC_NONE  0xf
#define REC_MIX         0x8
#define REC_BLINK       0x4

void tw_init();
void tw_write_buf(unsigned int wrADDR, unsigned char *wrBUF, unsigned char wrCNT);


void tw_osd_out_char(unsigned char _pos_X, unsigned int _pos_Y, unsigned char _chr);
void tw_puts(char *str, char posx, unsigned short posy);
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


struct widget_priv {
    int pitch_deg, roll_deg;
    float cos_roll, sin_roll;
    int heading;
};


extern widget_priv priv;



void render_horizon();
void render_roll_indicator();

/**************************** REdo */


void tw_write_register(unsigned int wrADDR, unsigned char content);
unsigned char tw_read_register(unsigned int rdADDR);
void tw_wait_for_osd_write(int timeout);

void tw_ch_set_window (unsigned char _ch, unsigned int _pos_H, unsigned int _pos_V, unsigned int _len_H);
void tw_ch_settings (unsigned char _ch, unsigned char _on_off, unsigned char _popup);

void tw_osd_set_display(char rd_page, char dp_field, char rec_field );
void tw_switch_display_field();

void tw_osd_qsetpixel(unsigned int x, unsigned int y);
void tw_osd_setpixel(unsigned int x, unsigned int y, unsigned char color1, unsigned char color2, unsigned char color3, unsigned char color4);

void tw_osd_fill_region_rec (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char color, unsigned char _field);
void tw_osd_fill_region(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2, unsigned char color, unsigned char _field, unsigned char path);



