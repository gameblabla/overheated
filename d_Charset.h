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
    d_Charset.h
    Charset structure definition 
*********************************************/

#ifndef D_CHARSET_H
#define D_CHARSET_H

struct charSet
{
	SDL_Surface *gfx;	    //Charset graphic data

	Uint16 tilesPerLine;	//How many tiles are in the same Horizontal line
				            //in the graphic data.
				            //*Needed to calculate the tiles number

	Uint8  tileW;		    //Tiles width
	Uint8  tileH;		    //Tiles heigth
};

typedef struct charSet charSet;

struct charSetArray
{
	Uint16 size;		//Number of elements
	charSet *array;		//Charset array
};

typedef struct charSetArray charSetArray;

#endif


