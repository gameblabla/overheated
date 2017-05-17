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

#ifndef D_ENEMIES_H
#define D_ENEMIES_H

//-------------------------------------------

//16x16 Enemies
#define SPITZE_BASE_FRAME 0
#define SPITZE_ANIM_FRAMES 3
#define SPITZE_FRAME_DELAY 7
#define ZORGEN_BASE_FRAME 7
#define GAUCER_BASE_FRAME 4 
#define GAUCER_ANIM_FRAMES 3
#define ZORGEN_LEFT_FRAME 8
#define ZORGEN_RIGHT_FRAME 9
#define ENEMY_MISSILE_BASE_FRAME 10
#define PETRUS_BASE_FRAME 12
#define STOM_BASE_FRAME 18
#define STOM_ANIM_FRAMES 0
#define ENEMY_HMISSILE_BASE_FRAME 19
#define ARCHER_BASE_FRAME 36
#define ARCHER_ANIM_FRAMES 5
#define ARCHER_FRAME_DELAY 12
//32x32 Enemies
#define LOVAC_BASE_FRAME 0
#define LOVAC_ANIM_FRAMES 2
#define FARBEN_BASE_FRAME 0
#define FARBEN_ANIM_FRAMES 2
#define LU_MIDBOSS_P_BASE_FRAME 2
#define LU_MIDBOSS_P_ANIM_FRAMES 0
#define RU_MIDBOSS_P_BASE_FRAME 3
#define RU_MIDBOSS_P_ANIM_FRAMES 0
#define LD_MIDBOSS_P_BASE_FRAME 4
#define LD_MIDBOSS_P_ANIM_FRAMES 0
#define RD_MIDBOSS_P_BASE_FRAME 5
#define RD_MIDBOSS_P_ANIM_FRAMES 0
#define STARVIEW_BASE_FRAME 6
#define STARVIEW_ANIM_FRAMES 3
#define STARVIEW_FRAME_DELAY 2
#define TIRTAS_L_BASE_FRAME 9
#define TIRTAS_L_ANIM_FRAMES 3
#define TIRTAS_L_FRAME_DELAY 45
#define TIRTAS_R_BASE_FRAME 12
#define TIRTAS_R_ANIM_FRAMES 3
#define TIRTAS_R_FRAME_DELAY 45
#define BKNIGHT_L_BASE_FRAME 17
#define BKNIGHT_L_ANIM_FRAMES 0
#define BKNIGHT_R_BASE_FRAME 15
#define BKNIGHT_R_ANIM_FRAMES 0
//64x64 Enemies
#define LITEC_BASE_FRAME 0
#define LITEC_ANIM_FRAMES 0
#define MIDBOSS_BASE_FRAME 2
#define MIDBOSS_ANIM_FRAMES 3
#define GODRILL_BASE_FRAME 6
#define GODRILL_ANIM_FRAMES 2 //WIP , 3 frames in the final ver
#define GODRILL_FRAME_DELAY 6
#define TEKKERZ_BASE_FRAME 9
#define TEKKERZ_ANIM_FRAMES 0
#define TEKKERZ_FRAME_DELAY 0
#define LAROC_BASE_FRAME 11
#define LAROC_ANIM_FRAMES 2
#define LAROC_FRAME_DELAY 3
#define MARAAC_BASE_FRAME 13
#define MARAAC_ANIM_FRAMES 3
#define MARAAC_FRAME_DELAY 16
#define NAKSER_BASE_FRAME 16
#define NAKSER_ANIM_FRAMES 0
#define NAKSER_FRAME_DELAY 0
#define NAKSER_PART_BASE_FRAME 17
#define NAKSER_PART_ANIM_FRAMES 0
#define NAKSER_PART_FRAME_DELAY 0
//80x80 Enemies
#define ANANCITES_ST_BASE_FRAME 0
#define ANANCITES_ST_ANIM_FRAMES 2
#define ANANCITES_ST_FRAME_DELAY 2
//128x128 Enemies
#define LASERRING_BASE_FRAME 3
#define LASERRING_ANIM_FRAMES 3
#define LASERRING_FRAME_DELAY 50
#define BOSS2_BASE_FRAME 6
#define BOSS2_ANIM_FRAMES 0
#define BOSS2_FRAME_DELAY 0

//
#define SPITZE_BONUS_POINTS 100
#define GAUCER_BONUS_POINTS 100
#define LOVAC_BONUS_POINTS  100
#define ZORGEN_BONUS_POINTS 100
#define LITEC_BONUS_POINTS  100
#define LAROC_BONUS_POINTS  100
#define ENEMY_MISSILE_BONUS_POINTS 10
#define FARBEN_BONUS_POINTS 100
#define PETRUS_BONUS_POINTS 100
#define JUMPA_BONUS_POINTS  100
#define STOM_BONUS_POINTS   100
#define ANANCITES_ST_BONUS_POINTS 100
#define GODRILL_BONUS_POINTS 100
#define STARVIEW_BONUS_POINTS 100
#define TIRTAS_BONUS_POINTS 100
#define ENEMY_HMISSILE_BONUS_POINTS 10
#define TEKKERZ_BONUS_POINTS 100
#define ARCHER_BONUS_POINTS 100
#define MARAAC_BONUS_POINTS 100
#define NAKSER_BONUS_POINTS 100
#define BKNIGHT_BONUS_POINTS 100
#define MID_BOSS_BONUS_POINTS 2500
#define BOSS1_BONUS_POINTS 5000
#define BOSS2_BONUS_POINTS 5000


//-------------------------------------------

//Spitze types

#define STD_SPITZE 0
#define VTSPITZE_R 1 
#define VTSPITZE_L 2 

struct spitzeAttributes
{
    s_trajectory trajectory;
};

typedef struct spitzeAttributes spitzeAttributes;

//-------------------------------------------

#define GAUCER_BURST_RADIUS 5
#define GAUCER_ROUND_BULLETS 4
#define GAUCER_ROUNDS_NUM 2
#define GAUCER_SHOOT_DELAY 30
#define GAUCER_LIFE 5
#define GAUCER_SHOOT_STEP 1

struct gaucerAttributes
{
    s_trajectory trajectory;

	int roundType;	//Type of bullet round selected
	int shootDelay; //Delay between bullet rounds;
};

typedef struct gaucerAttributes gaucerAttributes;

//-------------------------------------------

//lovac states:

#define LOVAC_CHASE    0 //Chase the player!
#define LOVAC_MOVLEFT  1 //Advance to the left
#define LOVAC_MOVRIGHT 2 //Advance to the right

//

#define LOVAC_XVEL 2
#define LOVAC_YVEL 3

struct lovacAttributes
{
	unsigned state;
	unsigned xvel;	//X velocity
	unsigned yvel;	//Y velocity
};

typedef struct lovacAttributes lovacAttributes;

//-------------------------------------------

//zorgen:

//
#define ZORGEN_XVEL 3
#define ZORGEN_YVEL 3

//states:
#define ZORGEN_MOVE_DOWN 0
#define ZORGEN_CHASE 1


//

struct zorgenAttributes
{
	unsigned state;
	unsigned xvel;	//X velocity
	unsigned yvel;	//Y velocity
};

typedef struct zorgenAttributes zorgenAttributes;

//-------------------------------------------

//litec

//
#define LITEC_LIFE 20
#define LITEC_XMOV_SPEED 1
#define LITEC_YMOV_SPEED 3
#define LITEC_SHOOTING_YPOS 40
#define LITEC_MISSILES_NUM 14
#define LITEC_MISSILE_DELAY 12


//

struct litecAttributes
{
	unsigned missiles;		//Number of missiles
	unsigned missileDelay;	//Delay between missiles
};

typedef struct litecAttributes litecAttributes;

//-------------------------------------------

//enemyMissile

#define ENEMY_MISSILE_SPEED 4
#define ENEMY_MISSILE_LIFE 1

//-------------------------------------------

//farben

#define FARBEN_TRAJECT_AMP 50 //Farben trajectory amplitude
#define TYPE_FARBEN_L 0
#define TYPE_FARBEN_R 1

struct farbenAttributes
{
    s_trajectory trajectory;
};

typedef struct farbenAttributes farbenAttributes;

//-------------------------------------------

//petrus

#define PETRUS_VEL 3.0

struct petrusAttributes
{
	double fx; //Floating point X coord
	double fy; //Floating point Y coord
	double dx; //X difference each frame
	double dy; //Y difference each frame
};

typedef struct petrusAttributes petrusAttributes;

//-------------------------------------------

//jumpa

#define JUMPA_BASE_FRAME 16

struct jumpaAttributes
{
    s_trajectory trajectory;
};

typedef struct jumpaAttributes jumpaAttributes;

//-------------------------------
//Stom

struct stomAttributes
{
    s_trajectory trajectory;
    
    int trajectIndex;
    int framesCounter;

    double x;
    double y;

};

typedef struct stomAttributes stomAttributes;

//-------------------------------------
//Laroc

#define LAROC_LIFE 40
#define LAROC_VEL 1.0
#define LAROC_SPRITE_CENTER_X 32.0
#define LAROC_SPRITE_CENTER_Y 32.0
#define LAROC_BULLET_SPEED 2.0
#define LAROC_ROUND_DELAY 80       /* Delay between rounds */
#define LAROC_BULLETS_PER_ROUND 4  /* Bullets per round */
#define LAROC_BULLETS_DELAY 3      /* Delay between bullets */

struct larocAttributes
{
    s_trajectory trajectory;
    int roundDelayCounter;
    int bulletsCounter;
    int bulletsDelayCounter;
};

typedef struct larocAttributes larocAttributes;


//-------------------------------------
//anancitesST

#define ANANCITES_ST_LIFE 40
#define ANANCITES_ST_SHOOT_RANGE_Y 20
#define ANANCITES_SHOOT_DELAY 30
#define ANANCITES_SHOOT_FRAMES 3*4 /*Number of frames the shoot lasts*/
#define ANANCITES_CHARGE_ANIM_X_PANNIG 24
#define ANANCITES_CHARGE_ANIM_Y_PANNIG 59
#define ANANCITES_STEPS_Y_NUM 30
#define ANANCITES_STEP_Y 0.77
#define ANANCITES_STEP_X 1.0

struct anancitesSTAttributes
{
    float fx;
    float fy;
    //
    float stepX;
    float stepY;
    //
    int stepYcounter;
    //
    int shootDelayCounter;
    int shootFramesCounter;
    GameObject *shootChargeAnim;
};

typedef struct anancitesSTAttributes anancitesSTAttributes;

//-------------------------------------
//Go-Drill

#define GODRILL_LIFE 4

struct godrillTAttributes
{
    float fx;
    float fy;
    //
    float stepX;
    float stepY;
    //
};

typedef struct godrillTAttributes godrillAttributes;

//-------------------------------------
//mid-boss part

struct midBossPartAttributes
{
    s_trajectory trajectory;
    int mainPart;    
};

typedef struct midBossPartAttributes midBossPartAttributes;

//-------------------------------------
//mid-boss

#define MIDBOSS_LIFE 60

struct midBossAttributes
{
    s_trajectory trajectory;
    int shootDelay;
    int cBullet;    //Current bullet
};

typedef struct midBossAttributes midBossAttributes;


//-------------------------------------
//starView

#define STARVIEW_LIFE 30
#define STAR_VIEW_X_SPEED 1.0
#define STAR_VIEW_Y_SPEED 0.3
enum
    {
        STARVIEW_SHOOT_DELAY = 100
    };

struct starViewAttributes
{
    s_trajectory trajectory;
    int shootDelay;
    int bulletCounter;
    float fx;
    float fy;
    float dirX;
    float dirY;
};

typedef struct  starViewAttributes starViewAttributes;

//-------------------------------------
//laserRing

enum 
     {
         MAX_LASER_RING_EXPANSIONS  = LASERRING_ANIM_FRAMES
        ,LASER_RING_EXPANSION_DELAY = LASERRING_FRAME_DELAY
     };

struct laserRingAttributes
{
    int delayCounter;
    int expansion;      //Expansion counter
};

typedef struct laserRingAttributes laserRingAttributes;

//-------------------------------------------
//tirtas types

#define LEFT_TIRTAS 0
#define RIGHT_TIRTAS 1

struct tirtasAttributes
{
    s_trajectory trajectory;
};

typedef struct tirtasAttributes tirtasAttributes;

//-------------------------------------------
//Enemy homming missile

#define ENEMY_HMISSILE_PITCH_DELAY 10
#define ENEMY_HMISSILE_MAX_PITCH 15

struct enemyHMissileAttributes
{
    unsigned int pitch;
    int pitchChangeDelay;
    vector fpos;
};

typedef struct enemyHMissileAttributes enemyHMissileAttributes;

//-------------------------------------
//tekkerZ

#define TEKKERZ_LIFE 30
#define TEKKERZ_X_SPEED 1.0
#define TEKKERZ_Y_SPEED 0.3
enum
    {
        TEKKERZ_SHOOT_DELAY = 200
    };

struct tekkerzAttributes
{
    s_trajectory trajectory;
    int shootDelay;
    int bulletCounter;
    float fx;
    float fy;
    float dirX;
    float dirY;
};

typedef struct  tekkerzAttributes tekkerzAttributes;

//-------------------------------------
//archer

#define R_ARCHER 0
#define L_ARCHER 1
#define ARCHER_LIFE 1

enum
    {
        ARCHER_SHOOT_DELAY = 60
    };

struct archerAttributes
{
    s_trajectory trajectory;
    int shootDelay;
};

typedef struct archerAttributes archerAttributes;


//-------------------------------------
//maraac
#define MARAAC_LIFE 10

struct maraacAttributes
{
    s_trajectory trajectory;
};

typedef struct maraacAttributes maraacAttributes;

//-------------------------------------
//nakser part

struct nakserPartAttributes
{
    int trajectStart;
    s_trajectory trajectory;
};

typedef struct nakserPartAttributes nakserPartAttributes;

//-------------------------------------
//nakser

#define NAKSER_LIFE 30

struct nakserAttributes
{
    int trajectStart;
    s_trajectory trajectory;
    LList_t tailList;
};

typedef struct nakserAttributes nakserAttributes;

//-------------------------------------
//bknight ( beam knight)
#define BKNIGHT_LIFE 6
#define BKNIGHT_SHOOT_STEP 91
struct bKnightAttributes
{
    s_trajectory trajectory;
    int type;
    int status;
    int shootCounter;
};

typedef struct bKnightAttributes bKnightAttributes;

//-------------------------------------
//boss1

#define BOSS1_LIFE 140
#define BOSS1_MIN_Y 01
#define BOSS1_X_VEL 1.0
#define BOSS1_Y_VEL 0.2
#define BOSS1_Y_AMP 5

#define BOSS1_W 128
#define BOSS1_H 128
#define BOSS1_STEPS_Y_NUM 25
#define BOSS1_STEP_Y 0.2


struct boss1attributes
{
	double posX;
	double posY;
    float stepY;
    int stepYcounter;
	double dirX;
	int roundDelay;  //Delay between bullet rounds
	int shootDelay;  //Delay between shoots
	int cRoundBullet;//Current round bullet

    //
    GameObject *leftBurst;
    GameObject *rightBurst;
};

typedef struct boss1attributes boss1attributes;

//-------------------------------------
//boss2
#define BOSS2_LIFE 200

struct boss2attributes
{
    int rocketDelay;
    int gatlingDelay;
    int state;
    s_trajectory trajectory;
};
typedef struct boss2attributes boss2attributes;


//-------------------------------------

#endif
