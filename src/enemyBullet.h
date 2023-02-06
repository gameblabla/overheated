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
    enemyBullet.h
*********************************************/

#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

void stdEnemyBullet( GameObject *object);
void newStdEnemyBullet( GameObject  *object , double xdir , double ydir);
//
void makeSmallEnemyBullet( SDL_Rect pos , double xdir , double ydir);
//
void makeGatlingBullet( SDL_Rect *pos , double xdir , double ydir);
void spawnGatlingBulletsBurst(SDL_Rect pos);
//
void newEnemyBlastBullet( GameObject *object , float xdir , float ydir , int size);
void makeEnemyBlastBullet( SDL_Rect *pos , float xdir , float ydir , int size);
//
void makeSmallEnemyBlastBullet( SDL_Rect *pos , float xdir , float ydir);
void makeMediumEnemyBlastBullet( SDL_Rect *pos , float xdir , float ydir);
//void spawnSmallEnergyBall( SDL_Rect *pos);
void spawnEnergyShoot( SDL_Rect pos);
//
void spawnBeamChunk( SDL_Rect pos);
void spawnRightBeamHead( SDL_Rect pos);
void spawnLeftBeamHead( SDL_Rect pos);
void spawnLeftBeamShoot( SDL_Rect pos);
void spawnRightBeamShoot( SDL_Rect pos);
//
#endif

