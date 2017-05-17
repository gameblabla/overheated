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
    spawn.c
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_spawn.h"
#include "globalVars.h"
//
#include "debug.h"
#include "enemies.h"
#include "objects.h"

/*********************************************/

static int spawnHalt = 0;

/*********************************************/


/********************************************
    spawn_INIT
*********************************************/
void spawn_INIT(void)
{
	spawnHalt = 0;
	return;
}
/********************************************
    spawnIsHalted
*********************************************/
int spawnIsHalted(void)
{
	return spawnHalt;
}
/********************************************
    pauseSpawn
*********************************************/
void pauseSpawn(void)
{
	spawnHalt++;
	return;
}
/********************************************
    pauseSpawn
*********************************************/
void resumeSpawn(void)
{
    if(spawnHalt > 0)
	spawnHalt--;
	return;
}
/********************************************
    spawnObj
*********************************************/
void spawnObj( objectSpawn *spawn)
{

	if(spawn->func == NULL)
		return;

	//Call spawn function
	(*( void (*) (SDL_Rect))(spawn->func))(spawn->pos);
	return;
}

int spawnObjTable( spawnTable *table , unsigned stageLine , int stoppable)
{
	if(spawnIsHalted()
	    && stoppable)
		return 0;

    if( table->spawns == NULL)
        return SPAWN_TABLE_END;

	if( table->line > stageLine)
		return 0;

	register i = table->num-1;
	for( ; i >= 0 ; i--)
		spawnObj(table->spawns+i);

	return OBJ_SPAWNED;
}


