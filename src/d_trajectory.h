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
   d_trajectory.h
*********************************************/

#ifndef D_TRAJECTORY_H
#define D_TRAJECTORY_H

//bit masks for the flags
#define TRAJECT_END    0x01
#define TRAJECT_REPEAT 0x02

struct trajectStep
{
    unsigned int flags;     //Trajectory step flags:
                            // "--------------rf"
                            //f = Indicates if this is the last step
                            //r = repeat traject from the beginning
                            //- = Unused bits

    int  frames;            //Number of frames that this traject step last
    //
    double x;               //Step x variation
    double y;               //Step y variation
};

typedef struct trajectStep trajectStep;

//-------------------------------------------

struct s_trajectory
{
    trajectStep *firstStep;
    trajectStep trajectPos; //Current position on the screen
                            //Current step frames counter
                            //Current step (firstStep + index)  flags
    int index;
};

typedef struct s_trajectory s_trajectory;

#endif
