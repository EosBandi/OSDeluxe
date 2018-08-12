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

#ifndef OSDELUXE_H
#define	OSDELUXE_H


#define VERSION "1.0b"
#define BUILD __DATE__




// Common definitions used all over the code
//#define uint8_t  unsigned char 
//#define uint16_t unsigned short


#include <Arduino.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <EEPROM.h>
#include "i2c_t3.h"

#include "strings.h"
#include "home_calc.h"
#include "globals.h"
#include "utils.h"
#include "tw_font.h"
#include "tw_func.h"
#include "bitmaps.h"
#include "osd_widgets.h"
#include "s1debug.h"
#include "settings.h"
#include "mavlink.h"
#include "params.h"
#include "helpers.h"


#endif


