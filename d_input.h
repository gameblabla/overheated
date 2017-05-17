/*
    OverHeated alpha 0.2
    Copyright (C) 2015-2017 BrokenTestType

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


/********************************************
    d_input.h
*********************************************/

#ifndef D_INPUT_H
#define D_INPUT_H

#define AXIS_THRESHOLD 16383

struct joypadStruct
{
	Uint16 A;
	Uint16 B;
	Uint16 X;
	Uint16 Y;

	Uint16 SELECT;
	Uint16 START;

	Uint16 UP;
	Uint16 DOWN;
	Uint16 LEFT;
	Uint16 RIGHT;

};

typedef struct joypadStruct joypadStruct;

struct controllerStruct
{
    joypadStruct raw;       // Raw input ( buttons that are pressed)
    joypadStruct old;       // Last frame Raw input
    joypadStruct held;      // Buttons held for more than one frame
    joypadStruct pressed;   // NEW buttons pressed this frame

};

typedef struct controllerStruct controllerStruct;

struct inputConf
{
	unsigned keyboard;
	unsigned joypad;

	Uint32 jaxisX;
	Uint32 jaxisY;
	Uint32 axisThreshold;

	joypadStruct key;
	joypadStruct jbutton;

};

typedef struct inputConf inputConf;

struct joypadRecordEntry
{
    void *next;         //Pointer to the next record
    joypadStruct data;  //Raw joypad input
};

typedef struct joypadRecordEntry joypadRecordEntry;

struct joypadRecording
{
    joypadRecordEntry *last;
    joypadRecordEntry *record;
    joypadRecordEntry *readPos; //Next record to be readed
    
};

typedef struct joypadRecording joypadRecording;

#endif
