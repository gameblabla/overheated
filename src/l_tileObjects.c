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
 l_tileObjects.c:
 Tile objects list
*********************************************/

#ifndef L_TILEOBJECTS_C
#define L_TILEOBJECTS_C
#endif

//
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "globalVars.h"
//
#include "tileObjects.h"

/********************************************
    Each entry contains the constructor of the object
    assigned to the tile with that index number.
Example:
    The address of the constructor for the object
    assigned to the interactive tile "i" will be:
    "interactiveTiles[i]"
*********************************************/

#define INTERACTIVE_TILES_NUMBER 28
void *interactiveTiles[ INTERACTIVE_TILES_NUMBER ] =
{
    //
    NULL,NULL,&newContainer,&newContainer,&newContainer,&newContainer,&newContainer,&newContainer,NULL,NULL,NULL,NULL,NULL,NULL,&newSpecialContainer,NULL,
    //
    &newSmallBonusContainer,&newPowerUpContainer,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
};


//-------------------------------------------

/********************************************
   getTileObject:
   Given the tile number ,
   return the constructor of the object assigned to it.
*********************************************/


void *getTileObject(int tileNumber)
{
    if(tileNumber < 0
       || tileNumber >= INTERACTIVE_TILES_NUMBER)
    return NULL;

    return interactiveTiles[tileNumber];
}


//-------------------------------------------
