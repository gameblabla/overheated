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
    l_objects.c
    List of object names and functions
*********************************************/

#ifndef L_OBJECTS_C
#define L_OBJECTS_C
#endif

//
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "globalVars.h"
//
#include "l_objects.h"
#include "enemies.h"
#include "enemyBullet.h"
#include "effects.h"
#include "espEnemies.h"


//Objects name array list
char  objectNames[][OBJECT_NAME_SIZE] = { {"spitze"} ,{"vtSpitzeR"} , {"vtSpitzeL"} ,  {"gaucer"} , {"lovac"} , {"zorgen"} , {"litec"} , {"nacreeb"}
					 , { "enemyMissile"} , {"enemyHommingMissile"} ,{"farbenL"} , {"farbenR"}, {"petrus"} , { "warningMsg" } , {"bossWarningMsg"} , {"stage1Boss"} 
                     , {"haltSpawn"} , {"jumpaLD"} , {"jumpaRD"} , {"jumpaLU"} , {"jumpaRU"} , {"stomLD"} , {"stomRD"} , {"stomLU"} , {"stomRU"} , {"laroc"}
                     , {"anancitesST"} , {"midBoss"} ,{"godrill"} , { "starView"} , { "laserRing" } , { "tirtasL"} , {"tirtasR"} , {"tekkerz"} 
                     , {"R_archer"} , {"L_archer"} , { "maraac"}  , {"nakser"} , {"bKnightL"} , {"bKnightR"} , {"leftBeamShoot" } , {"stage2Boss"} , { "\0" } };
void *objectSpawnFuncs[] = { &spawnSpitze , &spawnVtSpitzeR , &spawnVtSpitzeL ,  &spawnGaucer , &spawnLovac , &spawnZorgen , &spawnLitec , &spawnNacreeb
			    , &spawnEnemyMissile , &spawnEnemyHMissile , &spawnFarbenL , &spawnFarbenR , &spawnPetrus , &spawnWarningMsg, &spawnBossWarningMsg , &spawnStage1Boss 
                , &spawnHaltSpawnEvent , &spawnJumpaLD , &spawnJumpaRD , &spawnJumpaLU , &spawnJumpaRU , &spawnStomLD , &spawnStomRD , &spawnStomLU
                , &spawnStomRU , &spawnLaroc , &spawnAnancitesST , &spawnMidBossEntrance , &spawnGodrill , &spawnStarView , &spawnLaserRing 
                , &spawnTirtasL , &spawnTirtasR , &spawnTekkerz , &spawnR_archer , &spawnL_archer , &spawnMaraac , &spawnNakser  
                , &spawnBKnight_L , &spawnBKnight_R , &spawnLeftBeamShoot , &spawnStage2Boss };

/********************************************
    getObjectSpawnFunc
    Returns the object spawn function
*********************************************/

void *getObjectSpawnFunc( char *objectName)
{
	register i = 0;
	while( *(*objectNames+(i*OBJECT_NAME_SIZE)) != '\0')
	{
		if( strcmp( (char *)(*objectNames+(i*OBJECT_NAME_SIZE)) , objectName) == 0 )
		{
			return *(objectSpawnFuncs+i);
		}
		i++;
	}
	return NULL;
}
