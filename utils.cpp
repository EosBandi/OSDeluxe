
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



uint32_t FreeRam() { // for Teensy 3.0
	uint32_t stackTop;
	uint32_t heapTop;

	// current position of the stack.
	stackTop = (uint32_t)&stackTop;

	// current position of heap.
	void* hTop = malloc(1);
	heapTop = (uint32_t)hTop;
	free(hTop);

	// The difference is the free, available ram.
	return stackTop - heapTop;
}

