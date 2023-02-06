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
    background.c:
    Manage tile based backgrounds
*********************************************/

#ifndef BACKGROUND_C
#define BACKGROUND_C

//System
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

//Program
#include "globalDefines.h"
#include "d_Charset.h"
#include "d_GameObject.h"
#include "d_background.h"
#include "l_bgIns.h"
#include "l_tileObjects.h"
#include "background.h"
#include "collisions.h"
#include "objects.h"

//
#define ERROR_CODE -1
#define ERROR_MSG
#define ERROR_HANDLING exit(-1);

#define SYSTEM_VARS_NUMBER 0   //  See str_utils.h
#define STR_BUFFER_MAXSIZE 100 //  See str_utils.h


/*******************************************
    bgIsInteractive
********************************************/

int bgIsInteractive( background *bg)
{
    return bg->interactive;
}

//-------------------------------------------
/********************************************
    fillTilemapBuffer:

    FILL the tilemap BUFFER with a portion of the STAGE tilemap.
    The STAGE area that will be copied to the buffer starts at
    row(y) and ends at row+tilemapBuffer.h-1 or at the end of the STAGE tilemap

    *This function is used to initialize the contents of the tilemap buffer
     before the game begins

*********************************************/

void fillTilemapBuffer( background *bg , unsigned row)
{        
    if(row >= bg->tilemap.h)
        return;

    long transferSize = 0;

    if(row + bg->tilemapBuffer.h >= bg->tilemap.h)
        transferSize = bg->tilemap.h - row; 
    else
        transferSize = bg->tilemapBuffer.h;

    transferSize *= bg->tilemapBuffer.w * sizeof(Uint16);

    void *transferSrc =  bg->tilemap.w * row
                       + bg->tilemap.data;

    memcpy(bg->tilemapBuffer.data , transferSrc , transferSize);
    return;
}
//-------------------------------------------
/********************************************
    setTilemapObjects

    Construct/Initialize all the tile objects of the current
    tilemapBuffer

    *This function is used to initialize the contents
    of the tilemap buffer objects before the game begins
*********************************************/


void setTilemapObjects(background *bg)
{
    Uint16 *tileAddress = bg->tilemapBuffer.data;
    GameObject *objectAddress = bg->tilemapBuffer.tileObj;

    if( objectAddress == NULL)
        return;

    void * newObjectFunc;
    uint_fast32_t i,j;
    for( i = 0 ; i < bg->tilemapBuffer.h ; i++)
    {
        for( j = 0 ; j < bg->tilemapBuffer.w ; j++)
        {

            //Clear the object slot
            freeTileObject(objectAddress);

            //Retrieve the object constructor function
            //using the tile number (tile value)
            newObjectFunc = getTileObject(*(tileAddress));

            if( newObjectFunc != NULL)
            {
                //call the object constructor
                (*( void (*)(GameObject *, Uint16 *))newObjectFunc)(objectAddress , tileAddress);
            }

            tileAddress++;
            objectAddress++;
        }
    }

    //Set the objects initial position
    updateTileObjsPosition(bg);

    return;
}

//-------------------------------------------

/*********************************************
    updateTileObjects

    Update the objects in the current tilemap
    SCROLL row ( scroll row offset)

    *When the tilemap is scrolled
     the tile objects array is updated using this
     function
**********************************************/

void updateTileObjects(background *bg)
{
    int tileIndex = M_getTileIndex(bg->tilemapBuffer,0,0,bg->scroll.rowOffset);

    SDL_Rect tilePos = {  bg->scroll.tileScroll.x 
                        , bg->scroll.tileScroll.y 
                        , 0 , 0};

    void *newObjectFunc;
    GameObject *tileObject;
    Uint16 *tileAddress;
    uint_fast32_t i;
    for(i = 0 ; i < bg->tilemapBuffer.w ; i++ , tileIndex++)
    {
            tileObject = bg->tilemapBuffer.tileObj + tileIndex;
            tileAddress = bg->tilemapBuffer.data + tileIndex;
            //Retrieve the object constructor function
            //using the tile number (tile value)
            newObjectFunc = getTileObject(*(tileAddress));

            //Free object slot
            freeTileObject(tileObject);

            if( newObjectFunc != NULL)
            {
                //call the object constructor
                (*( void (*)(GameObject *, Uint16 *))newObjectFunc)(tileObject , tileAddress);
            }

            tilePos.x += bg->tileset->tileW;
    }
    return;
}

//-------------------------------------------

/*********************************************
    updateTileObjsPosition

    Update the position attributes
    of each tile object
**********************************************/

void updateTileObjsPosition(background *bg)
{
    int tileIndex = M_getTileIndex(bg->tilemapBuffer,0,0,bg->scroll.rowOffset);

    SDL_Rect tilePos = {  bg->scroll.tileScroll.x 
                        , bg->scroll.tileScroll.y 
                        , 0 , 0};

	uint_fast32_t i,j;
    for( i = 0 ; i < bg->tilemapBuffer.h ; i++)
    {
        tilePos.x = 0;

        for( j = 0 ; j < bg->tilemapBuffer.w ; j++)
        {
            //Set object position
            setObjectPos( bg->tilemapBuffer.tileObj + tileIndex , &tilePos);

            tilePos.x += bg->tileset->tileW;
            tileIndex++;
        }        
        tileIndex %= bg->tilemapBuffer.tilesNumber;
        tilePos.y += bg->tileset->tileH;
    }

    return;
}

//-------------------------------------------

/********************************************
    scrollBackground:

    Update all the necessary data to perform
    the background scroll
*********************************************/

void scrollBackground( background *bg)
{    

    //Update row offset
    if(--bg->scroll.rowOffset < 0)
            bg->scroll.rowOffset = bg->tilemapBuffer.h-1;

    //Copy the new Row on the new scroll offset
    if( bg->pos.y < 0
        || bg->pos.y >= bg->tilemap.h)
        return;

#ifdef DREAMCAST
    memcpy_( M_getTileAddress(bg->tilemapBuffer,0,0,bg->scroll.rowOffset)
           ,bg->tilemap.data + bg->pos.y * bg->tilemap.w
           ,bg->tilemap.w * sizeof(Uint16));
#else
    memcpy( M_getTileAddress(bg->tilemapBuffer,0,0,bg->scroll.rowOffset)
           ,bg->tilemap.data + bg->pos.y * bg->tilemap.w
           ,bg->tilemap.w * sizeof(Uint16));
#endif
    if(bg->interactive == 0)
        return;

    //Update the tilemap objects
    updateTileObjects(bg);
    
    return;
}

//-------------------------------------------
/********************************************
    drawBackground:

    Draw the background tilemap buffer on the given surface
*********************************************/

int drawBackground( background *bg , SDL_Surface *screen)
{
	SDL_Rect tileBmRect = //Tile area on the tileset bitmap
	{
		 0,0 // x ,y
		,bg->tileset->tileW,bg->tileset->tileH // w , h
	};

	SDL_Rect tileCoord = tileBmRect; //tile area on the screen
    tileCoord.y = bg->scroll.tileScroll.y; //Horizontal tile scroll

    SDL_Rect destRect;
    Uint16 *rowAddress = 0;
    int32_t tileValue = 0;
	int32_t i,j;
	for( i= 0 ; i < bg->tilemapBuffer.h ; i++)
    {
        if( tileCoord.y >= screen->h)
            break;
        //Calculate the address of the current tilemap row
        rowAddress = M_getTileAddress(bg->tilemapBuffer,0,i,bg->scroll.rowOffset);
        //Blit all the tiles on the row
        for(j = 0 ; j < bg->tilemapBuffer.w ; j++)
        {
            if(tileCoord.x >= screen->w)
                continue;
            //Get tile value/number
            tileValue = *(rowAddress+j);
            //Calculate tile position in the tileset bitmap
			tileBmRect.y = ( tileValue / bg->tileset->tilesPerLine) * bg->tileset->tileH;
			tileBmRect.x = ( tileValue % bg->tileset->tilesPerLine) * bg->tileset->tileW;
            //Blit it!
            destRect = tileCoord;
            SDL_BlitSurface( bg->tileset->gfx , &tileBmRect , screen , &destRect);
            tileCoord.x += tileCoord.w;
        }

        tileCoord.y += tileCoord.h;
        tileCoord.x = 0;
    }

	return 0;
    
}

//-------------------------------------------
/********************************************
    updateBackground:

    Manage background scroll
*********************************************/

void updateBackgroundScroll( background *bg , SDL_Surface *screen)
{
	if( bg->scroll.inc == 0)
	{
		drawBackground( bg , screen);
			return;
	}

	if( bg->scroll.loopRepeat == 0
	    && bg->pos.y <= bg->scroll.loopEnd)
	{
		drawBackground( bg , screen);
			return;
	}

	bg->scroll.framesCounter--;
	if( bg->scroll.framesCounter > 0)
	{//The delay is not over , dont update the scroll
		drawBackground( bg , screen);
			return;
	}

	bg->scroll.framesCounter = bg->scroll.framesDelay;

	//update scroll
	bg->scroll.tileScroll.y += bg->scroll.inc;

	if( bg->scroll.tileScroll.y > 0)
	{
		bg->scroll.tileScroll.y = -bg->tileset->tileH+1;

		if(bg->pos.y <= bg->scroll.loopEnd)
			bg->pos.y = bg->scroll.loopStart;
		else
			bg->pos.y--;

        scrollBackground( bg);
	}


    drawBackground( bg , screen);

    if(bg->interactive)
    {
        updateTileObjsPosition(bg);
	}
	return;
}

//-------------------------------------------
/********************************************
    manageBackgroundCollision

    Handle collisions between objects and background tiles
*********************************************/

//DEV NOTE: backgroundCollision is inside collisions.c
void manageBackgroundCollisions( GameObjectArray *objsArray , background *bg)
{
    if(bg->interactive == 0)
        return;
    GameObject *objects = objsArray->object;
	int32_t objNum = objsArray->size-1;
    
    for( ; objNum > 0 ; objNum--)
    {
        if( (objects+objNum)->func != NULL)
        {
            backgroundCollision( objects+objNum , bg);
        }
    }

    return;
}

//
#endif
