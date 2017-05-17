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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "d_programTypes.h"
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_trajectory.h"
#include "d_enemies.h"
#include "globalVars.h"
//
#include "l_trajectories.h"
#include "trajectory.h"
#include "cMath.h"
#include "math.h"
#include "objects.h"
#include "enemyBullet.h"
#include "player.h"
#include "enemies.h"
#include "effects.h"
#include "announcer.h"
#include "bonus.h"
#include "debug.h"
#include "time.h"

/********************************************/

/********************************************
    spitze:
    Moves following a sinusoidal trajectory
*********************************************/

void spitze( GameObject *object)
{

    spitzeAttributes *objAttributes = (spitzeAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(SPITZE_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}

/********************************************/

trajectStep * spitze_trajectories_list[] =
{
      spitze_trajectory
    , vtSpitzeR_trajectory
    , vtSpitzeL_trajectory
};

/********************************************/

void newSpitze( GameObject  *object , int type)
{
	object->func = &spitze;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(spitzeAttributes) );
    //
    spitzeAttributes *objAttributes = (spitzeAttributes *)object->attributes;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , spitze_trajectories_list[type] );


	return;
}

/********************************************/

void makeSpitze(SDL_Rect pos , int type)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {  SPITZE_BASE_FRAME
                              , SPITZE_ANIM_FRAMES
                              , 0 
                              , SPITZE_FRAME_DELAY
                              , 0
                              , ANIMATION_REPEAT};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 0 , 0 , 16 , 16};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_16X16_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newSpitze( newObjPtr , type );
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************
    spitze:
    Moves to the bottom of the screen
    following an horizontal sinusoidal trajectory

*********************************************/

void spawnSpitze(SDL_Rect pos)
{
    makeSpitze(pos , STD_SPITZE );
    return;
}

/********************************************

    vtspitze:
    (Vertical type spitze)
    moves following a vertical sinusoidal trajectory

********************************************* 
    vtSpitzeR: that moves to the right
*********************************************/
void spawnVtSpitzeR( SDL_Rect pos)
{
    makeSpitze( pos , VTSPITZE_R );
    return;
}

/*******************************************
VtSpitzeL that moves to the left
********************************************/

void spawnVtSpitzeL( SDL_Rect pos)
{
    makeSpitze( pos , VTSPITZE_L );
    return;
}

/*******************************************
    Archer
********************************************/

void archer( GameObject *object)
{

    archerAttributes *objAttributes = (archerAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > ARCHER_LIFE)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(ARCHER_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    //Shoot
    objAttributes->shootDelay--;
    if(objAttributes->shootDelay<= 0)
    {
        objAttributes->shootDelay = ARCHER_SHOOT_DELAY;

        SDL_Rect bulletPos =
            { 
                  object->pos.x 
                , object->pos.y
                , 0
                , 0
            };

        SDL_Rect playerPos = getPlayerPosition();
        double bulletDx = playerPos.x - object->pos.x;
        double bulletDy = playerPos.y - object->pos.y;
        double rad = sqrt(pow( bulletDx , 2) + pow( bulletDy , 2));

        makeSmallEnemyBullet( bulletPos 
                              , ( bulletDx / rad) * LAROC_BULLET_SPEED
                              , ( bulletDy / rad) * LAROC_BULLET_SPEED
                            );
    }

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}

/********************************************/

trajectStep * archer_trajectories_list[] =
{
    r_archer_trajectory
    ,l_archer_trajectory
};

/********************************************/

void newArcher( GameObject  *object , int type)
{
	object->func = &archer;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(archerAttributes) );
    //
    archerAttributes *objAttributes = (archerAttributes *)object->attributes;
    //
    objAttributes->shootDelay = ARCHER_SHOOT_DELAY;

    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , archer_trajectories_list[type] );



	return;
}

void makeArcher(SDL_Rect pos , int type)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {  ARCHER_BASE_FRAME
                              , ARCHER_ANIM_FRAMES
                              , 0 
                              , ARCHER_FRAME_DELAY
                              , 0
                              , 0};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 0 , 0 , 16 , 16};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_16X16_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newArcher( newObjPtr , type );
	}
	return;
}

/*******************************************/

void spawnR_archer(SDL_Rect pos)
{
    makeArcher(pos , R_ARCHER  );
    return;
}

/*******************************************/

void spawnL_archer(SDL_Rect pos)
{
    makeArcher(pos , L_ARCHER  );
    return;
}



/********************************************
    gaucer:
    Rotates over himself and shoots multiple bullets
*********************************************/

void gaucer( GameObject *object)
{
    //Round Bullets direction
	static int bullets_dir[GAUCER_ROUNDS_NUM][GAUCER_ROUND_BULLETS*2] =
	{
        //ROUND 0
        { 
		   0 , -1,
		    1 , 0,
		    0 , 1,
		   -1 , 0,
        }
        //ROUND 1
        ,{ 
		    1 , -1,
		    1 , 1,
		   -1 , 1,
		   -1 , -1
        }
	};

    gaucerAttributes *objAttributes = (gaucerAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > GAUCER_LIFE )
	{
		spawnSmallExplosion_1( object->pos);
        incScore(GAUCER_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );
    
    if( getStepNumber( &(objAttributes->trajectory) ) != GAUCER_SHOOT_STEP)
        return;

    //Update shoot delay counter and return
	objAttributes->shootDelay--;
	if( objAttributes->shootDelay > 0)
		return;

    //Time to Shoot!

    //Restore shoot delay counter
    objAttributes->shootDelay = GAUCER_SHOOT_DELAY;

    //Change the type of bullet round
    objAttributes->roundType = ( objAttributes->roundType+1 ) % GAUCER_ROUNDS_NUM;

    //Shoot the round bullets!
	SDL_Rect bulletPos;
	register i;
	for(i = 0 ; i < GAUCER_ROUND_BULLETS ; i++)
	{
            bulletPos.x = object->pos.x + bullets_dir[objAttributes->roundType][i*2] * GAUCER_BURST_RADIUS;
			bulletPos.y = object->pos.y + bullets_dir[objAttributes->roundType][i*2+1] * GAUCER_BURST_RADIUS;
            //
		    makeSmallEnemyBullet(   bulletPos 
                                  , bullets_dir[ objAttributes->roundType ][i*2]
                                  , bullets_dir[ objAttributes->roundType ][i*2+1]
                                );
	}
	return;
}

void newGaucer( GameObject  *object)
{
	object->func = &gaucer;
	object->attributes = malloc( sizeof(gaucerAttributes) );
    gaucerAttributes *objAttributes = (gaucerAttributes *)object->attributes;

    objAttributes->roundType = 0;
	objAttributes->shootDelay = GAUCER_SHOOT_DELAY;
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , gaucer_trajectory );
	return;
}

void spawnGaucer(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { GAUCER_BASE_FRAME , GAUCER_ANIM_FRAMES , 0 , 5 , 0 , ANIMATION_REPEAT };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;

		SDL_Rect colBox = { 0 , 0 , 16 , 16};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  ,ENEMIES_16X16_CHARSET , 5 , 0 , 1 , &enemyAnim);
		newGaucer( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************
    lovac:
    Move horizontally and then chase the player vertically
*********************************************/


void lovac( GameObject *object)
{
    lovacAttributes *objAttributes = (lovacAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0 )
	{
        SDL_Rect expPos = {  object->pos.x+8 
                           , object->pos.y+8
                           , 16 , 16
                          };
		spawnSmallExplosion_1( expPos);
        incScore(LOVAC_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(/*    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||*/object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    SDL_Rect playerPos = getPlayerPosition();

	switch(objAttributes->state)
	{
		case LOVAC_CHASE:

			object->pos.y += objAttributes->yvel;
		    break;
        //
		case LOVAC_MOVLEFT:

			if( object->pos.x <= playerPos.x)
			{
                //start the player chase!
				objAttributes->state = LOVAC_CHASE;
				break;
			}

			object->pos.x -= objAttributes->xvel;
		    break;
        //
		case LOVAC_MOVRIGHT:
        
			if( object->pos.x >= playerPos.x)
			{
                //start the player chase!
				objAttributes->state = LOVAC_CHASE;
				break;
			}
			object->pos.x += objAttributes->xvel;
		    break;
	}
	return;
}

//-------------------------------------------

void newLovac( GameObject  *object)
{
	object->func = &lovac;
	object->attributes = malloc( sizeof(lovacAttributes) );
    lovacAttributes *objAttributes = (lovacAttributes *)object->attributes;
	objAttributes->xvel = LOVAC_XVEL;
	objAttributes->yvel = LOVAC_YVEL;

    SDL_Rect playerPos = getPlayerPosition();

	//Set lovac status
	if( object->pos.x > playerPos.x)
	{
		objAttributes->state = LOVAC_MOVLEFT;
	}
	else if( object->pos.x == playerPos.x)
	{
		objAttributes->state = LOVAC_CHASE;
	}
	else
	{
		objAttributes->state = LOVAC_MOVRIGHT;
	}

	return;
}

//-------------------------------------------

void spawnLovac(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { LOVAC_BASE_FRAME , LOVAC_ANIM_FRAMES , 0 , 2 , 0 , 1 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 4 , 10 , 23 , 13};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_32X32_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newLovac( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

//-------------------------------------------

/********************************************
    zorgen:
    Move down and chase the player horizontally
*********************************************/

void zorgen( GameObject *object)
{
    zorgenAttributes *objAttributes = (zorgenAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0 )
	{
		spawnSmallExplosion_1( object->pos);
        incScore(ZORGEN_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	  /*||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h*/)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    //Update position
	object->pos.y += objAttributes->yvel;
	object->pos.x += objAttributes->xvel;

    SDL_Rect playerPos = getPlayerPosition();

    //Start the player chase
	if( objAttributes->state == ZORGEN_MOVE_DOWN
	    && object->pos.y >= playerPos.y)
	{
        //Change state and stop vertical movement
		objAttributes->state = ZORGEN_CHASE;
		objAttributes->yvel = 0;

        //Determine the horizontal sense
		if( object->pos.x >= playerPos.x)
		{
			object->tile = ZORGEN_LEFT_FRAME;
			objAttributes->xvel = -ZORGEN_XVEL;
		}
		else
		{
			object->tile =  ZORGEN_RIGHT_FRAME;
			objAttributes->xvel = ZORGEN_XVEL;
		}
	}

	return;
}

//-------------------------------------------

void newZorgen( GameObject  *object)
{
	object->func = &zorgen;
	object->attributes = malloc( sizeof(zorgenAttributes) );
	((zorgenAttributes *)object->attributes)->xvel = 0;
	((zorgenAttributes *)object->attributes)->yvel = ZORGEN_YVEL;

	//Set zorgen state
	((zorgenAttributes *)object->attributes)->state = ZORGEN_MOVE_DOWN;

	return;
}

//-------------------------------------------


void spawnZorgen(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { ZORGEN_BASE_FRAME , 0 , 0 , 0 , 0 , 0 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 1 , 1 , 14 , 14};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_16X16_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newZorgen( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

//-------------------------------------------

/********************************************
    litec:
    Align horizontaly with the player 
    and shoot missiles
*********************************************/

void litec(GameObject *object)
{
	static SDL_Rect missilePos;

    litecAttributes *objAttributes = (litecAttributes *)object->attributes;

    //Delete if it is outside the screen
	if( object->pos.y > GM_SCREEN_H + object->pos.h )
	{
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

    //Manage collision damage
    if(object->collision > LITEC_LIFE )
	{
        spawnMediumMultExplosions( object->pos);
        incScore(LITEC_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

    //Horizontal movement
	if(    objAttributes->missiles == 0
        || object->pos.y < LITEC_SHOOTING_YPOS)
	{       
			object->pos.y+= LITEC_YMOV_SPEED;
	}
    //Aim and shoot at the player!
	else
	{
        SDL_Rect playerPos = getPlayerPosition();
		//Aim to the player
		if( object->pos.x > playerPos.x)
			object->pos.x -= LITEC_XMOV_SPEED;
		else
			object->pos.x+= LITEC_XMOV_SPEED;


        //Shoot at the player!
		if( objAttributes->missileDelay == 0)
		{
            missilePos.x = object->pos.x+8;
            missilePos.y = object->pos.y+28;

            //Shoot left missile
			spawnEnemyMissile(missilePos);

            //Shoot right missile
			missilePos.x += 30;
			spawnEnemyMissile(missilePos);

			objAttributes->missiles--;
			objAttributes->missileDelay = LITEC_MISSILE_DELAY;
		}
		else objAttributes->missileDelay--;
	}

	return;
}

//-------------------------------------------

void newLitec(GameObject *object)
{
	object->func = &litec;
	object->attributes = malloc( sizeof(litecAttributes) );

	((litecAttributes *)object->attributes)->missiles = LITEC_MISSILES_NUM;
	((litecAttributes *)object->attributes)->missileDelay = LITEC_MISSILE_DELAY;
	return;
}

//-------------------------------------------

void spawnLitec(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { LITEC_BASE_FRAME , 0 , 0 , 0 , 0 , 0 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 2 , 7 , 60 , 18};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_64X64_CHARSET , 10 , 0 , 1 , &enemyAnim);
		newLitec( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

//-------------------------------------------

/********************************************
    enemyMissile:
    Enemy missile.
*********************************************/

void enemyMissile( GameObject *object)
{
    //Delete if it is outside the screen
	if( object->pos.y > GM_SCREEN_H + object->pos.h )
	{
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

    //Manage collision damage
    if(object->collision > ENEMY_MISSILE_LIFE)
    {
		spawnSmallExplosion_1( object->pos);
        incScore(ENEMY_MISSILE_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
        return;
    }

	object->pos.y+= ENEMY_MISSILE_SPEED;

	return;

}

//-------------------------------------------

void newEnemyMissile(GameObject *object)
{
	object->func = &enemyMissile;
	return;
}

//-------------------------------------------

void spawnEnemyMissile(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { ENEMY_MISSILE_BASE_FRAME , 0 , 0 , 0 , 0 , 0 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 48;
		pos.h = 224;
		SDL_Rect colBox = { 3 , 4 , 9 , 13};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_16X16_CHARSET , 10 , 0 , 1 , &enemyAnim);
		newEnemyMissile( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

//-------------------------------------------


/********************************************
    farben:
    Move horizontally while performing loops
*********************************************/


trajectStep *farben_trajectories_list[] =
{
      farbenL_trajectory
     ,farbenR_trajectory
};

//-------------------------------------------

void farben(GameObject *object)
{

    farbenAttributes *objAttributes = (farbenAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(FARBEN_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside of the range
	if(    object->pos.x > GM_SCREEN_W-1 + FARBEN_TRAJECT_AMP
	   ||  object->pos.x < -FARBEN_TRAJECT_AMP              )
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

}

//-------------------------------------------

void newFarben( GameObject  *object , int type )
{
	object->func = &farben;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(farbenAttributes) );
    //
    farbenAttributes *objAttributes = (farbenAttributes *)object->attributes;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , farben_trajectories_list[type] );

	return;
}

//-------------------------------------------

void makeFarben(SDL_Rect pos , int type )
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { FARBEN_BASE_FRAME , FARBEN_ANIM_FRAMES , 0 , 1 , 0 , 1 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 3 , 9 , 28 , 14};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_32X32_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newFarben( newObjPtr , type );
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************
    SpawnFarbenL:
        Spawn farben that goes to the LEFT
*********************************************/

void spawnFarbenL(SDL_Rect pos)
{
    makeFarben( pos , TYPE_FARBEN_L);
    return;
}

//-------------------------------------------

/********************************************
    SpawnFarbenL:
        Spawn farben that goes to the RIGHT
*********************************************/

void spawnFarbenR(SDL_Rect pos)
{
    makeFarben( pos , TYPE_FARBEN_R);
    return;
}

//-------------------------------------------


/********************************************
    petrus:
    rushes to the player
*********************************************/

void petrus(GameObject *object)
{

    petrusAttributes *objAttributes = (petrusAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(PETRUS_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

	//if the object is outside of the game zone
	//delete it
	if(  object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y < -object->pos.h
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->collision > 0)
	{
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

    //Move the object in the direction & sense of the direction vector
	objAttributes->fx += PETRUS_VEL * objAttributes->dx;
	objAttributes->fy += PETRUS_VEL * objAttributes->dy;

	object->pos.x = objAttributes->fx;
	object->pos.y = objAttributes->fy;


}

//-------------------------------------------

void newPetrus( GameObject  *object)
{
	object->func = &petrus;

	object->attributes = malloc(sizeof(petrusAttributes) );
    petrusAttributes *objAttributes = (petrusAttributes *)object->attributes;

	objAttributes->fx = object->pos.x;
	objAttributes->fy = object->pos.y;

    //Calculate vector that goes from enemy (a) to player (b)
    SDL_Rect playerPos = getPlayerPosition();
	double dx = playerPos.x - object->pos.x;
	double dy = playerPos.y - object->pos.y;

    //Normalize the vector and get the enemy movement direction
	objAttributes->dx = dx / sqrt(pow(dx , 2) + pow(dy , 2)) ;
	objAttributes->dy = dy / sqrt(pow(dx , 2) + pow(dy , 2)) ;
}

//-------------------------------------------

void spawnPetrus(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { PETRUS_BASE_FRAME , 4 , 0 , 5 , 0 , ANIMATION_REPEAT };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 2 , 1 , 12 , 16};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_16X16_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newPetrus( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

//------------------------------------------------

/********************************************
 jumpa:
    Jumps from one place to another
*********************************************/

void jumpa(GameObject *object)
{

	jumpaAttributes *objAttributes = (jumpaAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(JUMPA_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

    return;
}

//------------------------------------------------

enum jumpaTypes { JUMPA_LU , JUMPA_RU , JUMPA_LD , JUMPA_RD };
//
trajectStep * jumpa_trajectories_list[] =
{
       jumpaLU_trajectory
      ,jumpaRU_trajectory
      ,jumpaLD_trajectory
      ,jumpaRD_trajectory
};

//------------------------------------------------

void newJumpa( GameObject  *object , int type)
{
	object->func = &jumpa;
	object->attributes = malloc(sizeof(jumpaAttributes) );

    jumpaAttributes *objAttributes = (jumpaAttributes *)object->attributes;

    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , jumpa_trajectories_list[type] );

	return;
}

//------------------------------------------------

void makeJumpa(SDL_Rect pos , int type)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { JUMPA_BASE_FRAME , 2 , 0 , 20 , 0 , ANIMATION_REPEAT  };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 2 , 1 , 14 , 16};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_16X16_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);

	    newJumpa( newObjPtr , type);

#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

//------------------------------------------------

void spawnJumpaLD( SDL_Rect pos)
{
    makeJumpa( pos , JUMPA_LD );
}

//------------------------------------------------

void spawnJumpaRD( SDL_Rect pos)
{
    makeJumpa( pos , JUMPA_RD );
}

//------------------------------------------------

void spawnJumpaLU( SDL_Rect pos)
{
    makeJumpa( pos , JUMPA_LU );
}

//------------------------------------------------

void spawnJumpaRU( SDL_Rect pos)
{
    makeJumpa( pos , JUMPA_RU );
}

//------------------------------------------------

/********************************************
 stom:
    move in circles
*********************************************/

void stom( GameObject *object)
{
    stomAttributes *objAttributes = (stomAttributes *)object->attributes;

	//If the object is outside of the game zone
	//delete it
	if(  object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y < -object->pos.h
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h)
	{
        freeANDpushObj( &OAM_enemies , object);
		return;
	}
	//If there is collision damage delete it
    if( object->collision > 0)
    {
		spawnSmallExplosion_1( object->pos);
        incScore(STOM_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
		return;
    }

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );
    

    return;
}

//------------------------------------------------

void *stomTrayectories[] =
{
    stomLD_trajectory,
    stomRD_trajectory,
    stomLU_trajectory,
    stomRU_trajectory,
};

enum stomTypes { STOM_LD , STOM_RD , STOM_LU , STOM_RU };

//------------------------------------------------

void newStom( GameObject *object , int type)
{
    object->func = &stom;
	object->attributes = malloc(sizeof(stomAttributes) );
    stomAttributes *objAttributes = (stomAttributes *)object->attributes;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , *((trajectStep **)stomTrayectories + type) );
    
    return;
}

//------------------------------------------------

void spawnStom(SDL_Rect pos , int type)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { STOM_BASE_FRAME , STOM_ANIM_FRAMES , 0 , 0 , 0 , 0 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 2 , 1 , 12 , 16};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_16X16_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newStom( newObjPtr , type);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}


//------------------------------------------------

void spawnStomLD(SDL_Rect pos)
{
     spawnStom(pos , STOM_LD);
     return;
}

//------------------------------------------------


void spawnStomRD(SDL_Rect pos)
{
    spawnStom(pos , STOM_RD);
    return;
}

//------------------------------------------------

void spawnStomLU(SDL_Rect pos)
{
    spawnStom(pos , STOM_LU);
    return;
}

//------------------------------------------------

void spawnStomRU(SDL_Rect pos)
{
    spawnStom(pos , STOM_RU);
    return;
}

//------------------------------------------------

/********************************************
    Laroc
*********************************************/

void laroc( GameObject *object)
{
    larocAttributes *objAttributes = (larocAttributes *)object->attributes;

    //Manage collision damage
    if(object->collision > LAROC_LIFE )
	{
        spawnMediumMultExplosions( object->pos);
        incScore(LAROC_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

  
	//Delete if it is outside the screen
	if(/*  object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||*/  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );


    //Fire!

    /* Update round delay Counter */
    if( objAttributes->roundDelayCounter > 0)
        objAttributes->roundDelayCounter-- ;
    else
    {   
        /*Round shoot*/

        /*Update bullet delay counter*/
        if( objAttributes->bulletsDelayCounter > 0)
        {
            objAttributes->bulletsDelayCounter-- ;
        }
        else
        {
            /*Shoot bullet*/

            objAttributes->bulletsDelayCounter = LAROC_BULLETS_DELAY;

            SDL_Rect bulletPos =
            { 
                  object->pos.x + LAROC_SPRITE_CENTER_X 
                , object->pos.y + LAROC_SPRITE_CENTER_Y
                , 0
                , 0
            };
            SDL_Rect playerPos = getPlayerPosition();
            double bulletDx = playerPos.x - object->pos.x;
            double bulletDy = playerPos.y - object->pos.y;
            double rad = sqrt(pow( bulletDx , 2) + pow( bulletDy , 2));

            makeSmallEnemyBullet(     bulletPos 
                                  , ( bulletDx / rad) * LAROC_BULLET_SPEED
                                  , ( bulletDy / rad) * LAROC_BULLET_SPEED
                                );

            /* */
            objAttributes->bulletsCounter--;
            if( objAttributes->bulletsCounter == 0 )
            {
                objAttributes->roundDelayCounter = LAROC_ROUND_DELAY;
                objAttributes->bulletsCounter = LAROC_BULLETS_PER_ROUND;                
            }
            /* */
        }
    }
    

    return;
}

//------------------------------------------------

void newLaroc( GameObject *object)
{
    object->func = &laroc;
	object->attributes = malloc(sizeof(larocAttributes) );

    larocAttributes *objAttributes = (larocAttributes *)object->attributes;
    /**/
    objAttributes->roundDelayCounter = LAROC_ROUND_DELAY;
    objAttributes->bulletsCounter = LAROC_BULLETS_PER_ROUND;
    objAttributes->bulletsDelayCounter = LAROC_BULLETS_DELAY;
    /**/
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , laroc_trajectory );


    return;
}

//------------------------------------------------

void spawnLaroc(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {
                                LAROC_BASE_FRAME
                              , LAROC_ANIM_FRAMES
                              , 0 
                              , LAROC_FRAME_DELAY
                              , 0
                              , ANIMATION_REPEAT
                            };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 12 , 10 , 38, 41};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_64X64_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newLaroc( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
}

//------------------------------------------------


/********************************************
    anancitesST:
*********************************************/

void freeAnancitesST( GameObject *object)
{

    anancitesSTAttributes *objAttributes = (anancitesSTAttributes *)object->attributes;

    /*Free charge animation object if any*/
    if( objAttributes->shootChargeAnim != NULL)
    {
             freeEffect(objAttributes->shootChargeAnim);
             objAttributes->shootChargeAnim = NULL;
    }
    /*Free object*/
    freeANDpushObj( &OAM_enemies , object);
    return;   
}

void anancitesST( GameObject *object)
{
    anancitesSTAttributes *objAttributes = (anancitesSTAttributes *)object->attributes;

    //Delete when it goes out of the screen
	if(    object->pos.y > GM_SCREEN_H + object->pos.y
        || object->pos.x > GM_SCREEN_W + object->pos.w
        || object->pos.x < -object->pos.w)
	{
		freeAnancitesST(object);
		return;
	}

    //Move the object to the shooting position
    if( object->pos.y < ANANCITES_ST_SHOOT_RANGE_Y)
    {
        objAttributes->fy += 3.0;
        object->pos.y = objAttributes->fy;
        //Clear collision damage
        object->collision = 0;
        return;
    }

    //Manage collision damage
    if(object->collision > ANANCITES_ST_LIFE )
	{
        spawnMediumMultExplosions( object->pos);
        incScore(ANANCITES_ST_BONUS_POINTS);
		freeAnancitesST(object);
		return;
	}


    if( objAttributes->shootFramesCounter > 0)
    {
        //Energy Shoot ongoing
        objAttributes->shootFramesCounter--;
        return;
    }

    //Vertical oscillation
    objAttributes->fy += objAttributes->stepY;
    objAttributes->stepYcounter--;
    if( objAttributes->stepYcounter == 0)
    {
        objAttributes->stepY *= -1;
        objAttributes->stepYcounter = ANANCITES_STEPS_Y_NUM;
    }
    object->pos.y = objAttributes->fy;

    //Horizontal oscillation
    if( object->pos.x > GM_SCREEN_W - object->pos.w)
    {
        objAttributes->stepX = -1.0*ANANCITES_STEP_X;
    }
    else if( object->pos.x < 0)
    {
        objAttributes->stepX = ANANCITES_STEP_X;
    }
    objAttributes->fx += objAttributes->stepX;
    object->pos.x = objAttributes->fx;
 

    //Energy Shoot Charge Animation
    if( objAttributes->shootChargeAnim != NULL)
    {
        //Update charge animation position
        objAttributes->shootChargeAnim->pos.x = object->pos.x  
                                                + ANANCITES_CHARGE_ANIM_X_PANNIG;
        objAttributes->shootChargeAnim->pos.y = object->pos.y 
                                                + ANANCITES_CHARGE_ANIM_Y_PANNIG;

        if(objAnimEnded( objAttributes->shootChargeAnim))
        {
            //Delete charge effect object
            freeEffect(objAttributes->shootChargeAnim);
            objAttributes->shootChargeAnim = NULL;
            //Set the shoot frames counter
            objAttributes->shootFramesCounter = ANANCITES_SHOOT_FRAMES;
            //Shoot! Shoot! Shoot!
            SDL_Rect energyShootPos = {  object->pos.x + 32
                                        ,object->pos.y + 70
                                        , 0 ,0 };

            spawnEnergyShoot(energyShootPos);
        }

        return;
    }


    objAttributes->shootDelayCounter--;
    if( objAttributes->shootDelayCounter <= 0)
    {
        //Set timer
        objAttributes->shootDelayCounter = ANANCITES_SHOOT_DELAY;
        //Spawn shoot charge effect
        SDL_Rect chargePos =
        {
             object->pos.x + ANANCITES_CHARGE_ANIM_X_PANNIG 
            ,object->pos.y + ANANCITES_CHARGE_ANIM_Y_PANNIG 
            ,0
            ,0
        };
        //
        objAttributes->shootChargeAnim = generateEnergyShootCharge(chargePos);            
    }

    return;
}

void newAnancitesST( GameObject *object)
{
    object->func = &anancitesST;
    object->attributes = malloc(sizeof(anancitesSTAttributes) );
    //
    anancitesSTAttributes *objAttributes = (anancitesSTAttributes *)object->attributes;
    //
    objAttributes->shootDelayCounter =  ANANCITES_SHOOT_DELAY;
    objAttributes->shootChargeAnim = NULL;
    objAttributes->fx = object->pos.x;
    objAttributes->fy = object->pos.y;
    objAttributes->stepY = ANANCITES_STEP_Y;
    objAttributes->stepX = ANANCITES_STEP_X;
    objAttributes->stepYcounter = ANANCITES_STEPS_Y_NUM;
    objAttributes->shootFramesCounter = 0;
    
    return;
}

//------------------------------------------------

void spawnAnancitesST(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { 
                                  ANANCITES_ST_BASE_FRAME 
                                , ANANCITES_ST_ANIM_FRAMES
                                , 0 
                                , ANANCITES_ST_FRAME_DELAY
                                , 0 , ANIMATION_REPEAT
                              };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 80;
		pos.h = 80;
		SDL_Rect colBox = { 4 , 14 , 72, 42};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_80X80_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newAnancitesST( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
}

//------------------------------------------------

/********************************************
    midBoss
*********************************************/

void midBoss( GameObject *object)
{
    enum { midBossBulletsDirListNum = 40};
    static vector bulletsDir[midBossBulletsDirListNum] =
    {
         { 1.00,-2.00},{ 3.00,-2.00},{ 2.00,-3.00},{ 1.00,-2.00},{-1.00,-2.00}
        ,{-1.00,-2.00},{ 3.00, 2.00},{-3.00,-3.00},{ 2.00, 2.00},{ 3.00,-1.00}
        ,{ 1.00, 1.00},{-1.00,-1.00},{-1.00,-2.00},{-2.00,-2.00},{ 3.00,-3.00}
        ,{-3.00,-3.00},{ 3.00, 1.00},{-2.00, 2.00},{-2.00, 2.00},{ 1.00, 1.00}
        ,{ 1.00, 3.00},{-1.00,-2.00},{-1.00,-1.00},{ 3.00,-3.00},{ 3.00, 2.00}
        ,{-2.00,-1.00},{-1.00, 1.00},{-1.00,-1.00},{ 2.00,-2.00},{ 1.00, 1.00}
        ,{-1.00, 1.00},{-1.00, 3.00},{ 2.00, 1.00},{-2.00, 3.00},{-3.00, 1.00}
        ,{-3.00, 1.00},{-1.00, 1.00},{-1.00,-1.00},{ 1.00,-1.00},{-2.00, 2.00}
    };

    midBossAttributes *objAttributes = (midBossAttributes *)object->attributes;
    //Manage collision damage
    if(object->collision > MIDBOSS_LIFE )
	{
        spawnBigMultExplosions( object->pos);
        incScore(MID_BOSS_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);  
		return;
	}

    updateTrajectory( &(object->pos), &(objAttributes->trajectory));

    if( objAttributes->shootDelay == 0)
    {
        if(objAttributes->cBullet >= midBossBulletsDirListNum)
            objAttributes->cBullet = 0;            
        //
        SDL_Rect bulletPos = { object->pos.x+24 , object->pos.y+24 , 0 ,0};
        makeGatlingBullet(  &bulletPos
                          , bulletsDir[objAttributes->cBullet].x 
                          , bulletsDir[objAttributes->cBullet].y) ;

        objAttributes->shootDelay = 5;
        objAttributes->cBullet++;
    }
    else
    {
        objAttributes->shootDelay--;
    }

    return;
}

/********************************************/

void newMidBoss( GameObject *object)
{
    object->func = &midBoss;
    object->attributes = malloc(sizeof(midBossAttributes));
    midBossAttributes *objAttributes = (midBossAttributes *)object->attributes;
    //
    initTrajectory( &(objAttributes->trajectory)
                    , &(object->pos)
                    , midBoss_trajectory);

    objAttributes->shootDelay = 0;
    objAttributes->cBullet = 0;
    return;
}

/********************************************/

void spawnMidBoss(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { MIDBOSS_BASE_FRAME , MIDBOSS_ANIM_FRAMES , 0 , 8 , 0 , 1 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 0 , 0 , 64, 64};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_64X64_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);

		newMidBoss( newObjPtr);
	}
}

/********************************************/

/********************************************
    midBossPart
*********************************************/

/********************************************/
//L/R = Left/Right
//U/D = Up/Down

enum
{ 
      MB_MAIN_PART = 0 
    , MB_RU_PART 
    , MB_LD_PART
    , MB_RD_PART
    , MID_BOSS_PARTS_NUM
};
//
int midBossPartTile[MID_BOSS_PARTS_NUM] =
{
     LU_MIDBOSS_P_BASE_FRAME
    ,RU_MIDBOSS_P_BASE_FRAME
    ,LD_MIDBOSS_P_BASE_FRAME
    ,RD_MIDBOSS_P_BASE_FRAME
};

trajectStep *midBossPartsTrayectories[MID_BOSS_PARTS_NUM] =
{
     LU_midBossP_trajectory
    ,RU_midBossP_trajectory
    ,LD_midBossP_trajectory
    ,RD_midBossP_trajectory
};

SDL_Rect midBossPartsInitPos[MID_BOSS_PARTS_NUM] =
{
     {-32,-32,0,0}
    ,{320,-32,0,0}
    ,{-32,240,0,0}
    ,{320,240,0,0}
};

/********************************************/

void midBossPart( GameObject *object)
{
    midBossPartAttributes *objAttributes = (midBossPartAttributes *)object->attributes;
    if( trajectoryEnded(&(objAttributes->trajectory)) )
    {
        if(objAttributes->mainPart)
        {
            spawnMidBoss(object->pos);
        }
        //Delete object
		freeANDpushObj( &OAM_enemies , object);  
        return;
    }
    updateTrajectory( &(object->pos), &(objAttributes->trajectory));
    

    return;
}

/********************************************/

void newMidBossPart( GameObject *object , int part)
{
    object->func = &midBossPart;
    object->attributes = malloc(sizeof(midBossPartAttributes));
    //
    midBossPartAttributes *objAttributes = (midBossPartAttributes *)object->attributes;
    //
    objAttributes->mainPart = ( part == MB_MAIN_PART ? 1 : 0);
    //
    initTrajectory( &(objAttributes->trajectory)
                    , &(object->pos)
                    , midBossPartsTrayectories[part] );
    return;
}

/********************************************/

void makeMidBossPart(SDL_Rect pos , int part)
{
    if(part >= MID_BOSS_PARTS_NUM)
        return;

	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = { midBossPartTile[part] , 0 , 0 , 0 , 0 , 0 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 0 , 0 , 32, 32};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_32X32_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newMidBossPart( newObjPtr , part);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
}

/********************************************/

/********************************************
    midBossEntrance
*********************************************/

void midBossEntrance(GameObject *object)
{
    setWarningMsg();
    makeMidBossPart( midBossPartsInitPos[MB_MAIN_PART] , MB_MAIN_PART );
    makeMidBossPart( midBossPartsInitPos[MB_RU_PART] , MB_RU_PART );
    makeMidBossPart( midBossPartsInitPos[MB_LD_PART] , MB_LD_PART );
    makeMidBossPart( midBossPartsInitPos[MB_RD_PART] , MB_RD_PART );

    //Delete object
    freeANDpushObj( &OAM_enemies , object);
    
    return;
}

/********************************************/

void newMidBossEntrance( GameObject *object)
{
    object->func = &midBossEntrance;
    object->attributes = NULL;
    return;
}

/********************************************/

void spawnMidBossEntrance(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {0 , 0 , 0 , 0 , 0 , 0 };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
        //Put it out of the screen
        pos.x = -32;
        pos.y = -32;

		SDL_Rect colBox = { 0 , 0 , 1, 1};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_16X16_CHARSET , enemyAnim.base , 0 , 0 , &enemyAnim);

		newMidBossEntrance( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
}

/********************************************/

/********************************************
    nacreeb
*********************************************/

void nacreeb( GameObject *object)
{
    larocAttributes *objAttributes = (larocAttributes *)object->attributes;

    //Manage collision damage
    if(object->collision > LAROC_LIFE )
	{
        spawnMediumMultExplosions( object->pos);
        incScore(LAROC_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
		return;
	}

  
	//Delete if it is outside the screen
	if(/*  object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||*/  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );


    //Fire!

    /* Update round delay Counter */
    if( objAttributes->roundDelayCounter > 0)
        objAttributes->roundDelayCounter-- ;
    else
    {   
        /*Round shoot*/

        /*Update bullet delay counter*/
        if( objAttributes->bulletsDelayCounter > 0)
        {
            objAttributes->bulletsDelayCounter-- ;
        }
        else
        {
            /*Shoot bullet*/

            objAttributes->bulletsDelayCounter = LAROC_BULLETS_DELAY;

            SDL_Rect bulletPos =
            { 
                  object->pos.x + LAROC_SPRITE_CENTER_X 
                , object->pos.y + LAROC_SPRITE_CENTER_Y
                , 0
                , 0
            };
            SDL_Rect playerPos = getPlayerPosition();
            double bulletDx = playerPos.x - object->pos.x;
            double bulletDy = playerPos.y - object->pos.y;
            double rad = sqrt(pow( bulletDx , 2) + pow( bulletDy , 2));

            makeSmallEnemyBullet(     bulletPos 
                                  , ( bulletDx / rad) * LAROC_BULLET_SPEED
                                  , ( bulletDy / rad) * LAROC_BULLET_SPEED
                                );

            /* */
            objAttributes->bulletsCounter--;
            if( objAttributes->bulletsCounter == 0 )
            {
                objAttributes->roundDelayCounter = LAROC_ROUND_DELAY;
                objAttributes->bulletsCounter = LAROC_BULLETS_PER_ROUND;                
            }
            /* */
        }
    }
    

    return;
}

//------------------------------------------------

void newNacreeb( GameObject *object)
{
    object->func = &laroc;
	object->attributes = malloc(sizeof(larocAttributes) );

    larocAttributes *objAttributes = (larocAttributes *)object->attributes;
    /**/
    objAttributes->roundDelayCounter = LAROC_ROUND_DELAY;
    objAttributes->bulletsCounter = LAROC_BULLETS_PER_ROUND;
    objAttributes->bulletsDelayCounter = LAROC_BULLETS_DELAY;
    /**/
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , laroc_trajectory );


    return;
}

//------------------------------------------------

void spawnNacreeb(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = 
                             {
                                LAROC_BASE_FRAME
                              , LAROC_ANIM_FRAMES
                              , 0 
                              , LAROC_FRAME_DELAY
                              , 0
                              , ANIMATION_REPEAT
                            };

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 12 , 10 , 38, 41};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_64X64_CHARSET , enemyAnim.base , 0 , 1 , &enemyAnim);
		newLaroc( newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
}

//------------------------------------------------

/********************************************/

/********************************************
    Go-Drill:
*********************************************/

void godrill( GameObject *object)
{
    static const double maxStepY = 2.5;
    static const double stepYinc = 0.02; 
    godrillAttributes *objAttributes = (godrillAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > GODRILL_LIFE)
	{   
        incScore(GODRILL_BONUS_POINTS);
        spawnMediumMultExplosions( object->pos);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    //
    objAttributes->fy += objAttributes->stepY;
    if( objAttributes->stepY < maxStepY)
        objAttributes->stepY += stepYinc;
    //
    object->pos.y = objAttributes->fy;
 

	return;
}

/********************************************/

void newGodrill( GameObject  *object)
{
	object->func = &godrill;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(godrillAttributes) );
    //
    godrillAttributes *objAttributes = (godrillAttributes *)object->attributes;
    objAttributes->stepY = 0.0;
    objAttributes->fy = object->pos.y;

	return;
}

/********************************************/

void spawnGodrill(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {  GODRILL_BASE_FRAME
                              , GODRILL_ANIM_FRAMES
                              , 0 
                              , GODRILL_FRAME_DELAY
                              , 0
                              , ANIMATION_REPEAT};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 19 , 13 , 26 , 48};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_64X64_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newGodrill(newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}


/********************************************
    starView
    satellite equiped with a laser ring weapon
*********************************************/


void starView( GameObject *object)
{
     enum {
             SHOOT_POSITION_Y = 32
            ,OSC_AMPLITUDE = 10
          };

     starViewAttributes *objAttributes = (starViewAttributes *)object->attributes;

    //Entrance
    if( object->pos.y < SHOOT_POSITION_Y)
    {
        objAttributes->fy++;
        object->pos.y = objAttributes->fy;
        return;
    }

    //Handle collision damage
	if( object->collision > STARVIEW_LIFE)
	{   
        incScore(STARVIEW_BONUS_POINTS);
		spawnMediumMultExplosions(object->pos);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(/*   object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||*/  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    //Horizontal movement
    object->pos.x += objAttributes->dirX;

    if( object->pos.x > GM_SCREEN_W - object->pos.w
        || object->pos.x <= 0)
    {
         objAttributes->dirX *= -1.0;
    }

    //Vertical oscillation
    objAttributes->fy += objAttributes->dirY;
    object->pos.y = objAttributes->fy;
    if( object->pos.y > SHOOT_POSITION_Y + OSC_AMPLITUDE
        || object->pos.y < SHOOT_POSITION_Y)
    {
         objAttributes->dirY *= -1.0;
    }


    //Laser ring shooting
    objAttributes->shootDelay--;
    if(objAttributes->shootDelay <= 0)
    {
        objAttributes->shootDelay = STARVIEW_SHOOT_DELAY;
        SDL_Rect ringPos;
        ringPos.x = object->pos.x - 48;
        ringPos.y = object->pos.y + 20;
        spawnLaserRing(ringPos);
    }

    return ;
}

void newStarView( GameObject  *object)
{
	object->func = &starView;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(starViewAttributes) );
    //
    starViewAttributes *objAttributes = (starViewAttributes *)object->attributes;
    objAttributes->shootDelay = STARVIEW_SHOOT_DELAY;
    objAttributes->fx = object->pos.x;
    objAttributes->fy = object->pos.y;
    objAttributes->dirX = STAR_VIEW_X_SPEED;
    objAttributes->dirY = STAR_VIEW_Y_SPEED;

	return;
}

/********************************************/

void spawnStarView(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {  STARVIEW_BASE_FRAME
                              , STARVIEW_ANIM_FRAMES
                              , 0 
                              , STARVIEW_FRAME_DELAY
                              , 0
                              , ANIMATION_REPEAT};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 3 , 4 , 26 , 28};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_32X32_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newStarView(newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************
    laserRing
*********************************************/

static SDL_Rect laserRingColBoxes[MAX_LASER_RING_EXPANSIONS]= 
{
     { 53 , 4 , 22 , 9}
    ,{ 39 , 4 , 49 , 9}
    ,{ 26 , 4 , 76 , 13}
};

/********************************************/

void laserRing( GameObject *object)
{

    laserRingAttributes *objAttributes = (laserRingAttributes *)object->attributes;

	//Delete if it is outside the screen
	if(/*    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||*/  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    object->pos.y++;

    if( objAttributes->expansion
        >= MAX_LASER_RING_EXPANSIONS)
    {
        return;
    }
   
    objAttributes->delayCounter--;
    if(objAttributes->delayCounter <= 0)
    {
        objAttributes->delayCounter = LASER_RING_EXPANSION_DELAY;
        object->colBox = laserRingColBoxes[objAttributes->expansion];
        objAttributes->expansion++;
    }


    return ;
}

/********************************************/

void newLaserRing( GameObject  *object)
{
	object->func = &laserRing;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(laserRingAttributes) );
    //
    laserRingAttributes *objAttributes = (laserRingAttributes *)object->attributes;

    objAttributes->delayCounter =  0;
    objAttributes->expansion = 0;

	return;
}

/********************************************/

void spawnLaserRing(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {  LASERRING_BASE_FRAME
                              , LASERRING_ANIM_FRAMES
                              , 0 
                              , LASERRING_FRAME_DELAY
                              , 0
                              , 0};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &laserRingColBoxes[0]
					 , ENEMIES_128X128_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newLaserRing(newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************
    Tirtas:
*********************************************/

void tirtas( GameObject *object)
{

    tirtasAttributes *objAttributes = (tirtasAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(TIRTAS_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(/*  object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||*/ object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}

/********************************************/

trajectStep * tirtas_trajectories_list[] =
{
      tirtasL_trajectory
    , tirtasR_trajectory
};

/********************************************/

void newTirtas( GameObject  *object , int type)
{
	object->func = &tirtas;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(tirtasAttributes) );
    //
    tirtasAttributes *objAttributes = (tirtasAttributes *)object->attributes;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , tirtas_trajectories_list[type] );


	return;
}

/********************************************/

static GmObjectAnim tirtasAnim[] = 
{
    /*tirtas left*/
    {
          TIRTAS_L_BASE_FRAME
        , TIRTAS_L_ANIM_FRAMES
        , 0 
        , TIRTAS_L_FRAME_DELAY
        , 0
        , 0
    },
    /*tirtas Right*/
    {
          TIRTAS_R_BASE_FRAME
        , TIRTAS_R_ANIM_FRAMES
        , 0 
        , TIRTAS_R_FRAME_DELAY
        , 0
        , 0
    }
};

void makeTirtas(SDL_Rect pos , int type)
{
	GameObject *newObjPtr;


	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 0 , 0 , 32 , 32};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_32X32_CHARSET , 0 , 0 , 1 , &tirtasAnim[type]);
		newTirtas( newObjPtr , type );
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************/

void spawnTirtasR(SDL_Rect pos)
{
    makeTirtas(pos , RIGHT_TIRTAS);
    return;
}

/********************************************/

void spawnTirtasL(SDL_Rect pos)
{
    makeTirtas(pos , LEFT_TIRTAS);
    return;
}

/********************************************
    Tirtas:
*********************************************/

void enemyHMissile( GameObject *object)
{
    enum{
             rotationFramesNum = ENEMY_HMISSILE_MAX_PITCH+1
            ,objSpeed = 1
        };

    static const int maxPitch = ENEMY_HMISSILE_MAX_PITCH;
    static int rotationFrames[rotationFramesNum] =
    {
         ENEMY_HMISSILE_BASE_FRAME + 4
        ,ENEMY_HMISSILE_BASE_FRAME + 5
        ,ENEMY_HMISSILE_BASE_FRAME + 6
        ,ENEMY_HMISSILE_BASE_FRAME + 7
        ,ENEMY_HMISSILE_BASE_FRAME + 8
        ,ENEMY_HMISSILE_BASE_FRAME + 9
        ,ENEMY_HMISSILE_BASE_FRAME + 10
        ,ENEMY_HMISSILE_BASE_FRAME + 11
        ,ENEMY_HMISSILE_BASE_FRAME + 12
        ,ENEMY_HMISSILE_BASE_FRAME + 13
        ,ENEMY_HMISSILE_BASE_FRAME + 14
        ,ENEMY_HMISSILE_BASE_FRAME + 15
        ,ENEMY_HMISSILE_BASE_FRAME + 0
        ,ENEMY_HMISSILE_BASE_FRAME + 1
        ,ENEMY_HMISSILE_BASE_FRAME + 2
        ,ENEMY_HMISSILE_BASE_FRAME + 3
    };

    static vector rotationVectors[rotationFramesNum] =
    {
        {1.00,0.00},
        {0.92,0.38},
        {0.71,0.71},
        {0.38,0.92},
        {-0.00,1.00},
        {-0.38,0.92},
        {-0.71,0.71},
        {-0.92,0.38},
        {-1.00,-0.00},
        {-0.92,-0.38},
        {-0.71,-0.71},
        {-0.38,-0.92},
        {0.00,-1.00},
        {0.38,-0.92},
        {0.71,-0.71},
        {0.92,-0.38},
    };

    enemyHMissileAttributes *objAttributes; 
    objAttributes = (enemyHMissileAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision > 0)
	{   
		spawnSmallExplosion_1( object->pos);
        incScore(ENEMY_HMISSILE_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    vector thrust;
    thrust.x = (rotationVectors + objAttributes->pitch)->x * objSpeed;
    thrust.y = (rotationVectors + objAttributes->pitch)->y * objSpeed;

    objAttributes->pitchChangeDelay--;
    if(objAttributes->pitchChangeDelay <= 0)
    {
        objAttributes->pitchChangeDelay = ENEMY_HMISSILE_PITCH_DELAY;
        //
        SDL_Rect playerPos = getPlayerPosition();
        vector vectorToPlayer;
        //
        vectorToPlayer.x = playerPos.x - object->pos.x;
        vectorToPlayer.y = playerPos.y - object->pos.y;
        //Cross product of thrust and vectorToPlayer vectors
        float xProduct =  thrust.x * vectorToPlayer.y 
                        - thrust.y * vectorToPlayer.x;
        //
        if( xProduct >= 0)
            objAttributes->pitch++;
        else
            objAttributes->pitch--;

        objAttributes->pitch %= maxPitch+1;

        /* Change missile sprite and thrust direction*/
        object->tile = rotationFrames[objAttributes->pitch];
        thrust.x = (rotationVectors + objAttributes->pitch)->x * objSpeed;
        thrust.y = (rotationVectors + objAttributes->pitch)->y * objSpeed;

        SDL_Rect trailPos;
        trailPos.x = object->pos.x + object->pos.w/2 - (object->pos.w/2)*thrust.x;
        trailPos.y = object->pos.y + object->pos.w/2 - (object->pos.w/2)*thrust.y;
        spawnMissileTrail(trailPos);

    }

    objAttributes->fpos.x += thrust.x;
    objAttributes->fpos.y += thrust.y;
    object->pos.x = objAttributes->fpos.x;
    object->pos.y = objAttributes->fpos.y;

	return;
}

/********************************************/

void newEnemyHMissile( GameObject  *object , int pitch)
{
	object->func = &enemyHMissile;
    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(enemyHMissileAttributes) );
    enemyHMissileAttributes *objAttributes;
    objAttributes = (enemyHMissileAttributes *)object->attributes;
    //
    objAttributes->pitch = pitch % (ENEMY_HMISSILE_MAX_PITCH+1);
    objAttributes->pitchChangeDelay = ENEMY_HMISSILE_PITCH_DELAY;
    objAttributes->fpos.x = object->pos.x;
    objAttributes->fpos.y = object->pos.y;

	return;
}

/********************************************/

void makeEnemyHMissile(SDL_Rect pos , int pitch)
{
	GameObject *newObjPtr;

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 16;
		pos.h = 16;
		SDL_Rect colBox = { 4 , 4 , 8 , 8};

	    GmObjectAnim enemyAnim = {  ENEMY_HMISSILE_BASE_FRAME+8
                                   , 0 , 0 , 0 , 0 , 0 };

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_16X16_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newEnemyHMissile( newObjPtr , pitch);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************/

void spawnEnemyHMissile(SDL_Rect pos)
{
    int i;
    for(i = 0 ; i < ENEMY_HMISSILE_MAX_PITCH+1 ; i++)
    {
        makeEnemyHMissile(pos , i);
    }
	return;
}

/********************************************
    tekkerz
    power suit with missile launcher
*********************************************/


void tekkerz( GameObject *object)
{
     enum {
             SHOOT_POSITION_Y = 32
            ,OSC_AMPLITUDE = 10
          };

     starViewAttributes *objAttributes = (starViewAttributes *)object->attributes;

    //Entrance
    if( object->pos.y < SHOOT_POSITION_Y)
    {
        objAttributes->fy++;
        object->pos.y = objAttributes->fy;
        return;
    }

    //Handle collision damage
	if( object->collision > TEKKERZ_LIFE)
	{   
        incScore(TEKKERZ_BONUS_POINTS);
		spawnMediumMultExplosions(object->pos);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(/*   object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||*/  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    //Horizontal movement
    object->pos.x += objAttributes->dirX;

    if( object->pos.x > GM_SCREEN_W - object->pos.w
        || object->pos.x <= 0)
    {
         objAttributes->dirX *= -1.0;
    }

    //Vertical oscillation
    objAttributes->fy += objAttributes->dirY;
    object->pos.y = objAttributes->fy;
    if( object->pos.y > SHOOT_POSITION_Y + OSC_AMPLITUDE
        || object->pos.y < SHOOT_POSITION_Y)
    {
         objAttributes->dirY *= -1.0;
    }


    //Shoot homing missiles
    objAttributes->shootDelay--;
    if(objAttributes->shootDelay <= 0)
    {
        objAttributes->shootDelay = TEKKERZ_SHOOT_DELAY;
        SDL_Rect missilePos;
        missilePos.x = object->pos.x + 5;
        missilePos.y = object->pos.y + object->pos.h / 2;
        makeEnemyHMissile(missilePos , 5);
        makeEnemyHMissile(missilePos , 6);
        makeEnemyHMissile(missilePos , 7);
        makeEnemyHMissile(missilePos , 8);
        makeEnemyHMissile(missilePos , 9);
        makeEnemyHMissile(missilePos , 10);
        makeEnemyHMissile(missilePos , 11);


        missilePos.x = object->pos.x + object->pos.w + 5;
        missilePos.y = object->pos.y + object->pos.h / 2;

        makeEnemyHMissile(missilePos , 13);
        makeEnemyHMissile(missilePos , 14);
        makeEnemyHMissile(missilePos , 0);
        makeEnemyHMissile(missilePos , 1);
        makeEnemyHMissile(missilePos , 2);
        makeEnemyHMissile(missilePos , 3);
    }

    return ;
}

void newTekkerz( GameObject  *object)
{
	object->func = &tekkerz;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(tekkerzAttributes) );
    //
    tekkerzAttributes *objAttributes = (tekkerzAttributes *)object->attributes;
    objAttributes->shootDelay = 0;
    objAttributes->fx = object->pos.x;
    objAttributes->fy = object->pos.y;
    objAttributes->dirX = TEKKERZ_X_SPEED;
    objAttributes->dirY = TEKKERZ_Y_SPEED;

	return;
}

/********************************************/

void spawnTekkerz(SDL_Rect pos)
{
	GameObject *newObjPtr;
	GmObjectAnim enemyAnim = {  TEKKERZ_BASE_FRAME
                              , TEKKERZ_ANIM_FRAMES
                              , 0 
                              , TEKKERZ_FRAME_DELAY
                              , 0
                              , ANIMATION_REPEAT};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 13, 19 , 35 , 28};

		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_64X64_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newTekkerz(newObjPtr);
#ifdef DEBUG
		printfGameObjectArray(stdin , "--New&Pull--\n" , &OAM_enemies);
#endif
	}
	return;
}

/********************************************
    Maraac
*********************************************/

GmObjectAnim maraacAnim = 
    {
        MARAAC_BASE_FRAME
      , MARAAC_ANIM_FRAMES
      , 0 
      , MARAAC_FRAME_DELAY
      , 0
      , 0
    };

void maraac( GameObject *object)
{
    maraacAttributes *objAttributes = (maraacAttributes *)object->attributes;

    //Manage collision damage
    if(object->collision > MARAAC_LIFE )
	{
        spawnMediumMultExplosions( object->pos);
        incScore(MARAAC_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);
		return;
	}
  
	//Delete if it is outside the screen
	if(/*  object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||  object->pos.y > GM_SCREEN_H + object->pos.h
	   ||*/  object->pos.y < -object->pos.h)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );
    if( trajectoryEnded(&(objAttributes->trajectory)) )
    {
        initTrajectory(  &(objAttributes->trajectory) 
                       , &(object->pos)
                       , maraac_trajectory);
        setObjectAnimation( object , & maraacAnim);
        SDL_Rect impulsePos;
        impulsePos.x = object->pos.x +16;
        impulsePos.y = object->pos.y +29;
        spawnImpulseBurst(impulsePos);
    }

    return;
}

//------------------------------------------------

void newMaraac( GameObject *object)
{
    object->func = &maraac;
	object->attributes = malloc(sizeof(maraacAttributes) );

    maraacAttributes *objAttributes = (maraacAttributes *)object->attributes;
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , maraac_trajectory);
    return;
}

//------------------------------------------------

void spawnMaraac(SDL_Rect pos)
{
	GameObject *newObjPtr;

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 19 , 4 , 26, 54};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_64X64_CHARSET , maraacAnim.base , 0 , 1 , &maraacAnim);
		newMaraac( newObjPtr);
    }
}

//------------------------------------------------

/********************************************/

/********************************************
    nakserPart
    Nakser tail part
*********************************************/

#define NAKSER_START_POINT_Y 100
#define NAKESER_ENTRY_SPEED 2

void nakserPart( GameObject *object)
{
    nakserPartAttributes *objAttributes = (nakserPartAttributes *)object->attributes;
    if(objAttributes->trajectStart == 0)
    {
        if(object->pos.y < NAKSER_START_POINT_Y)
        {
            object->pos.y+=NAKESER_ENTRY_SPEED;
            return;
        }
        objAttributes->trajectStart = 1;
        //Reinit trayectory
        initTrajectory(  &(objAttributes->trajectory) 
                       , &(object->pos)
                       , nakser_trajectory );
    }

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}

/********************************************/

void newNakserPart( GameObject  *object)
{
	object->func = &nakserPart;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(nakserPartAttributes) );
    //
    nakserPartAttributes *objAttributes = (nakserPartAttributes *)object->attributes;
    //
    objAttributes->trajectStart = 0;
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , stomLU_trajectory );


	return;
}

/********************************************/

GameObject *makeNakserPart(SDL_Rect pos)
{
	GameObject *newObjPtr;

	GmObjectAnim enemyAnim = {  NAKSER_PART_BASE_FRAME
                              , NAKSER_PART_ANIM_FRAMES
                              , 0 
                              , NAKSER_PART_FRAME_DELAY
                              , 0
                              , 0 /*ANIMATION_REPEAT*/};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 25 , 25 , 20 , 20};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_64X64_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newNakserPart(newObjPtr);
	}
    return newObjPtr;
}



/********************************************/

LList_t makeNakserTail(SDL_Rect pos)
{
    LList_t tailList = newLinkedList();
    enum
    {
         TAIL_PARTS = 25
        ,PART_HEIGHT = 16
    };
    int i;
    SDL_Rect partPos = pos;
    //partPos.x += 16 ; //Padding from the nakser
                        //Main body
    partPos.y += -PART_HEIGHT/2;
    for( i = 0 ; i < TAIL_PARTS ; i++)
    {
        partPos.y -= PART_HEIGHT;
        addLLNode( &tailList , makeNakserPart(partPos));
    }
    return  tailList;
}

/********************************************/

void destroyNakserTailPart(void *objData/*GameObject *nakserObj*/)
{
    GameObject *object = (GameObject *)objData;
    spawnMediumMultExplosions( object->pos);
    freeANDpushObj( &OAM_enemies , object);  
    return;
}

/********************************************/

void destroyNakserTail(GameObject *nakserObj)
{
    nakserAttributes *objAttributes = (nakserAttributes *)nakserObj->attributes;
    applyToList( &(objAttributes->tailList), &destroyNakserTailPart);
    freeLList(&(objAttributes->tailList));
    return;
}

/********************************************
    nakser
*********************************************/


void nakser( GameObject *object)
{
    nakserAttributes *objAttributes = (nakserAttributes *)object->attributes;

    if(objAttributes->trajectStart == 0)
    {
        if(object->pos.y < NAKSER_START_POINT_Y)
        {
            object->pos.y+=NAKESER_ENTRY_SPEED;
            return;
        }
        objAttributes->trajectStart = 1;
        object->collision = 0;
        //Reinit trayectory
        initTrajectory(  &(objAttributes->trajectory) 
                       , &(object->pos)
                       , nakser_trajectory );
    }

    if(object->collision > NAKSER_LIFE )
    {
        spawnMediumMultExplosions( object->pos);
        destroyNakserTail(object);
        incScore(NAKSER_BONUS_POINTS);
		freeANDpushObj( &OAM_enemies , object);  
		return;
    }

    updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );

	return;
}

/*******************************************/

void newNakser( GameObject  *object)
{
	object->func = &nakser;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(nakserAttributes) );
    //
    nakserAttributes *objAttributes = (nakserAttributes *)object->attributes;
    //
    objAttributes->trajectStart = 0;
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , stomLU_trajectory );
    //Create enemy tail
    objAttributes->tailList = makeNakserTail(object->pos);

	return;
}

/********************************************/

void spawnNakser(SDL_Rect pos)
{
	GameObject *newObjPtr;

	GmObjectAnim enemyAnim = {  NAKSER_BASE_FRAME
                              , NAKSER_ANIM_FRAMES
                              , 0 
                              , NAKSER_FRAME_DELAY
                              , 0
                              , 0 /*ANIMATION_REPEAT*/};

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 64;
		pos.h = 64;
		SDL_Rect colBox = { 12 , 11 , 40 , 40};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					 , ENEMIES_64X64_CHARSET , 0 , 0 , 1 , &enemyAnim);
		newNakser(newObjPtr);
	}
    return;
}

//-------------------------------------------

/********************************************
    bKnight
    Beam knight
*********************************************/

enum bKnightTypes
{
    LEFT_BKNIGHT = 0
   ,RIGHT_BKNIGHT
   ,BKNIGHT_TYPES   //Number of bKnight types
};

/********************************************/

void bKnight( GameObject *object)
{
    enum { ENTRANCE_S = 0 , BEAM_SHOOT_S , EXIT_S };
    enum { SHOOT_FRAMES = 32+4 };
    bKnightAttributes *objAttributes = (bKnightAttributes *)object->attributes;

    //Handle collision damage
	if( object->collision >  BKNIGHT_LIFE)
	{   
        spawnMediumMultExplosions( object->pos);
        incScore(BKNIGHT_BONUS_POINTS);
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

	//Delete if it is outside the screen
	if(/*    object->pos.x > GM_SCREEN_W + object->pos.w
	   ||  object->pos.x < -object->pos.w
	   ||*/  object->pos.y > GM_SCREEN_H + object->pos.h
	   /*||  object->pos.y < -object->pos.h*/)
	{
		freeANDpushObj(&OAM_enemies , object);
		return;
	}

    SDL_Rect beamPos;

    switch(objAttributes->status)
    {
        case ENTRANCE_S:
        case EXIT_S:
            updateTrajectory( &(object->pos) , &(objAttributes->trajectory) );
            //
            if( getStepNumber( &(objAttributes->trajectory) ) 
                == BKNIGHT_SHOOT_STEP)
            {
                objAttributes->status = BEAM_SHOOT_S;
                objAttributes->shootCounter = SHOOT_FRAMES;
            }

        break;
        //
        case BEAM_SHOOT_S:

            //Shoot
            if(objAttributes->shootCounter == SHOOT_FRAMES)
            {
                beamPos.y = object->pos.y+8;
                if( objAttributes->type == LEFT_BKNIGHT)
                {
                    beamPos.x = object->pos.x-16;
                    spawnRightBeamShoot(beamPos);
                }
                else
                {
                    beamPos.x = object->pos.x+object->pos.w;
                    spawnLeftBeamShoot(beamPos);
                }
            }

            objAttributes->shootCounter--;
            //Move away
            if(objAttributes->shootCounter == 0)
                objAttributes->status = EXIT_S;
        break;
    }

    return;
}

/********************************************/

static trajectStep * bKnight_trajectories_list[] =
{
    bKnightL_trajectory 
   ,bKnightR_trajectory 
};


void newBKnight( GameObject  *object , int type)
{
	object->func = &bKnight;

    //Allocate and initialize attributes
	object->attributes = malloc( sizeof(bKnightAttributes) );
    //
    bKnightAttributes *objAttributes = (bKnightAttributes *)object->attributes;
    //
    initTrajectory(  &(objAttributes->trajectory) 
                   , &(object->pos)
                   , bKnight_trajectories_list[type] );
    //
    objAttributes->type = type;
    objAttributes->status = 0;

	return;
}

/********************************************/

void makeBKnight(SDL_Rect pos , int type)
{
    static GmObjectAnim enemyAnim[BKNIGHT_TYPES]=
    {
        { BKNIGHT_L_BASE_FRAME , BKNIGHT_L_ANIM_FRAMES , 0 , 0 , 0 , 0 }
       ,{ BKNIGHT_R_BASE_FRAME , BKNIGHT_R_ANIM_FRAMES , 0 , 0 , 0 , 0 }
    };

	GameObject *newObjPtr;

	if( searchEmptyObject( &OAM_enemies) >= 0)
	{
		pos.w = 32;
		pos.h = 32;
		SDL_Rect colBox = { 4 , 10 , 23 , 13};
		newObjPtr = newObjANDpull( &OAM_enemies , &pos , &colBox
					  , ENEMIES_32X32_CHARSET , enemyAnim[type].base , 0 , 1 , enemyAnim+type);
		newBKnight( newObjPtr , type);
	}
	return;
}

void spawnBKnight_L(SDL_Rect pos)
{
    makeBKnight(pos,LEFT_BKNIGHT);
    return;
}

void spawnBKnight_R(SDL_Rect pos)
{
    makeBKnight(pos,RIGHT_BKNIGHT);
    return;
}

