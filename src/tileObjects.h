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
   tileObjects.h
*********************************************/

#ifndef TILE_OBJECTS_H
#define TILE_OBJECTS_H

//-------------------------------------------

#define CONTAINER_LIFE 2
#define SPHERICAL_CONTAINER_LIFE 2
#define SPECIAL_CONTAINER_LIFE 25
//
#define DESTROYED_CONTAINER_TILE 1
#define DESTROYED_SPHERERICAL_CONTAINER_TILE 15
//
#define CONTAINER_BONUS_POINTS 10
#define SPECIAL_BONUS_POINTS 80000
//
#define SPECIAL_BONUS_STR "SPECIAL BONUS 8000"

//-------------------------------------------

void container( GameObject *object);
void specialContainer( GameObject *object);
void newContainer(GameObject  *object , Uint16 *tilePtr);
void newPowerUpContainer(GameObject *object , Uint16 *tilePtr);
void newSmallBonusContainer(GameObject *object , Uint16 *tilePtr);
void newSpecialContainer(GameObject *object , Uint16 *tilePtr);
    

#endif
