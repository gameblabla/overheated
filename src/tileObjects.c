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
   tileObjects.c
*********************************************/

#ifndef TILE_OBJECTS_C
#define TILE_OBJECTS_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_tileObjects.h"
#include "d_background.h"
#include "globalVars.h"
//
#include "cMath.h"
#include "math.h"
#include "objects.h"
#include "enemyBullet.h"
#include "enemies.h"
#include "effects.h"
#include "bonus.h"
#include "tileObjects.h"
#include "debug.h"
#include "time.h"
#include "background.h"
#include "announcer.h"
#include "score.h"
//-------------------------------------------

/********************************************
    container
*********************************************/


void container( GameObject *object)
{
    containerAttributes *objAttributes = (containerAttributes *)object->attributes;

    //Manage collision damage
    if( object->collision >= CONTAINER_LIFE )
	{
        //Change tile character
        *(objAttributes->tilePtr) = DESTROYED_CONTAINER_TILE;
        //
        incScore(CONTAINER_BONUS_POINTS);
        makeSmallTileExplosion( &object->pos);
        //Free object slot
        freeTileObject(object);

        return;
	}

    return;
}

//-------------------------------------------

void newContainer(GameObject  *object , Uint16 *tilePtr)
{
    object->attributes = malloc( sizeof(containerAttributes));
    if(object->attributes == NULL)
        return;

    object->func = &container;
    object->power = 1;
    //
    containerAttributes *objAttributes = (containerAttributes *)object->attributes;
    objAttributes->tilePtr = tilePtr;
    objAttributes->effectObj = NULL;
    
    return;
}

//-------------------------------------------

/********************************************
    sphericalContainer
*********************************************/

void sphericalContainer(GameObject *object)
{
    containerAttributes *objAttributes = (containerAttributes *)object->attributes;

    if( object->collision >= objAttributes->life )
	{
        //Change tile character
        *(objAttributes->tilePtr) = DESTROYED_SPHERERICAL_CONTAINER_TILE;
        //Spawn reward
         ((void (*)(SDL_Rect ))objAttributes->rewardSpawn)( object->pos);
        //Free object slot
		freeTileObject(object);
        return;
	}
    return;

}

//-------------------------------------------

/********************************************
    newPowerUpContainer
*********************************************/

void newPowerUpContainer(GameObject *object , Uint16 *tilePtr)
{
    object->attributes = malloc( sizeof(containerAttributes));
    if(object->attributes == NULL)
        return;

    object->func = &sphericalContainer;
    object->power = 1;

    containerAttributes *objAttributes = (containerAttributes *)object->attributes;

    objAttributes->tilePtr = tilePtr;
    objAttributes->life = SPHERICAL_CONTAINER_LIFE;
    objAttributes->rewardSpawn = &spawnLevelUp;
    
    return;
}
//-------------------------------------------

/********************************************
    newSmallBonusContainer
*********************************************/
void newSmallBonusContainer(GameObject *object , Uint16 *tilePtr)
{
    object->attributes = malloc( sizeof(containerAttributes));
    if(object->attributes == NULL)
        return;

    object->func = &sphericalContainer;
    object->power = 1;

    containerAttributes *objAttributes = (containerAttributes *)object->attributes;

    objAttributes->tilePtr = tilePtr;
    objAttributes->life = SPHERICAL_CONTAINER_LIFE;
    objAttributes->rewardSpawn = &spawnSmallBonus;
    
    return;
}


//-------------------------------------------

/********************************************
    specialContainer
*********************************************/


void specialContainer( GameObject *object)
{
    containerAttributes *objAttributes = (containerAttributes *)object->attributes;

    //Manage collision damage
    if( object->collision >= objAttributes->life )
	{
        //Change tile character
        *(objAttributes->tilePtr) = DESTROYED_SPHERERICAL_CONTAINER_TILE;
        //
        incScore(SPECIAL_BONUS_POINTS);

        setSpecialBonusMsg(SPECIAL_BONUS_POINTS);

        makeSmallTileExplosion( &object->pos);
        //Free object slot
        freeTileObject(object);

        return;
	}

    return;
}

//-------------------------------------------

/********************************************
    newSpecialContainer
*********************************************/
void newSpecialContainer(GameObject *object , Uint16 *tilePtr)
{
    object->attributes = malloc( sizeof(containerAttributes));
    if(object->attributes == NULL)
        return;

    object->func = &specialContainer;
    object->power = 1;

    containerAttributes *objAttributes = (containerAttributes *)object->attributes;

    objAttributes->tilePtr = tilePtr;
    objAttributes->life = SPECIAL_CONTAINER_LIFE;
    objAttributes->rewardSpawn = NULL;
    
    return;
}


//-------------------------------------------
