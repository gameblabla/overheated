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
    background.h:
    Manage tile based backgrounds , (animate , scroll , draw , etc...)
*********************************************/

#ifndef BACKGROUND_H
#define BACKGROUND_H

#define M_getTileAddress(map,x,y,yOffset) map.data + ((y+yOffset)%map.h) * map.w + x
#define M_getTileIndex(map,x,y,yOffset) ((y+yOffset)%map.h) * map.w + x

/********************************************
    fillTilemapBuffer:
        FILL the tilemap BUFFER with a portion of the STAGE tilemap.
*********************************************/
void fillTilemapBuffer( background *bg , unsigned row); 

/********************************************
    setTilemapObjects

    Construct/Initialize all the objects of the current
    tilemapBuffer
*********************************************/
void setTilemapObjects(background *bg);

/********************************************
    updateBackgroundScroll
*********************************************/
void updateBackgroundScroll( background *bg , SDL_Surface *screen);

/*********************************************
    updateTileObjsPosition
**********************************************/
void updateTileObjsPosition(background *bg);

/********************************************
    manageBackgroundCollision:
        Handle collisions between objects and background tiles
*********************************************/
void manageBackgroundCollisions( GameObjectArray *objsArray , background *bg);

#endif
