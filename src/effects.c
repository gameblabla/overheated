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
 effects.c:
 Special effects
*********************************************/

#ifndef EFFECTS_C
#define EFFECTS_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_effects.h"
#include "d_bmFont.h"
#include "l_sfx.h"
#include "globalVars.h"

//
#include "effects.h"
#include "OAM.h"
#include "objects.h"
#include "debug.h"
#include "sfx.h"
#include "announcer.h"
#include "spawn.h"
#include "game.h"
#include "player.h"

#define SMALL_EXPLOSION_START_TILE
#define SMALL_EXPLOSION_END_TILE

/*******************************************
    freeEffect:
     Free effect slot
*******************************************/

void freeEffect( GameObject *object)
{
    freeANDpushObj(&OAM_effects , object);
    object = NULL;
    return;
}
/*******************************************
    stdExplosion
    Standard Explosion
*******************************************/

void stdExplosion( GameObject *object)
{
	if(objAnimEnded(object))
    {
		freeANDpushObj(&OAM_effects , object);
    }
    
    return;
}

//-------------------------------------------

void newStdExplosion( GameObject *object)
{
	object->func = &stdExplosion;
	return;
}

//-------------------------------------------

/*******************************************
    newStdEnemyExplosion:
    Standard explosion with SFX_EXPLOSION_1 Sound
*******************************************/

void newStdEnemyExplosion( GameObject *object)
{
	playEffect(SFX_EXPLOSION_1);
    newStdExplosion( object);
	return;
}


//-------------------------------------------

void spawnStdBulletExplosion( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim={ STD_BULLET_EXPLOSION_BASE_FRAME
                                    ,STD_BULLET_EXPLOSION_ANIM_FRAMES
                                    ,0
                                    ,STD_BULLET_EXPLOSION_ANIM_DELAY
                                    ,0,0 };
		pos.w = 8;
		pos.h = 8;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_8X8_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newStdExplosion(newExplosion);
	}
	return;
}

//-------------------------------------------

/*******************************************
    spawnImpulseBurst
*******************************************/


void  spawnImpulseBurst( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
/*
		GmObjectAnim explosionAnim={ IMPULSE_BURST_BASE_FRAME
                                    ,IMPULSE_BURST_ANIM_FRAMES
                                    ,0
                                    ,IMPULSE_BURST_ANIM_DELAY
                                    ,0,0 };
*/

        GmObjectAnim anim = { 
                                SHIP_BURST_BASE_FRAME
                              , SHIP_BURST_ANIM_FRAMES
                              , 0 
                              , SHIP_BURST_ANIM_DELAY
                              , 0 , 0 
                            };
		pos.w = 16;
		pos.h = 16;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_32X32_CHARSET , 0 , 0 , 0 , &anim);
		newStdExplosion(newExplosion);
	}
	return;
}

//-------------------------------------------

/*******************************************
    missileTrail
*******************************************/

void spawnMissileTrail( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim={ MISSILE_TRAIL_BASE_FRAME
                                    , MISSILE_TRAIL_ANIM_FRAMES
                                    ,0
                                    , MISSILE_TRAIL_ANIM_DELAY
                                    ,0,0 };
		pos.w = 8;
		pos.h = 8;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_8X8_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newStdExplosion(newExplosion);
	}
	return;
}



//-------------------------------------------

/*******************************************
    spawnSmallExplosion_1
*******************************************/

void spawnSmallExplosion_1( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim={0,5,3,5,0,0};
		pos.w = 16;
		pos.h = 16;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_16X16_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newStdEnemyExplosion(newExplosion);
	}
	return;
}


//-------------------------------------------

/*******************************************
    spawnMediumExplosion
*******************************************/

void spawnMediumExplosion_1( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim=
                                    {      
                                         MEDIUM_EXPLOSION_BASE_FRAME
                                        ,MEDIUM_EXPLOSION_ANIM_FRAMES
                                        ,0
                                        ,MEDIUM_EXPLOSION_ANIM_DELAY 
                                        ,0,0
                                    };
		pos.w = 32;
		pos.h = 32;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_32X32_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newStdEnemyExplosion(newExplosion);
	}
	return;
}

//-------------------------------------------

/*******************************************
    spawnMediumExplosion 2 ( Ring explosion
*******************************************/

void spawnMediumExplosion_2( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim=
                                    {      
                                         MEDIUM_EXPLOSION_2_BASE_FRAME
                                        ,MEDIUM_EXPLOSION_2_ANIM_FRAMES
                                        ,0
                                        ,MEDIUM_EXPLOSION_2_ANIM_DELAY
                                        ,0,0
                                    };
		pos.w = 32;
		pos.h = 32;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_32X32_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newStdEnemyExplosion(newExplosion);
        playEffect(SFX_PLAYER_EXPLOSION);
	}
	return;
}

//-------------------------------------------


/*******************************************
    smallTileExplosion:

    Tile object explosion effect
*******************************************/

void smallTileExplosion( GameObject *object)
{
    smallTileExplosionAttributes *objAttributes = (smallTileExplosionAttributes *)object->attributes;

    object->pos.x = objAttributes->tilePos->x;
    object->pos.y = objAttributes->tilePos->y;

    //If the animation ends , free the object
	if( object->anim.counter == 0
	    && object->anim.timer == 0)
		freeANDpushObj(&OAM_effects , object);

    return;
}

//-------------------------------------------

void newSmallTileExplosion( GameObject *object , SDL_Rect *tilePos)
{
	playEffect(SFX_EXPLOSION_1);
    //
	object->func = &smallTileExplosion;
    //
    object->attributes = malloc(sizeof(smallTileExplosionAttributes) );    
    smallTileExplosionAttributes *objAttributes = (smallTileExplosionAttributes *)object->attributes;
    //
    objAttributes->tilePos = tilePos; //Initialize pointer to the tile position struct

	return;
}

//-------------------------------------------

void makeSmallTileExplosion( SDL_Rect *pos)
{

	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim={ SMALL_TILE_EXPLOSION_BASE_FRAME
                                    ,SMALL_TILE_EXPLOSION_ANIM_FRAMES
                                    ,0
                                    ,SMALL_TILE_EXPLOSION_ANIM_DELAY
                                    ,0,0};
		pos->w = 16;
		pos->h = 16;
        SDL_Rect colBox = { 0 , 0 , pos->w , pos->h};
	    GameObject *newExplosion = newObjANDpull(  &OAM_effects , pos , &colBox
						             , EFFECTS_16X16_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newSmallTileExplosion(newExplosion , pos);
	}

	return ;
}

//-------------------------------------------

/*******************************************
    haltSpawnEvent
*******************************************/

void haltSpawnEvent( GameObject *object)
{
	if( activeEnemies() == 0)
	{
		resumeSpawn();
		freeANDpushObj(&OAM_events , object);
	}
	return;
}

void newHaltSpawnEvent( GameObject *object)
{
	object->func = &haltSpawnEvent;
	pauseSpawn();

	return;
}

void spawnHaltSpawnEvent( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_events) >= 0)
	{
		GameObject *newObj = newObjANDpull( &OAM_events , &pos , &pos
						 , 0 , 0 , 0 , 0 , NULL);
		newHaltSpawnEvent( newObj);
	}
}

/********************************************/

void spawnWarningMsg( SDL_Rect pos)
{
        setWarningMsg();
		return;
}

/********************************************/

void spawnBossWarningMsg( SDL_Rect pos)
{
        setBossWarningMsg();
		return;
}

//-------------------------------------------

/********************************************
    shipBurstWave:
    Explosion wave that causes damage to the enemy
*********************************************/
void shipBurstWave( GameObject *object)
{
    shipBurstWaveAttributes *objAttributes = (shipBurstWaveAttributes *)object->attributes;

    if(objAttributes->lifeTimeCounter)
    {
        objAttributes->lifeTimeCounter--;
    }
    else
    {
        freeANDpushObj(&OAM_PlBullets , object);
        return;
    }
    SDL_Rect playerPos = getPlayerPosition();
    object->pos.x = playerPos.x + PL_BURST_X_PADDING;
    object->pos.y = playerPos.y + PL_BURST_Y_PADDING;    

    return;
}

//-------------------------------------------

void newShipBurstWave( GameObject *object)
{
    object->func = &shipBurstWave;
    SDL_Rect playerPos = getPlayerPosition();
    object->pos.x = playerPos.x + PL_BURST_X_PADDING;
    object->pos.y = playerPos.y + PL_BURST_Y_PADDING;
    /**/
    object->attributes = malloc( sizeof(shipBurstWaveAttributes) );
    shipBurstWaveAttributes *objAttributes = (shipBurstWaveAttributes *)object->attributes;
    objAttributes->lifeTimeCounter = SHIP_BURST_WAVE_LIFE_TIME;

    return;
}

//-------------------------------------------

void spawnShipBurstWave(SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_PlBullets) >= 0)
    {
        GameObject *object;

        SDL_Rect colBox = { 3 , 3 , 25 , 25 };
        GmObjectAnim objAnim = { 0 , 0 , 0 , 0 , 0 , 0 };
       
        object = newObjANDpull( &OAM_PlBullets , &pos , &colBox
						 , EMPTY_TILESET , 0 , 0 , 1 , &objAnim);

        newShipBurstWave(object);
    }
    return;
}

/********************************************
    shipBurst:
*********************************************/
#define SHIP_BURST_END_TILE 
void shipBurst( GameObject *object)
{
    shipBurstAttributes *objAttributes = (shipBurstAttributes *)object->attributes;

    if( object->anim.counter == 0
        && object->anim.timer == 0)
    {
        freeANDpushObj(&OAM_effects , object);        
        return;
    }

    SDL_Rect playerPos = getPlayerPosition();

    object->pos.x = playerPos.x + PL_BURST_X_PADDING;
    object->pos.y = playerPos.y + PL_BURST_Y_PADDING;

    return;
}



//-------------------------------------------

/********************************************
    newShipBurst:
*********************************************/
void newShipBurst( GameObject *object)
{
    object->func = &shipBurst;
    object->attributes = NULL;
    SDL_Rect playerPos = getPlayerPosition();
    object->pos.x = playerPos.x + PL_BURST_X_PADDING;
    object->pos.y = playerPos.y + PL_BURST_Y_PADDING;
    /*
    object->attributes = malloc( sizeof(shipBurstAttributes) );
    shipBurstAttributes *objAttributes = (shipBurstAttributes *)object->attributes;
    */
    return ;
}

//-------------------------------------------

/********************************************
    spawnShipBurst
*********************************************/

void spawnShipBurst( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{

        GmObjectAnim anim = { 
                                SHIP_BURST_BASE_FRAME
                              , SHIP_BURST_ANIM_FRAMES
                              , 0 
                              , SHIP_BURST_ANIM_DELAY
                              , 0 , 0 
                            };

        SDL_Rect colBox = { 12 , 7 , 8 , 14 };
		GameObject *newBurst = newObjANDpull( &OAM_effects , &colBox , &colBox
						 , EFFECTS_32X32_CHARSET , 0 , 0 , 0 , &anim);
		newShipBurst(newBurst);
	}

    return;
}

/********************************************
    voidShipTrail:
*********************************************/


void shipTrail( GameObject *object)
{
    shipTrailAttributes *objAttributes = (shipTrailAttributes *)object->attributes;

    objAttributes->lifeTime--;
    if( objAttributes->lifeTime == 0)
    {
        freeANDpushObj(&OAM_effects , object);        
        return;
    }

    return;
}


/********************************************
    newShipTrail:
*********************************************/
void newShipTrail( GameObject *object)
{
    object->func = &shipTrail;
    object->attributes = malloc( sizeof(shipTrailAttributes) );
    shipTrailAttributes *objAttributes = (shipTrailAttributes *)object->attributes;
    objAttributes->lifeTime = SHIP_TRAIL_LIFE_TIME;
    return ;
}

//-------------------------------------------

/********************************************
    spawnShipTrail
*********************************************/

void spawnShipTrail( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{

        GmObjectAnim anim = { 
                                SHIP_TRAIL_BASE_FRAME
                              , SHIP_TRAIL_ANIM_FRAMES
                              , 0 
                              , SHIP_TRAIL_ANIM_DELAY
                              , 0 , ANIMATION_REPEAT
                            };

        SDL_Rect colBox = { 0 , 0 , 16 , 16 };
		GameObject *newObj = newObjANDpull( &OAM_effects , &pos , &colBox
						 , EFFECTS_16X16_CHARSET , 0 , 0 , 0 , &anim);
		newShipTrail(newObj);
	}

    return;
}

/********************************************
    generateShipBurst:
*********************************************/


void generateShipBurst( SDL_Rect pos)
{

    spawnShipBurst(pos);
    spawnShipBurstWave(pos);

	return;
}

/*******************************************
    energyShootCharge
*******************************************/

#define ENERGY_SHOOT_CHARGE_BASE_FRAME 11
#define ENERGY_SHOOT_CHARGE_ANIM_FRAMES 11
#define ENERGY_SHOOT_CHARGE_ANIM_DELAY 2

void energyShootCharge( GameObject *object)
{   

    return;
}

//-------------------------------------------

void newEnergyShootCharge( GameObject *object)
{
	//playEffect(SFX_EXPLOSION_1);
	object->func = &energyShootCharge;
	return;
}


//-------------------------------------------

GameObject *generateEnergyShootCharge( SDL_Rect pos)
{
    GameObject *newObj = NULL;
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim objAnim=
                             {      
                                 ENERGY_SHOOT_CHARGE_BASE_FRAME
                                ,ENERGY_SHOOT_CHARGE_ANIM_FRAMES
                                ,0
                                ,ENERGY_SHOOT_CHARGE_ANIM_DELAY 
                                ,0,0
                             };
		pos.w = 32;
		pos.h = 32;
        SDL_Rect colBox = { 0 , 0 , pos.w , pos.h};
		newObj = newObjANDpull(  &OAM_effects , &pos , &colBox
		                       , EFFECTS_32X32_CHARSET , 0 , 0 , 0 , &objAnim);
		newEnergyShootCharge(newObj);
	}
	return newObj;
}

//-------------------------------------------

/*******************************************
    multExplosions
*******************************************/


void multExplosions( GameObject *object)
{

    multExplosionsAttributes *objAttributes;
    objAttributes = (multExplosionsAttributes *)object->attributes;

    if(objAttributes->delayCounter > 0)
    {
        objAttributes->delayCounter--;
    }
    else
    {
        if( objAttributes->cWave >= objAttributes->wavesNum)
        {
		    freeANDpushObj(&OAM_effects , object);
            return;                
        }

        objAttributes->delayCounter = objAttributes->waveDelay;
        //
        SDL_Rect explosionPos;
        explosionTmp *cList;    //Current Explosions list
        cList = objAttributes->waves[objAttributes->cWave];
        int i;
        for( i = 0 ; i < explosionsListSize ; i++)
        {
            if( cList[i].spawnFunc == NULL)
            {
                break;
            }
            explosionPos.x = cList[i].pos.x
                             + objAttributes->basePos.x;
            explosionPos.y = cList[i].pos.y
                             + objAttributes->basePos.y;
	        (*( void (*) (SDL_Rect ))(cList[i].spawnFunc))(explosionPos);
        }
        objAttributes->cWave++;

    }
    
    return;
}

//-------------------------------------------

/*******************************************
    mediumMultExplosions
*******************************************/

explosionsList mediumMultExplosionsWaves[MEDIUM_MULT_EXPLOSIONS_WAVES] =
{
    #include "mediumMultExplosionsWaves.inc"
};

void newMediumMultExplosions( GameObject *object)
{
     //
     object->func = &multExplosions;
     //
     object->attributes = malloc( sizeof(multExplosionsAttributes) );
     multExplosionsAttributes *objAttributes = (multExplosionsAttributes *)object->attributes;
     //
     objAttributes->wavesNum = MEDIUM_MULT_EXPLOSIONS_WAVES;
     objAttributes->waveDelay = MEDIUM_MULT_EXPLOSIONS_DELAY;
     objAttributes->waves = mediumMultExplosionsWaves;
     objAttributes->cWave = 0;
     objAttributes->delayCounter = 0;

     objAttributes->basePos = object->pos;
     //This object is not visible on the screen
     object->pos.x = -98;
     object->pos.y = -98;
     object->pos.h = 0;
     object->pos.w = 0;


     return;
}

//-------------------------------------------

void spawnMediumMultExplosions( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
        //This object is not visible on the screen
		pos.w = 0;
		pos.h = 0;
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &pos
						 , EFFECTS_16X16_CHARSET , 0 , 0 , 0 , NULL);
		newMediumMultExplosions(newExplosion);
	}
	return;
}

//-------------------------------------------

/*******************************************
    bigMultExplosions
*******************************************/

explosionsList bigMultExplosionsWaves[BIG_MULT_EXPLOSIONS_WAVES] =
{
    #include "bigMultExplosionsWaves.inc"
};

void newBigMultExplosions( GameObject *object)
{
     //
     object->func = &multExplosions;
     //
     object->attributes = malloc( sizeof(multExplosionsAttributes) );
     multExplosionsAttributes *objAttributes = (multExplosionsAttributes *)object->attributes;
     //
     objAttributes->wavesNum = BIG_MULT_EXPLOSIONS_WAVES;
     objAttributes->waveDelay = BIG_MULT_EXPLOSIONS_DELAY;
     objAttributes->waves = bigMultExplosionsWaves;
     objAttributes->cWave = 0;
     objAttributes->delayCounter = 0;

     objAttributes->basePos = object->pos;
     //This object is not visible on the screen
     object->pos.x = -98;
     object->pos.y = -98;
     object->pos.h = 0;
     object->pos.w = 0;

     return;
}

//-------------------------------------------

void spawnBigMultExplosions( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
        //This object is not visible on the screen
		pos.w = 0;
		pos.h = 0;
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &pos
						 , EFFECTS_16X16_CHARSET , 0 , 0 , 0 , NULL);
		newBigMultExplosions(newExplosion);
	}
	return;
}

//-------------------------------------------

void freePropulsionBurst(GameObject *object)
{
    freeANDpushObj( &OAM_effects , object);
    return;
}

/*******************************************
    propulsionBurst
*******************************************/

void propulsionBurst(GameObject *object)
{
    return;
}

//-------------------------------------------

void newPropulsionBurst(GameObject *object)
{
    object->func = &propulsionBurst;
    return;
}

//-------------------------------------------

GameObject *newBoss1LeftBurst(SDL_Rect pos)
{
    GameObject *newObj = NULL;
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim objAnim=
                             {      
                                 BOSS1_LEFT_BURST_BASE_FRAME
                                ,BOSS1_BURST_ANIM_FRAMES
                                ,0
                                ,BOSS1_BURST_ANIM_DELAY 
                                ,0
                                ,ANIMATION_REPEAT
                             };
		pos.w = 64;
		pos.h = 64;
		newObj = newObjANDpull(  &OAM_effects , &pos , &pos
		                       , EFFECTS_64X64_CHARSET , 0 , 0 , 0 , &objAnim);
		newPropulsionBurst(newObj);
	}
	return newObj;
}

//-------------------------------------------

GameObject *newBoss1RightBurst(SDL_Rect pos)
{
    GameObject *newObj = NULL;
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim objAnim=
                             {      
                                 BOSS1_RIGHT_BURST_BASE_FRAME
                                ,BOSS1_BURST_ANIM_FRAMES
                                ,0
                                ,BOSS1_BURST_ANIM_DELAY 
                                ,0
                                ,ANIMATION_REPEAT
                             };
		pos.w = 64;
		pos.h = 64;
		newObj = newObjANDpull(  &OAM_effects , &pos , &pos
		                       , EFFECTS_64X64_CHARSET , 0 , 0 , 0 , &objAnim);
		newPropulsionBurst(newObj);
	}
    return newObj;
}
//-------------------------------------------

/*******************************************
    stageBossExplosion   
*******************************************/

void stageBossExplosion( GameObject *object)
{
    multExplosionsAttributes *objAttributes;
    objAttributes = (multExplosionsAttributes *)object->attributes;

    if(objAttributes->delayCounter > 0)
    {
        objAttributes->delayCounter--;
    }
    else
    {
        if( objAttributes->cWave >= objAttributes->wavesNum)
        {
            endStage();
		    freeANDpushObj(&OAM_effects , object);
            return;                
        }

        objAttributes->delayCounter = objAttributes->waveDelay;
        SDL_Rect explosionPos;
        explosionTmp *cList;
        cList = objAttributes->waves[objAttributes->cWave];
        int i;
        for( i = 0 ; i < explosionsListSize ; i++)
        {
            if( cList[i].spawnFunc == NULL)
            {
                break;
            }
            explosionPos.x = cList[i].pos.x
                             + objAttributes->basePos.x;
            explosionPos.y = cList[i].pos.y
                             + objAttributes->basePos.y;
	        (*( void (*) (SDL_Rect ))(cList[i].spawnFunc))(explosionPos);
        }
        objAttributes->cWave++;

    }
    
    return;
}
//-------------------------------------------

void newStageBossExplosion( GameObject *object)
{
     //
     object->func = &stageBossExplosion;
     //
     object->attributes = malloc( sizeof(multExplosionsAttributes) );
     multExplosionsAttributes *objAttributes = (multExplosionsAttributes *)object->attributes;
     //
     objAttributes->wavesNum = BIG_MULT_EXPLOSIONS_WAVES;
     objAttributes->waveDelay =  BIG_MULT_EXPLOSIONS_DELAY;
     objAttributes->waves = bigMultExplosionsWaves;
     objAttributes->cWave = 0;
     objAttributes->delayCounter = 0;

     objAttributes->basePos = object->pos;
     //This object is not visible on the screen
     object->pos.x = -98;
     object->pos.y = -98;
     object->pos.h = 0;
     object->pos.w = 0;

     return;
}

//-------------------------------------------

void spawnStageBossExplosion( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
        //This object is not visible on the screen
		pos.w = 0;
		pos.h = 0;
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &pos
						 , EFFECTS_16X16_CHARSET , 0 , 0 , 0 , NULL);
		newStageBossExplosion(newExplosion);
	}
	return;
}

//-------------------------------------------

/*******************************************
    smallShipStarExplosion
*******************************************/

void smallShipStarExplosion( GameObject *object)
{
    SDL_Rect playerPos = getPlayerPosition();
    object->pos.x = playerPos.x-15;
    object->pos.y = playerPos.y-16;

	if( object->anim.counter == 0
	    && object->anim.timer == 0)
    {
        spawnPlayerMultExplosion(object->pos);  
		freeANDpushObj(&OAM_effects , object);
    }
    
    return;
}

//-------------------------------------------

void newSmallShipStarExplosion( GameObject *object)
{
	object->func = &smallShipStarExplosion;
	return;
}


//-------------------------------------------

void spawnSmallShipStarExplosion( SDL_Rect pos)
{
	if( searchEmptyObject( &OAM_effects) >= 0)
	{
		GmObjectAnim explosionAnim={ SMALL_SHIP_STAR_EXP_BASE_FRAME
                                    ,SMALL_SHIP_STAR_EXP_ANIM_FRAMES
                                    ,0
                                    ,SMALL_SHIP_STAR_EXP_ANIM_DELAY
                                    ,0,0 };
		pos.w = 8;
		pos.h = 8;
		GameObject *newExplosion = newObjANDpull( &OAM_effects , &pos , &pos
						 , EFFECTS_64X64_CHARSET , 0 , 0 , 0 , &explosionAnim);
		newSmallShipStarExplosion(newExplosion);
	}
	return;
}

//-------------------------------------------

void spawnPlayerDeathExplosion( SDL_Rect pos)
{
    spawnSmallShipStarExplosion(pos);
    return;
}

//-------------------------------------------

/*******************************************
    spawnPlayerExplosion
*******************************************/

void spawnPlayerMultExplosion( SDL_Rect pos)
{
    enum { explosionNum = 9};
    static const SDL_Rect explosionPanning[explosionNum] = 
    {
        {00,00},
        {00,-32,0,0},{-22,-22,0,0},{-32,00,0,0},{-22,22,0,0},
        {00,32,0,0},{22,22,0,0},{32,00,0,0},{22,-22,0,0}
    };
    
    int i;
    SDL_Rect explosionPos;
    SDL_Rect playerPos = getPlayerPosition();
    for( i = 0 ; i < explosionNum ; i++)
    {
        explosionPos.x = explosionPanning[i].x + playerPos.x;
        explosionPos.y = explosionPanning[i].y + playerPos.y;
        spawnMediumExplosion_2(explosionPos);
    }
	return;
}




