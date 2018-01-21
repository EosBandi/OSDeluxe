
#ifndef GRAPHICS_H
#define	GRAPHICS_H

struct point {
    int x, y;
};
//
//struct point3d {
    //s32 x, y, z;
//};

struct polygon {
    struct point *points;
    unsigned char len;
};

void transform_polygon(struct polygon *p, int x, int y, int rot);
void move_polygon(struct polygon *p, int x, int y);
void draw_polygon(struct polygon *p);
void draw_circle(int xm, int ym, int r);
#endif