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
    d_effects.h
    effects definitions and constants
*********************************************/

#ifndef D_EFFECTS_H
#define D_EFFECTS_H

//-------------------------------------------

//8x8 Effects
#define STD_BULLET_EXPLOSION_BASE_FRAME 0
#define STD_BULLET_EXPLOSION_ANIM_FRAMES 3
#define STD_BULLET_EXPLOSION_ANIM_DELAY 6
#define MISSILE_TRAIL_BASE_FRAME 3
#define MISSILE_TRAIL_ANIM_FRAMES 3
#define MISSILE_TRAIL_ANIM_DELAY 10


//16X16 Effects
#define SMALL_TILE_EXPLOSION_BASE_FRAME 0
#define SMALL_TILE_EXPLOSION_ANIM_FRAMES 5
#define SMALL_TILE_EXPLOSION_ANIM_DELAY 4
#define SHIP_TRAIL_BASE_FRAME 5
#define SHIP_TRAIL_ANIM_FRAMES 3
#define SHIP_TRAIL_ANIM_DELAY 1

//32x32 Effects
#define MEDIUM_EXPLOSION_BASE_FRAME 22
#define MEDIUM_EXPLOSION_ANIM_FRAMES 4
#define MEDIUM_EXPLOSION_ANIM_DELAY 5
#define MEDIUM_EXPLOSION_2_BASE_FRAME 26
#define MEDIUM_EXPLOSION_2_ANIM_FRAMES 4
#define MEDIUM_EXPLOSION_2_ANIM_DELAY 5
#define SHIP_BURST_BASE_FRAME 0
#define SHIP_BURST_ANIM_FRAMES 4
#define SHIP_BURST_ANIM_DELAY 3

//64x64 Effects
#define BOSS1_LEFT_BURST_BASE_FRAME 0
#define BOSS1_BURST_ANIM_FRAMES 2
#define BOSS1_BURST_ANIM_DELAY 2
#define BOSS1_RIGHT_BURST_BASE_FRAME 2
#define SMALL_SHIP_STAR_EXP_BASE_FRAME 4
#define SMALL_SHIP_STAR_EXP_ANIM_FRAMES 8 
#define SMALL_SHIP_STAR_EXP_ANIM_DELAY 2

//---------------------------------------------

#define SHIP_BURST_WAVE_LIFE_TIME 2

struct shipBurstWaveAttributes
{
    int lifeTimeCounter;
};

typedef struct shipBurstWaveAttributes shipBurstWaveAttributes;

//---------------------------------------------

#define SMALL_EXPLOSION1_END_TILE 4
#define PL_BURST_X_PADDING 1
#define PL_BURST_Y_PADDING 20

struct shipBurstAttributes
{

};

typedef struct shipBurstAttributes shipBurstAttributes;

//---------------------------------------------

#define SHIP_TRAIL_LIFE_TIME 20

struct shipTrailAttributes
{
    int lifeTime;
};

typedef struct shipTrailAttributes shipTrailAttributes;


//---------------------------------------------

struct smallTileExplosionAttributes
{
    SDL_Rect *tilePos;
};

typedef struct smallTileExplosionAttributes smallTileExplosionAttributes;

//---------------------------------------------

#define MEDIUM_MULT_EXPLOSIONS_WAVES 3
#define MEDIUM_MULT_EXPLOSIONS_DELAY 6
//---------------------------------------------

#define BIG_MULT_EXPLOSIONS_WAVES 5
#define BIG_MULT_EXPLOSIONS_DELAY 6
//---------------------------------------------

#define LIST_NUL_CH 0xFFF

struct explosionTmp     //Explosion template
{
    void *spawnFunc;    //Explosion spawn/constructor function
    SDL_Rect pos;       //Explosion position
};
typedef struct explosionTmp explosionTmp;
//
enum { explosionsListSize = 20 };
typedef explosionTmp explosionsList[explosionsListSize];

struct multExplosionsAttributes
{
    SDL_Rect basePos;
    int wavesNum;        //Number of explosions waves
    int cWave;           //Current explosions wave
    int waveDelay;       //Delay Between explosions waves
    int delayCounter;    
    explosionsList *waves;
};

typedef struct multExplosionsAttributes multExplosionsAttributes;

//---------------------------------------------


#endif
