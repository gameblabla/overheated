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
 player.h
********************************************/

#ifndef PLAYER_H
#define PLAYER_H

//private:
#ifdef PLAYER_C

void managePlayer( controllerStruct *controller);
void playerEntrance(controllerStruct *controller);
void playerDeathSequence(controllerStruct *controller);
void playerDead(controllerStruct *controller);
void playerParting(controllerStruct *controller);

#endif

//public:
void playerLeave(void);
void initPlayer(void);
void killPlayer( GameObject *player);
void playerDudFunc(GameObject *obj);

SDL_Rect getPlayerPosition(void);
int getPlSpeedLevel(void);
void setPlayerLives(int lives);
int getPlayerLives(void);
int playerWeaponAtMaxLevel(void); //Returns:
                                  // 1 if the player weapon is already at max level
void levelUpPlayerWeapon(void);

#ifdef D_INPUT_H
void runPlayer(controllerStruct *controller); // Run player functions
#endif
//
#endif
