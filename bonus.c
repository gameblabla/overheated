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
   bonus.c
*********************************************/

#ifndef BONUS_C
#define BONUS_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "d_GameObject.h"
#include "d_Charset.h"
#include "globalDefines.h"
#include "globalVars.h"
#include "l_sfx.h"
#include "d_trajectory.h"
#include "l_trajectories.h"
#include "d_bonus.h"
//
#include "d_input.h"
#include "objects.h"
#include "effects.h"
#include "player.h"
#include "bonus.h"
#include "debug.h"
#include "sfx.h"
#include "score.h"
#include "trajectory.h"

//-------------------------------------------

/********************************************
    levelUp
*********************************************/

void levelUp( GameObject *object)
{

    powerUpAttributes *objAttributes = (powerUpAttributes *)object->attributes;

	if( object->collision > 0)
	{
        if(playerWeaponAtMaxLevel())
            { incScore(POWERUP_BONUS_POINTS); }
        else
		    { levelUpPlayerWeapon(); }

        playEffect(SFX_POWERUP_1);
		freeANDpushObj(&OAM_bonus , object);
		return;
	}
    //Delete if it is out of the screen
	if( object->pos.y > GM_SCREEN_H+16)
	{
		freeANDpushObj(&OAM_bonus , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}

//-------------------------------------------

void newLevelUp( GameObject  *object)
{
	object->func = &levelUp;

	//Allocate and initialize attributes
	object->attributes = malloc( sizeof(powerUpAttributes) );
    //
    powerUpAttributes *objAttributes = (powerUpAttributes *)object->attributes;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , bonus_trajectory );

	return;
}

//-------------------------------------------

void spawnLevelUp(SDL_Rect pos)
{
	GameObject *newObjPtr;
	//GmObjectAnim LevelUpAnim = { 0 , 0 , 0 , 0 , 0 , 0};
	GmObjectAnim LevelUpAnim = { 0 , 6 , 6 , 3 , 0 , 1};

	if( searchEmptyObject( &OAM_bonus) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 2 , 2 , 14 , 14};

		newObjPtr = newObjANDpull( &OAM_bonus , &pos , &colBox
					 , BONUS_16X16_CHARSET , 0 , 0 , 0 , &LevelUpAnim);
		newLevelUp( newObjPtr);
	}
	return;
}

//-------------------------------------------

/********************************************
    standardBonus
*********************************************/

void standardBonus( GameObject *object)
{
    standardBonusAttributes *objAttributes = (standardBonusAttributes *)object->attributes;

	if( object->collision > 0)
	{
        incScore( objAttributes->sPoints );
		playEffect(SFX_BONUS_1);
		freeANDpushObj(&OAM_bonus , object);
		return;
	}
    //Delete if it is out of the screen
	if( object->pos.y > GM_SCREEN_H+16)
	{
		freeANDpushObj(&OAM_bonus , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}
//-------------------------------------------

void newStandardBonus( GameObject  *object , int sPoints)
{
	object->func = &standardBonus;

	//Allocate and initialize attributes
	object->attributes = malloc( sizeof(standardBonusAttributes) );

    standardBonusAttributes *objAttributes = (standardBonusAttributes *)object->attributes;
    objAttributes->sPoints = sPoints;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , bonus_trajectory );

	return;
}

//-------------------------------------------

void spawnSmallBonus(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim bonusAnim = { SMALL_BONUS_TILE , 0 , 0 , 0 , 0 , 0};

	if( searchEmptyObject( &OAM_bonus) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 2 , 2 , 14 , 14};

		newObjPtr = newObjANDpull( &OAM_bonus , &pos , &colBox
					 , BONUS_16X16_CHARSET , SMALL_BONUS_TILE , 0 , 0 , &bonusAnim);
		newStandardBonus( newObjPtr , SMALL_BONUS_POINTS );
	}
	return;
}

//-------------------------------------------


