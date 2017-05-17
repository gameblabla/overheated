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
    d_spawn.h
    Define spawn structures   
*********************************************/

#define OBJ_SPAWNED 1
#define SPAWN_TABLE_END 2

#ifndef D_SPAWN_H
#define D_SPAWN_H

/*********************************************/

struct objectSpawn
{
	SDL_Rect pos;		//Position where the object will spawn
	//(...)
	void *func;		//Function that will spawn the object
};

typedef struct objectSpawn objectSpawn;

/*********************************************/

struct spawnTable
{
	int line;			//Stage line where the objects will be spawn
	int num;			//Number of objects to be spawned
	objectSpawn *spawns;		//spawns
};

typedef struct spawnTable spawnTable;

#endif
