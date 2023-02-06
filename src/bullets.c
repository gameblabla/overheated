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
 bullets.c
********************************************/

#ifndef BULLETS_C
#define BULLETS_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "objects.h"
#include "d_Charset.h"
#include "enemies.h"
#include "bullets.h"
#include "globalVars.h"
#include "effects.h"
//
#include "game.h"

/***************************************************
    stdBullet
****************************************************/

void stdBullet( GameObject *object)
{
    stdBulletAttributes *objAttributes = (stdBulletAttributes *)object->attributes;
	if( object->collision != 0)
	{

		//create a new explosion
		spawnStdBulletExplosion(object->pos);
		//delete bullet object
		freeANDpushObj( &OAM_PlBullets , object);
		return;
	}

    //If the bullet leaves the screen kill it
	if(   object->pos.x > GM_SCREEN_W
	   || object->pos.x < 0
	   || object->pos.y > GM_SCREEN_H
	   || object->pos.y < 0)
	{
		freeANDpushObj( &OAM_PlBullets , object);
		return;
	}

	object->pos.x += objAttributes->xdir;
	object->pos.y += objAttributes->ydir;

	return;
}

//-------------------------------------------

/***************************************************
    new_stdBullet
****************************************************/

void new_stdBullet( GameObject  *object , int xdir , int ydir)
{
	object->func = &stdBullet;
	object->attributes = malloc( sizeof(stdBulletAttributes) );
	((stdBulletAttributes *)object->attributes)->xdir = xdir;
	((stdBulletAttributes *)object->attributes)->ydir = ydir;
	return;

}

//-------------------------------------------
