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
 d_player.h
********************************************/

#ifndef D_PLAYER_H
#define D_PLAYER_H

#define PL_TRANSPARENT_TILE 0
#define PL_STILL_TILE 1
#define PL_LEFT_TILE 2
#define PL_RIGHT_TILE 3
#define PL_SPRITE_RIGHT_PANING 18

//-------------------------------------------

struct PLweapon             //Player weapon
{
	int type;               //Weapon type
	int maxLevel;           //Max weapon level
	int level;              //Current weapon level
	int mode;               //Weapon mode
	int rounds;             //Remaining rounds
	int roundsPerRefill;
	int refillDelay;        //Refill delay (in frames)
	int refillCounter;      //Refill delay counter ( in frames)
};

typedef struct PLweapon PLweapon;

//-------------------------------------------

struct playerAttributes //Player attributes
{
	PLweapon weapon;
    int respawnCounter; //Frames until player respawn
    int spawnFrames;    //Number of spawn frames remaining
    int invisible;

};

typedef struct playerAttributes playerAttributes;

#endif
