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


int wrap360(int x) {
	if (x >= 360) x = x % 360;
	if (x < 0) x = x + 360;
	return x;
}
float angle2rad(int a) { return (a * (PI / 180)); }