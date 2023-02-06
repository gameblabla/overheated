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
   trajectory.c
   Handle object's precalculated trajectories
*********************************************/

#ifndef TRAJECTORY_C
#define TRAJECTORY_C
#endif
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "globalVars.h"
#include "d_trajectory.h"
#include "trajectory.h"

/********************************************
    trajectoryEnded
********************************************/

int trajectoryEnded( s_trajectory *trajectory)
{
    trajectStep *nextStep = trajectory->index + trajectory->firstStep;
    return M_isLastTrajectStep(nextStep) != 0 ? 1 : 0 ;
}

/********************************************
    initTrajectory:
*********************************************/

void initTrajectory(  s_trajectory *trajectory 
                    , SDL_Rect *initPos 
                    , trajectStep *firstStep )
{
    trajectory->firstStep = firstStep;
    trajectory->index = 0;

    //init traject position
    trajectStep *trajectPos = &(trajectory->trajectPos);
    trajectPos->x = initPos->x;
    trajectPos->y = initPos->y;
    trajectPos->frames = trajectory->firstStep->frames;
    trajectPos->flags  = trajectory->firstStep->flags;

    return;
}

//-------------------------------------------

/********************************************
    getStepNumber:
    Get the number of the current trajectory step
*********************************************/

int getStepNumber( s_trajectory *trajectory)
{
    return trajectory->index;
}

//-------------------------------------------

/********************************************
setPosToTraject:
    Set the position with the value
    of the trajectory position
*********************************************/

void setPosToTraject( SDL_Rect *pos , s_trajectory *trajectory)
{
    pos->x = trajectory->trajectPos.x;
    pos->y = trajectory->trajectPos.y;
    return;
}

//-------------------------------------------

/********************************************
    updateTrajectory:
*********************************************/

void updateTrajectory(SDL_Rect *pos , s_trajectory *trajectory)
{
    trajectStep *nextStep = trajectory->index + trajectory->firstStep;
    trajectStep *trajectPos = &(trajectory->trajectPos);

    //Update trajectory position
    trajectPos->x += nextStep->x;
    trajectPos->y += nextStep->y;
    setPosToTraject( pos , trajectory);

    //
    trajectPos->frames--;
    if( trajectPos->frames <= 0 )
    {
        //Check flags
        if( M_isTrajectRepeat(nextStep))
        { //Repeat traject
            trajectory->index = 0;
        }
        else if( M_isLastTrajectStep(nextStep))
        { //Last trajectory step
            ;
        }
        else
        { //No flags set
            trajectory->index++;
        }

        //Fetch next trajectory step
        nextStep = trajectory->index +  trajectory->firstStep;
        trajectPos->flags = nextStep->flags;
        trajectPos->frames = nextStep->frames;
    }
        
    return;
}

//-------------------------------------------
