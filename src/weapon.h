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
    weapon.h
*******************************************/

struct bulleTmp // bullet template
{
	int num; //Number of bullets in this round
	SDL_Rect colBox;
	int tile;
	int dirX;	//Bullet direction
	int dirY;	//Bullet direction
	int paddingX;	//Position relative to the player
	int paddingY;	//

};

typedef struct bulleTmp bulleTmp;

#define MAX_BULLETS_PER_ROUND 8
#define PL_BULLET_SPEED 3

//-------------------------------------------

void setWeapon( PLweapon *weapon , int type , int level);	//Change weapon and level
int  levelUpWeapon( PLweapon *weapon);				        //Increase weapon level
int  levelDownWeapon( PLweapon *weapon);                    //Decrease weapon level
int  getWeaponLevel( PLweapon *weapon);                     //Return weapon level
void setWeaponLevel(PLweapon *weapon , unsigned level);      //Set weapon level
void nullShoot(GameObject *player);				            //Null shoot
void multShoot(GameObject *player);				            //Multi-directional shoot
void shootWeapon( GameObject *player);				        //Shoot weapon
int weaponAtMaxLevel( PLweapon *weapon);                    //weaponAtMaxLevel
                                                            //Return 1 if the weapon is already at max level
