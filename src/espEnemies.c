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
    espEnemies.c
********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_programTypes.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_trajectory.h"
#include "d_enemies.h"
#include "globalVars.h"
#include "d_bmFont.h"
//
#include "OAM.h"
#include "cMath.h"
#include "math.h"
#include "objects.h"
#include "l_trajectories.h"
#include "enemyBullet.h"
#include "enemies.h"
#include "effects.h"
#include "bonus.h"
#include "debug.h"
#include "time.h"
#include "announcer.h"
#include "game.h"
#include "score.h"
#include "spawn.h"
#include "trajectory.h"
/*------------------------------------*/

extern void makeEnemyHMissile(SDL_Rect pos , int pitch);

#define BOSS1_ROUND_SIZE 24
#define BOSS1_BULLET_VEL 2
#define BOSS1_SHOOT_DELAY 2
#define BOSS1_ROUND_DELAY 100

static vector boss1Round[BOSS1_ROUND_SIZE] =
{
     {   0.000000 ,  1.000000 }
    //
    ,{  -0.500000 ,  0.866025 }
    ,{  -0.866025 ,  0.500000 }
    //
    ,{  -1.000000 ,  0.000000 }
    //
    ,{  -0.866025 , -0.500000 }
    ,{  -0.500000 , -0.866025 }
    //
    ,{  -0.000000 , -1.000000 }
    //
    ,{   0.500000 , -0.866025 }
    ,{   0.866025 , -0.500000 }
    //
    ,{   1.000000 ,  0.000000 }
    //
    ,{   0.866025 ,  0.500000 }
    ,{   0.500000 ,  0.866025 }

    //Second round
    ,{   0.000000 ,  1.000000 }
    //
    ,{   0.500000 ,  0.866025 }
    ,{   0.866025 ,  0.500000 }
    //
    ,{   1.000000 ,  0.000000 }
    //
    ,{   0.866025 , -0.500000 }
    ,{   0.500000 , -0.866025 }
    //
    ,{  -0.000000 , -1.000000 }
    //
    ,{  -0.500000 , -0.866025 }
    ,{  -0.866025 , -0.500000 }
    //
    ,{  -1.000000 ,  0.000000 }
    //
    ,{  -0.866025 ,  0.500000 }
    ,{  -0.500000 ,  0.866025 }
};

#define BOSS1_ATTRIBUTE(x)\
	((boss1attributes *)object->attributes)->x

void boss1( GameObject *object)
{

    static const SDL_Rect leftBurstPadding = { 1 , 35 , 0 ,0 };
    static const SDL_Rect rightBurstPadding = { 102 , 35 , 0 ,0 };

    SDL_Rect bulletSpawnPos1;
    SDL_Rect bulletSpawnPos2;

    boss1attributes *objAttributes = ( boss1attributes *)object->attributes;

    //Manage collision damage
	if( object->collision > BOSS1_LIFE)
	{
		freeANDpushObj( &OAM_enemies , object);
        freePropulsionBurst(objAttributes->leftBurst);
        freePropulsionBurst(objAttributes->rightBurst);
        incScore(BOSS1_BONUS_POINTS);
		//resumeSpawn();
        //endStage();
        spawnStageBossExplosion(object->pos);
		return;
	}

    //Move to the shoot position
	if( object->pos.y < BOSS1_MIN_Y)
	{
		objAttributes->posY++;
		object->pos.y = objAttributes->posY;
		return;
	}
    
    //Vertical oscillation
    objAttributes->posY += objAttributes->stepY;
    objAttributes->stepYcounter--;
    if( objAttributes->stepYcounter == 0)
    {
        objAttributes->stepY *= -1;
        objAttributes->stepYcounter = BOSS1_STEPS_Y_NUM;
    }

	object->pos.y = objAttributes->posY;

	//shoot bullets
    if(objAttributes->roundDelay)
    {
        objAttributes->roundDelay--;
        //X movement
	    objAttributes->posX += objAttributes->dirX;
	    if(   objAttributes->posX <= 0
	        || objAttributes->posX >= GM_SCREEN_W - object->pos.w)
        {
	    	objAttributes->dirX *= -1.0;
        }
	    object->pos.x =objAttributes->posX;

    }
    else
    {
        if(objAttributes->shootDelay)
        {
            objAttributes->shootDelay--;
        }
        else
        {
            bulletSpawnPos1.x = object->pos.x + 33 ; 
            bulletSpawnPos1.y = object->pos.y + 110 ;
            bulletSpawnPos2.x = object->pos.x + 89 ;
            bulletSpawnPos2.y = object->pos.y + 110 ;
            //Shoot
            makeSmallEnemyBullet(   bulletSpawnPos1
                                  , boss1Round[objAttributes->cRoundBullet].x * BOSS1_BULLET_VEL
                                  , boss1Round[objAttributes->cRoundBullet].y * BOSS1_BULLET_VEL);
            //
             makeSmallEnemyBullet(  bulletSpawnPos2
                                  , boss1Round[objAttributes->cRoundBullet].x * BOSS1_BULLET_VEL * -1
                                  , boss1Round[objAttributes->cRoundBullet].y * BOSS1_BULLET_VEL);            
            //
            objAttributes->shootDelay = BOSS1_SHOOT_DELAY;
            //
            objAttributes->cRoundBullet++;
            if( objAttributes->cRoundBullet >= BOSS1_ROUND_SIZE)
            {
                objAttributes->cRoundBullet = 0;
                objAttributes->roundDelay = BOSS1_ROUND_DELAY;
            }
        }
    }

    //Update Propulsion Burst effects position
    objAttributes->leftBurst->pos.x = object->pos.x
                                      + leftBurstPadding.x;
   
    objAttributes->leftBurst->pos.y = object->pos.y
                                      + leftBurstPadding.y;

    objAttributes->rightBurst->pos.x = object->pos.x
                                      + rightBurstPadding.x;
   
    objAttributes->rightBurst->pos.y = object->pos.y
                                      + rightBurstPadding.y;

    

	return;
}

void newBoss1( GameObject *object)
{
	object->func = &boss1;
	object->attributes = malloc(sizeof(boss1attributes));
    boss1attributes *objAttributes = object->attributes ;
	objAttributes->dirX = BOSS1_X_VEL;
	objAttributes->posX = object->pos.x;
	objAttributes->posY = object->pos.y;
    objAttributes->stepY = BOSS1_STEP_Y;
    objAttributes->stepYcounter = BOSS1_STEPS_Y_NUM;
    int stepYcounter;
	objAttributes->cRoundBullet = 0;
	objAttributes->roundDelay = BOSS1_ROUND_DELAY;
	objAttributes->shootDelay = BOSS1_SHOOT_DELAY;
    //
    objAttributes->leftBurst = newBoss1LeftBurst(object->pos);
    objAttributes->rightBurst = newBoss1RightBurst(object->pos);
}


/********************************************
    boss1Entrance
    Wait until there are no enemies alive
    to spawn the stage boss
*********************************************/

void boss1Entrance(GameObject  *object)
{
	if( activeEnemies() == 0)
	{
		GameObject *newObjPtr;
		GmObjectAnim enemyAnim = { 0 , 3 , 0 , 4 , 0 , ANIMATION_REPEAT};

		SDL_Rect pos;
		pos.w = 128;
		pos.h = 128;
		pos.x = GM_SCREEN_W/2-pos.w/2;
		pos.y = -pos.h;

		if( searchEmptyObject( &OAM_enemies) >= 0)
		{
			SDL_Rect colBox = { 49 , 62 , 29 , 29};
			newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  ,  ENEMIES_128X128_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
			newBoss1( newObjPtr);
		}
		freeANDpushObj(&OAM_events , object);
	}
	return;
}

/********************************************/

void spawnStage1Boss(SDL_Rect pos)
{
	pauseSpawn();
    //
	if( searchEmptyObject( &OAM_events) >= 0)
	{
		GameObject *newObj = newObjANDpull( &OAM_events , &pos , &pos
						 , 0 , 0 , 0 , 0 , NULL);
		newObj->func = &boss1Entrance;
		newObj->attributes = NULL;
	}
	return;
}

//-------------------------------------------

/********************************************
    boss2
*********************************************/

//Boss 2 states
enum 
{
     BOSS2_ENTRANCE = 0
    ,BOSS2_STANDING
};


void boss2( GameObject *object)
{
    enum 
    {
         ROCKET_SHOOT_DELAY = 200
        ,START_Y = 40
    };

    boss2attributes *objAttributes = (boss2attributes *)object->attributes;

/**/
switch(objAttributes->state)
{
/**/

case BOSS2_ENTRANCE:

    if( object->pos.y < START_Y)
    {
         object->pos.y++;
         break;
    }

    objAttributes->state = BOSS2_STANDING;
    /*Initialize trajectory*/
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , boss2_trajectory );    
    /*Clean collision damage*/
    object->collision = 0;

case BOSS2_STANDING:

    //Handle collision damage
    
	if( object->collision > BOSS2_LIFE)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(BOSS2_BONUS_POINTS);
        spawnStageBossExplosion(object->pos);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

    objAttributes->rocketDelay++;
    if( objAttributes->rocketDelay >= ROCKET_SHOOT_DELAY)
    {
        objAttributes->rocketDelay = 0;
        SDL_Rect missilePos;
        missilePos.x = object->pos.x + 10;
        missilePos.y = object->pos.y + object->pos.h / 2 - 30;
        makeEnemyHMissile(missilePos , 5);
        makeEnemyHMissile(missilePos , 6);
        makeEnemyHMissile(missilePos , 7);
        makeEnemyHMissile(missilePos , 8);
        makeEnemyHMissile(missilePos , 9);
        makeEnemyHMissile(missilePos , 10);
        makeEnemyHMissile(missilePos , 11);


        missilePos.x = object->pos.x + 90;
        missilePos.y = object->pos.y + object->pos.h / 2 - 30;

        makeEnemyHMissile(missilePos , 13);
        makeEnemyHMissile(missilePos , 14);
        makeEnemyHMissile(missilePos , 0);
        makeEnemyHMissile(missilePos , 1);
        makeEnemyHMissile(missilePos , 2);
        makeEnemyHMissile(missilePos , 3);
    }


    //DEV
    objAttributes->gatlingDelay++;
    SDL_Rect bulletPos = object->pos;
    if( objAttributes->gatlingDelay == 80)
    {
        bulletPos.x = object->pos.x+28;
        bulletPos.y = object->pos.y+66;
        spawnGatlingBulletsBurst(bulletPos);
    }
    else if( objAttributes->gatlingDelay >= 160)
    {
        objAttributes->gatlingDelay = 0;
        bulletPos.x = object->pos.x+28+58;
        bulletPos.y = object->pos.y+66;
        spawnGatlingBulletsBurst(bulletPos);
    }
    //END DEV

    /*
	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}
    */
}/*end switch*/
    return;
}

/********************************************/

void newBoss2( GameObject *object)
{
	object->func = &boss2;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(boss2attributes) );
    //
    boss2attributes *objAttributes = (boss2attributes *)object->attributes;
    //
    objAttributes->rocketDelay = 0;
    objAttributes->state = BOSS2_ENTRANCE;
    objAttributes->gatlingDelay= 0;
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , boss2_trajectory );
}

/********************************************
    boss2Entrance
    Wait until there are no enemies alive
    to spawn the stage boss
*********************************************/

void boss2Entrance(GameObject  *object)
{
	if( activeEnemies() == 0)
	{
		GameObject *newObjPtr;
		GmObjectAnim enemyAnim = { 
                                     BOSS2_BASE_FRAME
                                   , BOSS2_ANIM_FRAMES 
                                   , 0
                                   , BOSS2_FRAME_DELAY 
                                   , 0 
                                   , ANIMATION_REPEAT
                                 };

		SDL_Rect pos;
		pos.w = 128;
		pos.h = 128;
		pos.x = GM_SCREEN_W/2-pos.w/2;
		pos.y = -pos.h;

		if( searchEmptyObject( &OAM_enemies) >= 0)
		{
			SDL_Rect colBox = { 9 , 25 , 109 , 29};
			newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  ,  ENEMIES_128X128_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
			newBoss2( newObjPtr);
		}
		freeANDpushObj(&OAM_events , object);
	}
	return;
}

/********************************************/

void spawnStage2Boss(SDL_Rect pos)
{
	pauseSpawn();
    //
	if( searchEmptyObject( &OAM_events) >= 0)
	{
		GameObject *newObj = newObjANDpull( &OAM_events , &pos , &pos
						 , 0 , 0 , 0 , 0 , NULL);
		newObj->func = &boss2Entrance;
		newObj->attributes = NULL;
	}
	return;
}
