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
   enemies.c
*********************************************/

#ifndef ENEMY_BULLET_C
#define ENEMY_BULLET_C
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <math.h>
//
#include "d_programTypes.h"
#include "globalDefines.h"
#include "d_GameObject.h"
#include "objects.h"
#include "d_Charset.h"
#include "d_enemyBullet.h"
#include "enemyBullet.h"
#include "enemies.h"
#include "bullets.h"
#include "globalVars.h"
#include "game.h"

/********************************************
    stdEnemyBullet:
*********************************************/

void stdEnemyBullet( GameObject *object)
{

    stdEnemyBulletAttributes *objAttributes = (stdEnemyBulletAttributes *) object->attributes;
    //
	if( object->collision != 0)
	{
		freeANDpushObj( &OAM_enemiesBullets , object);
		return;
	}
    //
	if(   object->pos.x > GM_SCREEN_W + object->pos.w
	   || object->pos.x < -object->pos.w
	   || object->pos.y > GM_SCREEN_H + object->pos.h
	   || object->pos.y < -object->pos.h)
	{
		freeANDpushObj( &OAM_enemiesBullets , object);
		return;
	}
    //
    objAttributes->posX +=  objAttributes->xdir;
    objAttributes->posY +=  objAttributes->ydir;
    
	object->pos.x = objAttributes->posX;
	object->pos.y = objAttributes->posY;


	return;
}

//-------------------------------------------

/********************************************
    new_smallEnemyBullet:
*********************************************/

void newStdEnemyBullet( GameObject  *object , double xdir , double ydir)
{
	object->func = &stdEnemyBullet;

	object->attributes = malloc( sizeof(stdEnemyBulletAttributes) );

    stdEnemyBulletAttributes *objAttributes = (stdEnemyBulletAttributes *) object->attributes;

	objAttributes->xdir = xdir;
	objAttributes->ydir = ydir;
    objAttributes->posX = object->pos.x;
    objAttributes->posY = object->pos.y;

	return;

}

//-------------------------------------------

/********************************************
    makeSmallEnemyBullet:
*********************************************/

void makeSmallEnemyBullet( SDL_Rect pos , double xdir , double ydir)
{
    GameObject *newObjPtr;
	GmObjectAnim bulletAnim = {  SMALL_ENEMY_BULLET_BASE_FRAME 
                               , SMALL_ENEMY_BULLET_ANIM_FRAMES 
                               , 0 , 6 , 0 , ANIMATION_REPEAT };

    if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
    {
            pos.w = 8;
            pos.h = 8;

        	SDL_Rect colBox = { 1 , 1 , 6 , 6};

			newObjPtr = newObjANDpull( &OAM_enemiesBullets , &pos , &colBox
						  , BULLETS_8X8_CHARSET , 0 , 0 , 1 , &bulletAnim);

		    newStdEnemyBullet( newObjPtr , xdir , ydir );
    }
    return;
}

//-------------------------------------------

/********************************************
    makeGatlingBullet:
*********************************************/

void makeGatlingBullet( SDL_Rect *pos , double xdir , double ydir)
{
    GameObject *newObjPtr;
	GmObjectAnim bulletAnim = {  GATLING_BULLET_BASE_FRAME
                               , GATLING_BULLET_ANIM_FRAMES 
                               , 0 
                               , GATLING_BULLET_FRAME_DELAY
                               , 0 , ANIMATION_REPEAT };

    if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
    {
            pos->w = 16;
            pos->h = 16;

        	SDL_Rect colBox = { 6 , 6 , 5 , 5};

			newObjPtr = newObjANDpull( &OAM_enemiesBullets , pos , &colBox
						  , BULLETS_16X16_CHARSET , 0 , 0 , 1 , &bulletAnim);

		    newStdEnemyBullet( newObjPtr , xdir , ydir );
    }
    return;
}

//-------------------------------------------

void spawnGatlingBulletsBurst(SDL_Rect pos)
{
    enum { burstBullets = 12};
    static vector bulletsDir[burstBullets] =
    {
         { 0.00,-1.00},{ 0.50,-0.86},{ 0.86,-0.50},{ 1.00, 0.00}
        ,{ 0.86, 0.50},{ 0.50, 0.86},{ 0.00, 1.00}
        ,{-0.50, 0.86},{-0.86, 0.50},{-1.00, 0.00}
        ,{-0.86,-0.50},{-0.50,-0.86}

    };

    int i;
    for( i = 0 ; i < burstBullets ; i++)
    {
        makeGatlingBullet(  &pos
                          , bulletsDir[i].x
                          , bulletsDir[i].y);
    }
}

//-------------------------------------------

/********************************************
    enemyBlastBullet
/********************************************/

void enemyBlastBullet( GameObject *object)
{
    enemyBlastBulletAttributes *objAttributes = (enemyBlastBulletAttributes *) object->attributes;

	//Delete if it is outside the screen
    if(   object->pos.x > GM_SCREEN_W
	   || object->pos.x < -object->pos.w
	   || object->pos.y > GM_SCREEN_H
	   || object->pos.y < -object->pos.h)
	{
        if( objAttributes->listEntry != NULL)
        {
            *objAttributes->listEntry = NULL;
        }

		freeANDpushObj( &OAM_enemiesBullets , object);
		return;
	}

    //Update position
    objAttributes->fx += objAttributes->xdir * objAttributes->velocity;
    objAttributes->fy += objAttributes->ydir * objAttributes->velocity; 

    if(objAttributes->velocity > BLAST_BULLET_MIN_VELOCITY)
    {
       objAttributes->velocity -= BLAST_BULLET_VEL_DEC_RATE;
    }

    object->pos.x = objAttributes->fx;
    object->pos.y = objAttributes->fy;
    
    return;
}

//-------------------------------------------

void newEnemyBlastBullet( GameObject *object , float xdir , float ydir , int size)
{
    object->func = &enemyBlastBullet;
    /**/
	object->attributes = malloc( sizeof(enemyBlastBulletAttributes) );
    /**/
    enemyBlastBulletAttributes *objAttributes = (enemyBlastBulletAttributes *) object->attributes;
    /**/
    objAttributes->status = 0;
    objAttributes->size = size;
    objAttributes->listEntry = NULL;
    /**/
	objAttributes->xdir = xdir;
	objAttributes->ydir = ydir;
    objAttributes->fx = object->pos.x;
    objAttributes->fy = object->pos.y;
    objAttributes->velocity = BLAST_BULLET_INITAL_VEL;

    return;
};

//-------------------------------------------

SDL_Rect blastBulletsRectSize[ ENEMYBLASTBULLETS_NUM ] =
{
      { 0 , 0 , 8 , 8}   //SMALL
    , { 0 , 0 , 16 , 16} //MEDIUM
    , { 0 , 0 , 32 , 32} //BIG
};

//-------------------------------------------

int blastBulletBaseFrame[ ENEMYBLASTBULLETS_NUM ] =
{
     0 //SMALL
    ,8 //MEDIUM
    ,0 //BIG
};

//-------------------------------------------

int blastBulletCharset[ ENEMYBLASTBULLETS_NUM ] =
{
     BULLETS_8X8_CHARSET //SMALL
    ,BULLETS_16X16_CHARSET
    ,BULLETS_8X8_CHARSET
};

//-------------------------------------------

void makeEnemyBlastBullet( SDL_Rect *pos , float xdir , float ydir , int size)
{
    GameObject *newObjPtr;
	//GmObjectAnim bulletAnim = { _BASE_FRAME , _ANIM_FRAMES , 0 , 5 , 0 , ANIMATION_REPEAT };
    if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
    {
            pos->w = blastBulletsRectSize[size].w;
            pos->h = blastBulletsRectSize[size].h;

        	SDL_Rect colBox = blastBulletsRectSize[size];

			newObjPtr = newObjANDpull( &OAM_enemiesBullets , pos , &colBox
						  , blastBulletCharset[size] , blastBulletBaseFrame[size] , 0 , 1 , NULL);

		    newEnemyBlastBullet( newObjPtr , xdir , ydir , size);
    }
    return;
};

//-------------------------------------------

void makeSmallEnemyBlastBullet( SDL_Rect *pos , float xdir , float ydir)
{
    makeEnemyBlastBullet( pos , xdir , ydir , ENEMYBLASTBULLET_SMALL );
    return;
}

//-------------------------------------------

void makeMediumEnemyBlastBullet( SDL_Rect *pos , float xdir , float ydir)
{
    makeEnemyBlastBullet( pos , xdir , ydir , ENEMYBLASTBULLET_MEDIUM );
    return;
}

/********************************************
    energyShootChunk:
*********************************************/


void energyShootChunk( GameObject *object)
{
    /*Delete when the object animation ends*/
	if( object->anim.counter == 0
	    && object->anim.timer == 0)
    {
		freeANDpushObj(&OAM_enemiesBullets , object);
    }
    return ;
}

//-------------------------------------------

void newEnergyShootChunk( GameObject *object)
{
    object->func = &energyShootChunk;
    return;
}

//-------------------------------------------

void spawnEnergyShootChunk( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
	{
		GmObjectAnim objAnim={  ENERGY_SHOOT_CHUNK_BASE_FRAME
                               ,ENERGY_SHOOT_CHUNK_ANIM_FRAMES
                               ,0
                               ,ENERGY_SHOOT_CHUNK_ANIM_DELAY 
                               ,0,0 };
		pos.w = ENERGY_SHOOT_CHUNK_W;
		pos.h = ENERGY_SHOOT_CHUNK_H;
		SDL_Rect colBox = { 3 , 0 , 10, 16}; 
		GameObject *newObj = newObjANDpull( &OAM_enemiesBullets , &pos , &colBox
						 , BULLETS_16X16_CHARSET , 0 , 0 , 1 , &objAnim);
		newEnergyShootChunk(newObj);
	}
    return;
}

//-------------------------------------------

void spawnEnergyShootTop( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
	{
		GmObjectAnim objAnim={  ENERGY_SHOOT_TOP_BASE_FRAME
                               ,ENERGY_SHOOT_TOP_ANIM_FRAMES
                               ,0
                               ,ENERGY_SHOOT_TOP_ANIM_DELAY 
                               ,0,0 };
		pos.w = ENERGY_SHOOT_CHUNK_W;
		pos.h = ENERGY_SHOOT_CHUNK_H;
		SDL_Rect colBox = { 3 , 0 , 10, 16}; 
		GameObject *newObj = newObjANDpull( &OAM_enemiesBullets , &pos , &colBox
						 , BULLETS_16X16_CHARSET , 0 , 0 , 1 , &objAnim);
		newEnergyShootChunk(newObj);
	}
    return;
}

//-------------------------------------------

void spawnEnergyShoot( SDL_Rect pos)
{
    SDL_Rect chunkPos = pos;
    spawnEnergyShootTop(chunkPos);
    chunkPos.y += ENERGY_SHOOT_CHUNK_H;
    for( ; chunkPos.y < GM_SCREEN_H ; chunkPos.y += ENERGY_SHOOT_CHUNK_H)
    {
        spawnEnergyShootChunk(chunkPos);
    }
    return;
}

/********************************************
    leftBeamShoot
*********************************************/


void BeamChunk( GameObject *object)
{
    /*Delete when the object animation ends*/
	if( object->anim.counter == 0
	    && object->anim.timer == 0)
    {
		freeANDpushObj(&OAM_enemiesBullets , object);
    }
    return ;
}

//-------------------------------------------

void newBeamChunk( GameObject *object)
{
    object->func = &BeamChunk;
    return;
}

//-------------------------------------------

void spawnLeftBeamHead( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
	{
		GmObjectAnim objAnim={  L_BEAM_HEAD_BASE_FRAME
                               ,L_BEAM_HEAD_ANIM_FRAMES
                               ,0
                               ,L_BEAM_HEAD_ANIM_DELAY 
                               ,0,0 };
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 0 , 3 , 16, 10}; 
		GameObject *newObj = newObjANDpull( &OAM_enemiesBullets , &pos , &colBox
						 , BULLETS_16X16_CHARSET , 0 , 0 , 1 , &objAnim);
		newBeamChunk(newObj);
	}
    return;
}

//-------------------------------------------

void spawnRightBeamHead( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
	{
		GmObjectAnim objAnim={  R_BEAM_HEAD_BASE_FRAME
                               ,R_BEAM_HEAD_ANIM_FRAMES
                               ,0
                               ,R_BEAM_HEAD_ANIM_DELAY 
                               ,0,0 };
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 0 , 3 , 16, 10}; 
		GameObject *newObj = newObjANDpull( &OAM_enemiesBullets , &pos , &colBox
						 , BULLETS_16X16_CHARSET , 0 , 0 , 1 , &objAnim);
		newBeamChunk(newObj);
	}
    return;
}

//-------------------------------------------

void spawnBeamChunk( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_enemiesBullets) >= 0)
	{
		GmObjectAnim objAnim={  BEAM_SHOOT_BASE_FRAME
                               ,BEAM_SHOOT_ANIM_FRAMES
                               ,0
                               ,BEAM_SHOOT_ANIM_DELAY 
                               ,0,0 };
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 0 , 3 , 16, 10}; 
		GameObject *newObj = newObjANDpull( &OAM_enemiesBullets , &pos , &colBox
						 , BULLETS_16X16_CHARSET , 0 , 0 , 1 , &objAnim);
		newBeamChunk(newObj);
	}
    return;
}

//-------------------------------------------

void spawnLeftBeamShoot( SDL_Rect pos)
{
    SDL_Rect chunkPos = pos;
    spawnLeftBeamHead( chunkPos);
    chunkPos.x += BEAM_SHOOT_CHUNK_W;
    for( ; chunkPos.x < GM_SCREEN_W ; chunkPos.x += BEAM_SHOOT_CHUNK_W)
    {
        spawnBeamChunk(chunkPos);
    }
    return;    
}

//------------------------------------------

void spawnRightBeamShoot( SDL_Rect pos)
{
    SDL_Rect chunkPos = pos;
    spawnRightBeamHead( chunkPos);
    chunkPos.x -= BEAM_SHOOT_CHUNK_W;
    for( ; chunkPos.x > -BEAM_SHOOT_CHUNK_W ; chunkPos.x -= BEAM_SHOOT_CHUNK_W)
    {
        spawnBeamChunk(chunkPos);
    }
    return;    
}


