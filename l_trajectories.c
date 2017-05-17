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


/***********************************
 l_trajectories.c
 Objects precalculated trajectories
************************************/

#ifndef L_TRAJECTORIES_C
#define L_TRAJECTORIES_C
#endif

#include "d_trajectory.h"

trajectStep stomLD_trajectory[] = 
{
    #include "./trajectories/stomLD_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep stomRD_trajectory[] = 
{
    #include "./trajectories/stomRD_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep stomLU_trajectory[] = 
{
    #include "./trajectories/stomLU_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep stomRU_trajectory[] = 
{
    #include "./trajectories/stomRU_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep bonus_trajectory[] =
{
    #include "./trajectories/bonus_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep spitze_trajectory[] =
{
    #include "./trajectories/spitze_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep vtSpitzeR_trajectory[] =
{
    #include "./trajectories/vtSpitzeR_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep vtSpitzeL_trajectory[] =
{
    #include "./trajectories/vtSpitzeL_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep gaucer_trajectory[] =
{
	 {0 , 40 , 0.000000 , 2.00000 }
    ,{0 , 100  , 0.000000 , 0.00000 }
	, {TRAJECT_END , 1 ,  0.000000 , 2.00000 }
};

//--------------------------------------------------------------------------

trajectStep farbenL_trajectory[] =
{
    #include "./trajectories/farbenL_trajectory.inc"
};


//--------------------------------------------------------------------------

trajectStep farbenR_trajectory[] =
{
    #include "./trajectories/farbenR_trajectory.inc"
};

//--------------------------------------------------------------------------

#define LAROC_X_VEL  0.3
#define LAROC_Y_VEL -0.5

trajectStep laroc_trajectory[] =
{
    #include "./trajectories/laroc_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep jumpaLU_trajectory[] =
{
    #include "./trajectories/jumpaLU_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep jumpaRU_trajectory[] =
{
    #include "./trajectories/jumpaRU_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep jumpaLD_trajectory[] =
{
    #include "./trajectories/jumpaLD_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep jumpaRD_trajectory[] =
{
    #include "./trajectories/jumpaRD_trajectory.inc"
};

//--------------------------------------------------------------------------

trajectStep LU_midBossP_trajectory[] =
{
    {0 , 44  ,   1.000000 , 1.0000000 }
   ,{0 , 29  ,   4.000000 , 0.0000000 }
   ,{0 , 28  ,   0.000000 , 2.0000000 }
   ,{1 , 0   ,  0.000000 , 0.0000000 }
};

//--------------------------------------------------------------------------

trajectStep RU_midBossP_trajectory[] =
{
    {0 , 44  , -1.000000 , 1.0000000 }
   ,{0 , 29  , -4.000000 , 0.0000000 }
   ,{0 , 28  ,   0.000000 , 2.0000000 }
   ,{1 , 0   ,  0.000000 , 0.0000000 }
};

//--------------------------------------------------------------------------

trajectStep LD_midBossP_trajectory[] =
{
    {0 , 44 ,   1.000000 ,  -1.0000000 }
   ,{0 , 29  ,   4.000000 , 0.0000000 }
   ,{0 , 28  ,   0.000000 , -3.428571429 }
   ,{1 , 0   ,  0.000000 , 0.0000000 }
};

//--------------------------------------------------------------------------

trajectStep RD_midBossP_trajectory[] =
{
    {0 , 44 ,  -1.000000 ,  -1.0000000 }
   ,{0 , 29  , -4.000000 , 0.0000000 }
   ,{0 , 28  ,   0.000000 , -3.428571429 }
   ,{1 , 0   ,  0.000000 , 0.0000000 }
};

//--------------------------------------------------------------------------

trajectStep midBoss_trajectory[] =
{
    #include "./trajectories/midBoss_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep tirtasL_trajectory[] =
{
    #include "./trajectories/tirtasL_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep tirtasR_trajectory[] =
{
    #include "./trajectories/tirtasR_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep r_archer_trajectory[] =
{
    #include "./trajectories/r_archer_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep l_archer_trajectory[] =
{
    #include "./trajectories/l_archer_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep maraac_trajectory[] =
{
    #include "./trajectories/maraac_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep nakser_trajectory[] =
{
    #include "./trajectories/nakser_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep bKnightL_trajectory[] =
{
    #include "./trajectories/bKnightL_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep bKnightR_trajectory[] =
{
    #include "./trajectories/bKnightR_trajectory.inc"
};

//--------------------------------------------------------------------------
trajectStep boss2_trajectory[] =
{
    #include "./trajectories/boss2_trajectory.inc"
};
