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
   trajectory.h
   Handle object's precalculated trajectories
*********************************************/

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

/*******************************************
    M_setTrajectoryIndex:
    x = s_trajectory *trajectory;
    y = int index;  step index
********************************************/
#define M_setTrajectoryIndex(x , y)  (x)->index = (y)

//-------------------------------------------

//Macros for checking the trajectStep's flags

/********************************************
    M_isLastTrajectStep
    Check the last traject step flag.
    x = trajectStep *step
********************************************/
#define M_isLastTrajectStep(x) ( (x)->flags & TRAJECT_END )
#define M_isTrajectRepeat(x) ( (x)->flags & TRAJECT_REPEAT )

//-------------------------------------------


/********************************************
    Functions
*********************************************/

int trajectoryEnded( s_trajectory *trajectory);
void initTrajectory(  s_trajectory *trajectory , SDL_Rect *initPos , trajectStep *firstStep );
int  getStepNumber( s_trajectory *trajectory);
void setPosToTraject( SDL_Rect *pos , s_trajectory *trajectory);
void updateTrajectory(SDL_Rect *pos , s_trajectory *trajectory);


//-------------------------------------------

#endif
