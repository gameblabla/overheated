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
    collisions.c
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_background.h"
#include "globalVars.h"

//Program modules
#include "background.h"

/********************************************
    objCollision:

    Detect collisions between objects
*********************************************/

#define AC c - a
#define AB object1->colBox.w
#define CA a - c
#define CD object2->colBox.w
#define EG g - e
#define EF object1->colBox.h
#define GE e - g
#define GH object2->colBox.h

int objCollision( GameObject *object1 , GameObject *object2)
{
	//Collision on X axis
	int a = object1->colBox.x + object1->pos.x;
	int c = object2->colBox.x + object2->pos.x;

	if(a < c )
	{
		if( AC > AB)
			return 0;
	}
	else
	{
		if( CA > CD)
			return 0;
	}
	//Collision on Y axis
	int e = object1->colBox.y + object1->pos.y;
	int g = object2->colBox.y + object2->pos.y;

	if( e < g)
	{
		if( EG > EF)
			return 0;
	}
	else
	{
		if( GE > GH)
			return 0;
	}

	return 1;

}

//-------------------------------------------
/********************************************
    backgroundCollision

    Handle collisions between the object and backgroud tiles
*********************************************/

int backgroundCollision( GameObject *object , background *bg)
{
    if( object->pos.x + object->colBox.x < bg->scroll.tileScroll.x
        ||  object->pos.y + object->colBox.y < bg->scroll.tileScroll.y)
        return 0;

    //Postion of the object collision box on the background tilemap
    bgCoord colPos =
    {
         object->pos.x + object->colBox.x - bg->scroll.tileScroll.x
        ,object->pos.y + object->colBox.y - bg->scroll.tileScroll.y
    };

    //Background TILE position of the object collision box
    bgCoord colTile =
    {
         colPos.x/bg->tileset->tileW
        ,colPos.y/bg->tileset->tileH
    };
    //Object collision box AREA SIZE IN TILES
    bgCoord colArea;

    colArea.x = (object->colBox.w + colPos.x % bg->tileset->tileW) / bg->tileset->tileW
                + ((object->colBox.w + colPos.x % bg->tileset->tileW) % bg->tileset->tileW ? 1 : 0);

    colArea.y = (object->colBox.h + colPos.y % bg->tileset->tileH) / bg->tileset->tileH
                + ((object->colBox.h + colPos.y % bg->tileset->tileH) % bg->tileset->tileH ? 1 : 0);

    register i,j;
    bgCoord cTile; //Current Tile
    GameObject *tileObject;
    for( i = 0 ; i < colArea.y ; i++)
    {
        cTile.y = colTile.y + i;
        if( cTile.y >= bg->tilemapBuffer.h)
           return 0;

        for( j = 0 ; j < colArea.x ; j++)
        {
            cTile.x = colTile.x + j;
            if(cTile.x >= bg->tilemap.w)
                break;

            tileObject =  bg->tilemapBuffer.tileObj 
                        + M_getTileIndex(bg->tilemapBuffer,cTile.x,cTile.y,bg->scroll.rowOffset);

            if( tileObject->func != NULL)
            {
                //Update collision damage counters
                object->collision += tileObject->power;
    			tileObject->collision += object->power;

                /*
                //Update tile object position
                tileObject->pos.x = cTile.x * bg->tileset->tileW - bg->scroll.tileScroll.x;
                tileObject->pos.y = cTile.y * bg->tileset->tileH - bg->scroll.tileScroll.y;
                */

                //Execute tile object
                ( *(void (*)(void *))(tileObject->func) )(tileObject);

            }

        }
    }

    return 1;
}
