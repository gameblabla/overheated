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
    player.c
    Player functions
********************************************/


#ifndef PLAYER_C
#define PLAYER_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

//
#include "globalDefines.h"
#include "d_input.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "objects.h"
#include "enemies.h"
#include "d_player.h"
#include "player.h"
#include "weapon.h"
#include "l_PLweapons.h"
#include "globalVars.h"
#include "sfx.h"
#include "l_sfx.h"
#include "spawn.h"
#include "effects.h"

//
#include "debug.h"

//

static unsigned int playerLives = 0;
static int playerSpeedLevel = 1;

GameObjectArray OAM_player;
GameObject *playerObj;
void *playerRunFunction = NULL; //Current function in control of the player

#define PLAYER_START_LINE 140
#define PLAYER_RESPAWN_DELAY 48


/***************************************************
    getPlayerLives
****************************************************/

int getPlayerLives(void)
{
    return playerLives;
}

//-------------------------------------------

/***************************************************
    setPlayerLives
****************************************************/

void setPlayerLives(int lives)
{
    playerLives = lives;
    return;
}

//-------------------------------------------

/***************************************************
    getPlSpeedLevel
    Get the speed player level
****************************************************/

int getPlSpeedLevel(void)
{
    return playerSpeedLevel;
}

//-------------------------------------------

/***************************************************
    getPlayerSpeed:
    Get the speed value assigned to the given level
****************************************************/

#define PL_SPEED_LEVELS 3

static int speedLevels[] =
{
    2, // Lv0
    2, // Lv1
    3, // Lv2
    4, // Lv3
};

int getPlayerSpeed( unsigned speedLevel)
{
    if( speedLevel > PL_SPEED_LEVELS)
        speedLevel = PL_SPEED_LEVELS;

    return speedLevels[speedLevel];
}

//-------------------------------------------

/***************************************************
    getPlayerPosition
****************************************************/

SDL_Rect getPlayerPosition(void)
{
    return playerObj->pos;
}

//-------------------------------------------

/***************************************************
    changePlSpeed
****************************************************/

void incPlSpeedLevel()
{
    if( ++playerSpeedLevel > PL_SPEED_LEVELS)
        playerSpeedLevel = 1;

    return;
}

/***************************************************
   levelUpPlayerWeapon
****************************************************/

void levelUpPlayerWeapon(void)
{
    playerAttributes *plAttributes = playerObj->attributes;
    levelUpWeapon( &plAttributes->weapon);
    return;
}

//---------------------------------------------

/***************************************************
    playerWeaponAtMaxLevel

    Returns:
    1 if the player weapon is already at max level
****************************************************/

int playerWeaponAtMaxLevel(void)
{
    playerAttributes *plAttributes = playerObj->attributes;
    return weaponAtMaxLevel(&plAttributes->weapon);
}

//---------------------------------------------

/***************************************************
    playerDudFunc:
    
    standard object managing functions (from object.c)
    Will call this dud function that does nothing.
****************************************************/

void playerDudFunc(GameObject *obj)
{
    return;
}


//-------------------------------------------

/*******************************************
    playerLeave
    Set the player to playerParting status
********************************************/

void playerLeave(void)
{
    
    playerObj->collision = 0;
    playerObj->power = 0; //Dissable collisions with the player
    playerRunFunction = (void *) &playerParting;    
    return;
}

/*******************************************
    playerParting
    Mission complete! , get out of the screen
********************************************/

void playerParting(controllerStruct *controller)
{
    playerObj->collision = 0; //Clean collision damage
    if( playerObj->pos.y > -playerObj->pos.h*2 )
    {
        SDL_Rect trailPos;
        trailPos.x = playerObj->pos.x +9;
        trailPos.y = playerObj->pos.y +22;
        spawnShipTrail(trailPos);
        playerObj->pos.y -= getPlayerSpeed(3);
        return;
    }

    playerObj->pos.x = GM_SCREEN_W/2 - playerObj->pos.w/2;
    playerObj->pos.y = GM_SCREEN_H*2; /*Out of the visible screen*/
    playerObj->tile = PL_TRANSPARENT_TILE;
    playerRunFunction = (void *) &playerDead;
    
    return;
}

/*******************************************
    playerDead
*******************************************/

void playerDead(controllerStruct *controller)
{
    playerObj->collision = 0; //Clean collision damage
    return;
}

/*******************************************
    playerDeathSequence
*******************************************/

void playerDeathSequence(controllerStruct *controller)
{
    playerAttributes *plAttributes = playerObj->attributes;
    if( plAttributes->respawnCounter <= 0)
    {
        if(playerLives <= 0)
        {
            /*DED, RIP in peace*/
            playerObj->pos.x = GM_SCREEN_W/2 - playerObj->pos.w/2;
            playerObj->pos.y = GM_SCREEN_H*2; /*Out of the visible screen*/
            playerObj->tile = PL_TRANSPARENT_TILE;
            //(...)
            playerRunFunction = (void *) &playerDead;
            return;
        }
        else
        {
            /*Respawn player*/
		    playerObj->pos.x = GM_SCREEN_W / 2 - playerObj->pos.w/2 ;
		    playerObj->pos.y = GM_SCREEN_H - playerObj->pos.h/2;
	        playerObj->tile = PL_STILL_TILE;
            playerRunFunction = (void *) &playerEntrance;
            return;
        }
    }
    plAttributes->respawnCounter--;

    return;
}


/***************************************************
    killplayer:

    Kill the player and reset the player object
****************************************************/

void killPlayer( GameObject *player)
{   
		SDL_Rect ps;
		ps.x = player->pos.x;
        playerAttributes *plAttributes = player->attributes;
        //Spawn explosion effect
        spawnPlayerDeathExplosion(ps);
        if(playerLives > 0) playerLives--;
        //Reset weapon
		setWeapon( &plAttributes->weapon  , 1 , 0);
		player->collision = 0;
        player->power = 0; //Dissable collisions with the player
        plAttributes->respawnCounter = PLAYER_RESPAWN_DELAY;
        player->tile = PL_TRANSPARENT_TILE;
        playerRunFunction = (void *) &playerDeathSequence;
		return;
}

/*******************************************
    runPlayer:
    Run player functions
*******************************************/

void runPlayer(controllerStruct *controller)
{
    if( playerRunFunction != NULL)
    {
        (*(void(*)( controllerStruct *))(playerRunFunction))(controller);
    }
    return;
}

//-------------------------------------------

/*******************************************
    managePlayer
*******************************************/

void managePlayer(controllerStruct *controller)
{
    playerAttributes *plAttributes = playerObj->attributes;
    int playerSpeed = getPlayerSpeed(playerSpeedLevel);

	playerObj->tile = PL_STILL_TILE;

    //Spawn time invincibility period
    if( plAttributes->spawnFrames > 0)
    {
        plAttributes->spawnFrames--;

        playerObj->collision = 0; //Remove collision damage
        plAttributes->invisible = ~plAttributes->invisible; //flickering

        //End spawn period
        if( plAttributes->spawnFrames == 0)
        {
            plAttributes->invisible = 0;  //Make it visible
            playerObj->power = 1;            //Make it solid again
        }
    }

    //Manage collision damage
	if( playerObj->collision != 0)
	{
        if( getWeaponLevel( &plAttributes->weapon) == 0)
        {
		    killPlayer(playerObj);
            return;
        }
        //else
        setWeaponLevel(&plAttributes->weapon , 0);
        playEffect(SFX_POWERDOWN_1);
        plAttributes->spawnFrames = PL_SPAWN_FRAMES;
        playerObj->collision = 0;
	}

    //Manage controller input

    //Vertical movement
	if(controller->raw.UP
        && playerObj->pos.y > 16)
    {
		playerObj->pos.y -= playerSpeed;
    }
	else if(controller->raw.DOWN
            && playerObj->pos.y < GM_SCREEN_H - playerObj->pos.h)
    {
		playerObj-> pos.y += playerSpeed;
    }

    //Horizontal movement
	if( controller->raw.LEFT
       && playerObj->pos.x > 0)
	{
		playerObj->pos.x -= playerSpeed;
		playerObj->tile = PL_LEFT_TILE;
	}
	else if( controller->raw.RIGHT
            && playerObj->pos.x < GM_SCREEN_W 
               - playerObj->pos.w -  PL_SPRITE_RIGHT_PANING)
	{
		playerObj->pos.x += playerSpeed ;
		playerObj->tile = PL_RIGHT_TILE;
	}

    //Shoot
	if(controller->raw.A)
	{
		shootWeapon(playerObj);
	}

    //Change speed
    if(controller->pressed.B)
    {
        incPlSpeedLevel();
        generateShipBurst(playerObj->pos);
    }

    //make the player invisible
    if(plAttributes->invisible)
       playerObj->tile = PL_TRANSPARENT_TILE;

	return;
}

//-------------------------------------------

/*******************************************
    playerEntrance
    Player entrance/spawn sequence.
*******************************************/

#define PLAYER_START_LINE 140

void playerEntrance(controllerStruct *controller)
{
    if( playerObj->pos.y > PLAYER_START_LINE )
    {
        SDL_Rect trailPos;
        trailPos.x = playerObj->pos.x +9;
        trailPos.y = playerObj->pos.y +22;
        spawnShipTrail(trailPos);
        playerObj->pos.y -= getPlayerSpeed(3);
        return;
    }
    generateShipBurst(playerObj->pos);
    ((playerAttributes *)playerObj->attributes)->spawnFrames = PL_SPAWN_FRAMES;
    playerRunFunction = (void *) &managePlayer;
    

    return;
}


/***************************************************
    initPlayer:
    Initialize the player object and OAM
****************************************************/

void initPlayer(void)
{
    freeObjectArray(&OAM_player);
    initObjectArray( &OAM_player , 1);   

	SDL_Rect colBox = { 7 , 8 , 20 , 15 };
    SDL_Rect PLinitPos = { 
                              GM_SCREEN_W / 2 - PL_SHIP_W/2 
                            , GM_SCREEN_H - PL_SHIP_H
                            , PL_SHIP_W 
                            , PL_SHIP_H
                         };

    playerObj = newObjANDpull(   &OAM_player , &PLinitPos , &colBox 
                               , PLAYER_CHARSET , PL_STILL_TILE , 0 , 1 , NULL);

    //
	playerObj->attributes = malloc( sizeof(playerAttributes));
    playerAttributes *plAttributes = playerObj->attributes;

	setWeapon( &plAttributes->weapon  , 1 , 0);
    plAttributes->invisible = 0;
    plAttributes->spawnFrames = 0;
    playerSpeedLevel = 1;

    playerObj->func = &playerDudFunc;
    playerRunFunction = &playerEntrance;


	return;
}

//-------------------------------------------

