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
    weapon.c
*******************************************/

#ifndef WEAPON_C
#define WEAPON_C
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
#include "weapon.h"
#include "l_PLweapons.h"
#include "globalVars.h"
#include "bullets.h"
//
#include "debug.h"
//

//Weapons function list
void *weaponsFunc[] ={ &nullShoot , &multShoot };

/*******************************************
    setWeapon
*******************************************/

void setWeapon( PLweapon *weapon , int type , int level)
{
	*weapon = *(PLweaponsList+type);

	if(level > weapon->maxLevel)
		weapon->level = weapon->maxLevel;
	else
		weapon->level = level;

	weapon->rounds = weapon->roundsPerRefill;

	return;
}

/*******************************************
    levelUpWeapon
*******************************************/

int levelUpWeapon( PLweapon *weapon)
{
	if(weapon->level < weapon->maxLevel)
		weapon->level++;

	return weapon->level;
}

//-------------------------------------------

/*******************************************
    weaponAtMaxLevel
    Returns:
    1 if the weapon is already at max level
    0 otherwise
*******************************************/

int weaponAtMaxLevel( PLweapon *weapon)
{
	return weapon->level == weapon->maxLevel ? 1 : 0;
}


/*******************************************
    levelDownWeapon
*******************************************/

int levelDownWeapon( PLweapon *weapon)
{
    if(weapon->level)
        weapon->level--;

    return weapon->level;
}

//-------------------------------------------

/*******************************************
    getWeaponLevel
*******************************************/
int getWeaponLevel( PLweapon *weapon)
{
    return weapon->level;
}

//-------------------------------------------

/*******************************************
    setWeaponLevel
*******************************************/
void setWeaponLevel(PLweapon *weapon , unsigned level)
{
	if(level > weapon->maxLevel)
		weapon->level = weapon->maxLevel;
	else
		weapon->level = level;
    return;
}

//-------------------------------------------


/*******************************************
    shootWeapon
*******************************************/
void shootWeapon( GameObject *player)
{
    int weaponType = ((playerAttributes *)playerObj->attributes)->weapon.type;
	( *(void (*)(GameObject *))weaponsFunc[weaponType])(player);
	return;
}

//-------------------------------------------

//Null Shoot
void nullShoot(GameObject *player)
{
	return;
}

//Multi-directional shoot
extern bulleTmp multShootLevels[][MAX_BULLETS_PER_ROUND];

void multShoot(GameObject *player)
{
    PLweapon *playerWeapon = &(((playerAttributes *)playerObj->attributes)->weapon);
    int weaponLevel = playerWeapon->level;

	if(playerWeapon->refillCounter > 0)
	{
		playerWeapon->refillCounter--;
		return;
	}

	SDL_Rect pos = player->pos;
	SDL_Rect colBox;
	void *newObjPtr;
	int32_t i= multShootLevels[weaponLevel][0].num-1;
	for( ; i >= 0 ; i--)
	{
		//search bullets object array for a new slot
		if( searchEmptyObject( &OAM_PlBullets) >= 0)
		{
			pos.x = player->pos.x + multShootLevels[weaponLevel][i].paddingX;
			pos.y = player->pos.y + multShootLevels[weaponLevel][i].paddingY;
			colBox = multShootLevels[weaponLevel][i].colBox;
			newObjPtr = newObjANDpull( &OAM_PlBullets , &pos , &colBox , PLAYER_BULLETS_CHARSET
						  , multShootLevels[weaponLevel][i].tile , 0 , 1 , NULL);
			new_stdBullet( newObjPtr , multShootLevels[weaponLevel][i].dirX , multShootLevels[weaponLevel][i].dirY );
		}
		else break;
	}

	playerWeapon->rounds--;
	if( playerWeapon->rounds == 0)
	{
		playerWeapon->rounds = playerWeapon->roundsPerRefill;
		playerWeapon->refillCounter = playerWeapon->refillDelay;
	}

	return;
}
