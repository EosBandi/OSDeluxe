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


#ifndef GRAPHICS_H
#define	GRAPHICS_H

struct point {
    int x, y;
};

struct point3d {
    int x, y, z;
};

struct polygon {
    struct point *points;
    U8 len;
};

void transform_polygon(struct polygon *p, int x, int y, int rot);
void move_polygon(struct polygon *p, int x, int y);
void draw_polygon(struct polygon *p, char color);
void draw_circle(int xm, int ym, int r);
void scale_polygon(struct polygon *p, float scale);

#endif