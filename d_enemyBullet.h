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
   d_enemies.h
*********************************************/

#ifndef D_ENEMYBULLET_H
#define D_ENEMYBULLET_H

//-------------------------------------------

//8x8 Bullets
#define SMALL_ENEMY_BULLET_BASE_FRAME 0
#define SMALL_ENEMY_BULLET_ANIM_FRAMES 2
//16x16 Bullets
#define ENERGY_SHOOT_TOP_BASE_FRAME 9
#define ENERGY_SHOOT_TOP_ANIM_FRAMES 4
#define ENERGY_SHOOT_CHUNK_BASE_FRAME 13
#define ENERGY_SHOOT_CHUNK_ANIM_FRAMES 4
#define GATLING_BULLET_BASE_FRAME 17
#define GATLING_BULLET_ANIM_FRAMES 4
#define GATLING_BULLET_FRAME_DELAY 3
#define BEAM_SHOOT_BASE_FRAME 32
#define BEAM_SHOOT_ANIM_FRAMES 4
#define BEAM_SHOOT_ANIM_DELAY 8
#define L_BEAM_HEAD_BASE_FRAME 24
#define L_BEAM_HEAD_ANIM_FRAMES BEAM_SHOOT_ANIM_FRAMES
#define L_BEAM_HEAD_ANIM_DELAY BEAM_SHOOT_ANIM_DELAY
#define R_BEAM_HEAD_BASE_FRAME 28
#define R_BEAM_HEAD_ANIM_FRAMES BEAM_SHOOT_ANIM_FRAMES
#define R_BEAM_HEAD_ANIM_DELAY BEAM_SHOOT_ANIM_DELAY



//-------------------------------------------
//Standard Enemy Bullet

struct stdEnemyBulletAttributes
{
	double xdir;
	double ydir;
    double posX;
    double posY;

};

typedef struct stdEnemyBulletAttributes stdEnemyBulletAttributes;

//-------------------------------------------
//Enemy blast bullet
/*
    This bullet is part of a bullet OUTBREAK/eruption.

    In a bullet outbreak/eruption multiple bullets are spawned at the same point
    then the bullets move away from the spawn point with a radial trajectory.
    The farther from the spawn point , slower the bullets will become.

    *for each bullet outbreak/eruption , there is an array 
    somewhere that contains the address of all the bullets
    that are part of the outbreak/eruption.
*/

#define BLAST_BULLET_INITAL_VEL 2.0
#define BLAST_BULLET_MIN_VELOCITY 1.0
#define BLAST_BULLET_VEL_DEC_RATE 0.05
#define MAX_DIST_CENTER 300.0

/************************************************/

enum enemyBlastBulletStatusEnum
{ 
      ENEMYBLASTBULLET_DEFAULT_STATUS = 0
    , ENEMYBLASTBULLET_BEGIN_EXPLOSION
    , ENEMYBLASTBULLET_EXPLOSION
};

/************************************************/

enum enemyBlastBulletSizeEnum
{
      ENEMYBLASTBULLET_SMALL = 0
    , ENEMYBLASTBULLET_MEDIUM
    , ENEMYBLASTBULLET_BIG
    , ENEMYBLASTBULLETS_NUM
};

/************************************************/

struct enemyBlastBulletAttributes
{
    int status;  
    int size;

    float xdir;       //X direction
    float ydir;       //Y direction

    float velocity;

    float fx;         //X position
    float fy;         //Y position

    GameObject **listEntry; //This is a pointer to the pointer OF THIS OBJECT
                            //on the OUTBREAK ARRAY
};

typedef struct enemyBlastBulletAttributes enemyBlastBulletAttributes;

//-------------------------------------------
// Energy shoot

#define ENERGY_SHOOT_CHUNK_ANIM_DELAY 3
#define ENERGY_SHOOT_CHUNK_W 16
#define ENERGY_SHOOT_CHUNK_H 16
//
#define ENERGY_SHOOT_TOP_ANIM_DELAY 3

//-------------------------------------------
// Beam shoot

#define BEAM_SHOOT_CHUNK_W 16

#endif
