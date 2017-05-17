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
   d_bonus.h
*********************************************/

#ifndef D_BONUS_H
#define D_BONUS_H

//-------------------------------------------

#define SMALL_BONUS_TILE 6
#define SMALL_BONUS_POINTS 1000
//
#define POWERUP_BONUS_POINTS 5000

//-------------------------------------------

struct powerUpAttributes
{
    s_trajectory trajectory;	
};

typedef struct powerUpAttributes powerUpAttributes;

//-------------------------------------------

struct standardBonusAttributes
{
    s_trajectory trajectory;
    int sPoints;
};

typedef struct standardBonusAttributes standardBonusAttributes;

//-------------------------------------------

#endif

