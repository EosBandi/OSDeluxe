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

void debug (const char *format, ...)
{
    char buf[128];
    va_list args;
    va_start (args, format);
    vsnprintf (buf, sizeof (buf), format,
                args); // does not overrun sizeof(buf) including null terminator
    va_end (args);
    // the below assumes that the new data will fit into the I/O buffer. If not, Serial may drop it.
    // if Serial had a get free buffer count, we could delay and retry. Such does exist at the
    // device class level, but not at this level.
    Serial.write (buf); // move chars to I/O buffer, freeing up local buf
    return;
}

