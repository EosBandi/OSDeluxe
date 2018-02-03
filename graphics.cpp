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


void draw_circle(int xm, int ym, int r)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
   do {
      tw_osd_qsetpixel(xm-x, ym+y); /*   I. Quadrant */
      tw_osd_qsetpixel(xm-y, ym-x); /*  II. Quadrant */
      tw_osd_qsetpixel(xm+x, ym-y); /* III. Quadrant */
      tw_osd_qsetpixel(xm+y, ym+x); /*  IV. Quadrant */
      r = err;
      if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
      if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}

void transform_polygon(struct polygon *p, int x, int y, int rot)
{
    struct point *pt = p->points;
    unsigned char i;
    float angle = DEG2RAD(rot);
    float cos_rot = cos(angle) * 10000;
    float sin_rot = sin(angle) * 10000;
    long cos_l = (long) cos_rot;
    long sin_l = (long) sin_rot;

    long xr, yr;

    for (i = 0; i < p->len; i++) {
        xr = ((cos_l * pt->x) - (sin_l * pt->y));
        xr = (xr + 5000) / 10000;
        yr = ((cos_l * pt->y) + (sin_l * pt->x));
        yr = (yr + 5000) / 10000;
        pt->x = (int) xr;
        pt->y = (int) yr;
        pt->x += x;
        pt->y += y;
        pt++;
    }
}

void move_polygon(struct polygon *p, int x, int y)
{
    struct point *pt = p->points;
    unsigned char i;

    for (i = 0; i < p->len; i++) {
        pt->x += x;
        pt->y += y;
        pt++;
    }
}

void draw_polygon(struct polygon *p)
{
   unsigned char i;
   struct point *pt1 = p->points;
   struct point *pt2 = pt1+1;

    for (i = 1; i < p->len; i++) {
        tw_osd_drawline(pt1->x, pt1->y, pt2->x, pt2->y);
        pt1++;
        pt2++;
    }
    pt2 = p->points;
    tw_osd_drawline(pt1->x, pt1->y, pt2->x, pt2->y);
}
