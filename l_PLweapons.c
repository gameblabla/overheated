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
    l_PLweapons.h
*******************************************/

#ifndef L_PLWEAPONS_C
#define L_PLWEAPONS_C
#endif

//
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_input.h"
#include "globalVars.h"
//
#include "enemies.h"
#include "d_player.h"
#include "weapon.h"

//-------------------------------------------

PLweapon PLweaponsList[] = {
	//First weapon is empty
	{
		0,0,0,0,0,0,0,0
	},
	//Multi-directional shoot
	{
		1, //type
		3, //max level
		0, //Level
		0, //Mode
		0, //Rounds
		1, //Rounds per refill
		5, //Refill delay
		0, //Refill counter
	}
};

//-------------------------------------------

//Multi-directional shoot
bulleTmp multShootLevels[][MAX_BULLETS_PER_ROUND] =
{
	//level0
	{
		 { 2, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-2, 11, 0}
		,{ 2, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-2, 5, 0}
	},
	//level1
	{
		 { 3, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-2, 11, 0}
		,{ 3, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-2,  5, 0}
		,{ 3, {6 , 5 , 5 , 4} , 1, 0,PL_BULLET_SPEED*2,   9, 16}
	},
	//level 3
	{
		 { 5, {6 , 5 , 5 , 4} , 4,-8,PL_BULLET_SPEED*-3, 8, 0}
		,{ 5, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-3, 8, -1}
		,{ 5, {6 , 5 , 5 , 4} , 5, 8,PL_BULLET_SPEED*-3,10, 0}
		,{ 5, {6 , 5 , 5 , 4} , 3,-8,PL_BULLET_SPEED*3,  9, 7}
		,{ 5, {6 , 5 , 5 , 4} , 2, 8,PL_BULLET_SPEED*3,  9, 7}
	},
	//level 4
	{
		 { 6, {6 , 5 , 5 , 4} , 4,-8,PL_BULLET_SPEED*-3, 5, 0}
		,{ 6, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-2, 11, 0}
		,{ 6, {6 , 5 , 5 , 4} , 0, 0,PL_BULLET_SPEED*-2, 5, 0}
		,{ 5, {6 , 5 , 5 , 4} , 5, 8,PL_BULLET_SPEED*-3, 13, 0}
		,{ 6, {6 , 5 , 5 , 4} , 3,-8,PL_BULLET_SPEED*3,  9, 7}
		,{ 6, {6 , 5 , 5 , 4} , 2, 8,PL_BULLET_SPEED*3,  9, 7}
	}
};
