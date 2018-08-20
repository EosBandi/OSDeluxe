
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

#ifndef _UTILS_H_
#define _UTILS_H_

#define SetBit(x, y) ((x) |= (y))
#define ClearBit(x, y) ((x) &= ~(y))
#define BitSet(x, y) (((x) & (y)) == (y))
#define BitClear(x, y) (((x) & (y)) == 0)
#define IfBitSet(x, y) if (((x) & (y)) == (y))
#define IfBitClear(x, y) if (((x) & (y)) == 0)

#define BIT0 (0x0001)
#define BIT1 (0x0002)
#define BIT2 (0x0004)
#define BIT3 (0x0008)
#define BIT4 (0x0010)
#define BIT5 (0x0020)
#define BIT6 (0x0040)
#define BIT7 (0x0080)

#define setWriteDirInline()  \
    {                        \
        pinMode(2, OUTPUT);  \
        pinMode(14, OUTPUT); \
        pinMode(7, OUTPUT);  \
        pinMode(8, OUTPUT);  \
        pinMode(6, OUTPUT);  \
        pinMode(20, OUTPUT); \
        pinMode(21, OUTPUT); \
        pinMode(5, OUTPUT);  \
    }
#define setReadDirInline()  \
    {                       \
        pinMode(2, INPUT);  \
        pinMode(14, INPUT); \
        pinMode(7, INPUT);  \
        pinMode(8, INPUT);  \
        pinMode(6, INPUT);  \
        pinMode(20, INPUT); \
        pinMode(21, INPUT); \
        pinMode(5, INPUT);  \
    }

#define NOP3  \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t"
#define NOP4  \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t"
#define NOP6  \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t" \
    "nop\n\t"
// P1-5 are 100-500 ns pauses, tested with an oscilloscope (2 second
// display persistence) and a Teensy 3.2 compiling with
// Teensyduino/Arduino 1.8.1, "faster" setting
#if F_CPU == 96000000
#define P1 __asm__(NOP4 NOP4)
#define P2 __asm__(NOP6 NOP6 NOP6)
#define P3 __asm__(NOP6 NOP6 NOP6 NOP6 NOP3)
#define P4 __asm__(NOP6 NOP6 NOP6 NOP6 NOP6 NOP4 NOP4)
#define P5 __asm__(NOP6 NOP6 NOP6 NOP6 NOP6 NOP6 NOP6 NOP4 NOP3)

#elif F_CPU == 72000000
#define P1 __asm__(NOP6)
#define P2 __asm__(NOP6 NOP6)
#define P3 __asm__(NOP6 NOP6 NOP6 NOP3)
#define P4 __asm__(NOP6 NOP6 NOP6 NOP6 NOP4)
#define P5 __asm__(NOP6 NOP6 NOP6 NOP6 NOP4 NOP4 NOP3)

#elif F_CPU == 48000000
#define P1 __asm__(NOP4)
#define P2 __asm__(NOP6 NOP3)
#define P3 __asm__(NOP6 NOP4 NOP3)
#define P4 __asm__(NOP6 NOP6 NOP6)
#define P5 __asm__(NOP6 NOP6 NOP4 NOP4 NOP3)

#endif

#endif

