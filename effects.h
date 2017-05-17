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
 effects.h:
 Special effects
*********************************************/

#ifndef EFFECTS_H
#define EFFECTS_H

/********************************************/

void freeEffect( GameObject *object);

/********************************************/

void stdExplosion( GameObject *object);
void newStdExplosion( GameObject *object);

/********************************************/

void spawnStdBulletExplosion( SDL_Rect pos);

/********************************************/

void newStdEnemyExplosion( GameObject *object);
void spawnSmallExplosion_1( SDL_Rect pos);
void spawnMediumExplosion_1( SDL_Rect pos);
void spawnMediumExplosion_2( SDL_Rect pos); //Ring explosion

/********************************************/

void smallTileExplosion( GameObject *object);
void newSmallTileExplosion( GameObject *object , SDL_Rect *tilePos);
void makeSmallTileExplosion( SDL_Rect *pos);

/********************************************/

void shipBurstWave( GameObject *object);
void newShipBurstWave( GameObject *object);
void spawnShipBurstWave(SDL_Rect pos);
void spawnShipBurst(SDL_Rect pos);
void generateShipBurst( SDL_Rect pos);

/********************************************/

void shipTrail( GameObject *object);
void newShipTrail( GameObject *object);
void spawnShipTrail( SDL_Rect pos);

/********************************************/

void shipBurst( GameObject *object);
void newShipBurst( GameObject *object);

/********************************************/

void haltSpawnEvent( GameObject *object);
void newHaltSpawnEvent( GameObject *object);
void spawnHaltSpawnEvent( SDL_Rect pos);

void spawnWarningMsg( SDL_Rect pos);
void spawnBossWarningMsg( SDL_Rect pos);

/********************************************/

void energyShootCharge( GameObject *object);
void newEnergyShootCharge( GameObject *object);
GameObject *generateEnergyShootCharge( SDL_Rect pos);

/********************************************/

void multExplosions( GameObject *object);

/********************************************/

void newMediumMultExplosions( GameObject *object);
void spawnMediumMultExplosions( SDL_Rect pos);

/********************************************/

void newBigMultExplosions( GameObject *object);
void spawnBigMultExplosions( SDL_Rect pos);

/********************************************/

void propulsionBurst( GameObject *object);
void freePropulsionBurst(GameObject *object);
void newPropulsionBurst( GameObject *object);
//
GameObject *newBoss1LeftBurst(SDL_Rect pos);
GameObject *newBoss1RightBurst(SDL_Rect pos);

/********************************************/

void spawnMissileTrail( SDL_Rect pos);

/********************************************/

void spawnStageBossExplosion( SDL_Rect pos);

/********************************************/

void  spawnImpulseBurst( SDL_Rect pos);

/********************************************/

void smallShipStarExplosion( GameObject *object);
void newSmallShipStarExplosion( GameObject *object);
void spawnSmallShipStarExplosion( SDL_Rect pos);
void spawnPlayerMultExplosion( SDL_Rect pos);
void spawnPlayerDeathExplosion( SDL_Rect pos);

/********************************************/


#endif
