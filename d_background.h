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


/*******************************************
    d_background.h
    Background data types definitions
*******************************************/

#ifndef D_BACKGROUND_H
#define D_BACKGROUND_H

struct bgRect
{
	int w;
	int h;
};

typedef struct bgRect bgRect;

//-------------------------------------------

struct bgCoord
{
	int x;
	int y;
};

typedef struct bgCoord bgCoord;

//-------------------------------------------

struct bgScroll
{
	int loopStart;		//Scroll loop will start on this tilemap row
	int loopEnd;		//Scroll loop will end   on this tilemap row
	int loopRepeat;		//0 = No repeat(stop when the end is reached)

	bgCoord tileScroll;	//Scroll each tile by this value
                        //Used for 1px scrolling of the bg

    int rowOffset;

	int inc;            //tileScroll is incremented by adding this variable

	int framesDelay;	//Number of frames between scroll increments
	int framesCounter;	//Number of frames remaining until the next increment
};

typedef struct bgScroll bgScroll;

//-------------------------------------------

struct bgInstruction
{
	void *func	;//Operation function
	int  *operands	;//Operands array
};

typedef struct bgInstruction bgInstruction;

//-------------------------------------------

struct bgScript
{
	int frame;			//frame where the instructions
					    //have to be executed
	int insNum;			//Number of instructions
	bgInstruction *ins;		//Instruction array
};

typedef struct bgScript bgScript;

//-------------------------------------------

struct bgTilemap
{
    int w;                //Width
    int h;                //Heigth
    int tilesNumber;      //Tiles Number
    Uint16 *data;         //Pointer to the tilemap data 
    GameObject *tileObj;  //Tile objects array
};

typedef struct bgTilemap bgTilemap;

//-------------------------------------------

struct background
{
	bgCoord pos;             //Position on the background
	bgScroll scroll;         //Scroll variables
    int interactive;         //the game objects can interact with the map
	int scriptSize;          //Number of scripts
	int currentScript;       //Current script
	bgScript *script;        //Background scripts
	bgTilemap tilemap;       //tilemap data
    bgTilemap tilemapBuffer; //background tilemap buffer
	charSet *tileset;        //Pointer to the tileset

};

typedef struct background background;

//-------------------------------------------

struct bgArray
{
	int number;		//Number of backgrounds
	background *bg;		//Backgrounds counter

};

typedef struct bgArray bgArray;

#endif
