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
    d_GameObjects.h
    Game object type definition
********************************************/

#ifndef D_GAME_OBJECT_H
#define D_GAME_OBJECT_H

struct GmObjectAnim
{
	int base;		    // Base tile
	int frames;		    // Number of frames of the animation
	int counter;		// Number of frames left ( counter == 0 , animation end)
                        // *If the animation ends and the repeat flag is set , then counter = frames
	unsigned delay;		// Delay between frames
	unsigned timer;		// Number of delays until the next animation frame
                        // (Each time that the animation frame changes: timer = delay)
	unsigned flags;		// Animation flags
};

//Animation flags defines:
#define ANIMATION_NO_REPEAT 0x00
#define ANIMATION_REPEAT 0x01

typedef struct GmObjectAnim GmObjectAnim;

struct GameObject
{
	SDL_Rect pos;		//Object graphic rectangle
	SDL_Rect colBox;	//Collision box
	Uint16 chrData;		//Index on charSetArray sprites_charSets ( see OAM.c)
	Uint16 tile;		//Tile the object is using
	Uint8  properties;	//Binary flag , properties of the object (To be implemented);
	GmObjectAnim anim;	//Animation structure

	int power;          //Damage caused when it hits another object
	int collision;      // >0 if there is a collision
                        //The value of collision indicates the amount of damage 
                        //that one object has taken from collisions

	void *func;	        //Pointer to the function that handles the object behaviour
	void *attributes;	//Pointer to the object atributes
};

typedef struct GameObject GameObject;

struct GameObjectArray
{
	int size;			//Size of the array
	int active;			//Number of active objects
	GameObject *object;	//Pointer to the array

	int nxtFree;		        //freeObjStack Stack index
	GameObject **freeObjStack;	//Stack for keeping track of which object
					            //slots are free.
					            //On top of the stack , pointer to the most recently 
					            //released object.
					            //On the bottom the oldest.
					            //Same number of elements that the objects array
};

typedef struct GameObjectArray GameObjectArray;

#endif
