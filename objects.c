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
   objects.c
*********************************************/

#ifndef OBJECTS_C
	#define OBJECTS_C
#endif

//System includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

//Program definitions & declarations

#include "globalDefines.h"
#include "d_Charset.h"
#include "d_GameObject.h"
#include "d_background.h"
#include "globalVars.h"

//Program modules
#include "enemies.h"
#include "enemyBullet.h"
#include "collisions.h"
#include "objects.h"
#include "bonus.h"
#include "bullets.h"

//
//#define DRAW_COLBOX_OVER
//#define DRAW_POWERLESS_COLBOX 0

/*
===================
initObjectArray:
   Set the array size , allocate memory for all the objects data
   & init all the data.
return:
   0 if everything goes well
   !0 if there is any problem
===================
*/

int initObjectArray(GameObjectArray *objArray , int  size)
{
	objArray->object = (GameObject *) malloc( sizeof(GameObject) * size);
	if(objArray->object == NULL)
	{
		return -1;
	}

	objArray->freeObjStack = (GameObject **) malloc( sizeof(objArray->object) * size);//objArray->object is of type (GameObject *)
	if(objArray->freeObjStack == NULL)
	{
		free(objArray->object);
        objArray->object = NULL;
		return -1;
	}

	objArray->size = size;

	//init objects array
	initObjects( objArray->object , size);

	//init free objects array
	register i;
	for(i = size-1; i >= 0 ; i--)
		*(objArray->freeObjStack+i) = objArray->object+i;

	objArray->nxtFree = 0;
	objArray->active = 0;

	return 0;
}

/*
===================
getObjectPos
===================
*/

SDL_Rect getObjectPos( GameObject *object)
{
    SDL_Rect pos = object->pos;
    return pos;
}

//---------------------------------------

/***********************
setObjectPos
************************/

void setObjectPos( GameObject *object , const SDL_Rect *pos)
{
    object->pos.x = pos->x;
    object->pos.y = pos->y;
    return;
}


///---------------------------------------


//=======================================================================

void setObjectAnimation( GameObject *obj , GmObjectAnim *anim)
{
	if( anim == NULL || obj == NULL)
		return;

	obj->tile = anim->base;
	obj->anim.base = anim->base;
	obj->anim.frames = anim->frames;
	obj->anim.counter = anim->frames;
	obj->anim.delay = anim->delay;
	obj->anim.timer = anim->delay;
	obj->anim.flags = anim->flags;

	return;
}

//=========================================================================

void freeObjectArray(GameObjectArray *objArray)
{
	if(objArray->object != NULL)
	{
		freeNobjects( objArray->object , objArray->size );
		free(objArray->object);
        objArray->object = NULL;
	}

	if( objArray->freeObjStack != NULL)
    {
		free( objArray->freeObjStack );
        objArray->freeObjStack = NULL;
    }

	objArray->size = 0;
	objArray->nxtFree = 0;
	objArray->active = 0;

	return;
}


//=========================================================================

void initObjects( GameObject *objects , int objNum)
{
	for( objNum-- ;  objNum >= 0 ; objNum--)
	{
		(objects + objNum)->func = NULL;
		(objects + objNum)->attributes = NULL;
	}
	return;
}

//==========================================================================

/*
===================
newObject:
   Initialize the GameObject fields with the given parameters
return:
   pointer to the new object
NOTE:
   DON'T use this function
===================
*/

GameObject *newObject( GameObject *object , SDL_Rect *pos , SDL_Rect *colBox
		, int chrData , int tile
		, Uint8 propieties , int power , GmObjectAnim *anim)
{
		object->pos = *pos;
		object->colBox = *colBox;

		object->power = power;
		object->collision = 0;

		object->chrData = chrData;
		object->tile = tile;
		object->properties = propieties;

		setObjectAnimation( object , anim);

		return object;
}

//==========================================================================

/*
===================
newObjANDpull:
   Make a new object and pull his address from the free objects stack
return:
   if everything goes well , returns a pointer to the new object
   NULL if there are not free objects available
===================
*/

GameObject *newObjANDpull( GameObjectArray *objects , const SDL_Rect *pos , SDL_Rect *colBox
		, int chrData , int tile
		, Uint8 propieties , int power , GmObjectAnim *anim)
{
		//Check if there are free objects available
		if( objects->nxtFree > objects->size-1 )
			return NULL;

		GameObject *newObjPtr = *(objects->freeObjStack + objects->nxtFree);
		*(objects->freeObjStack + objects->nxtFree) = NULL;
		objects->nxtFree++;

		newObjPtr->pos    = *pos;
		newObjPtr->colBox = *colBox;

		newObjPtr->power = power;
		newObjPtr->collision = 0;

		newObjPtr->chrData = chrData;
		newObjPtr->tile = tile;
		newObjPtr->properties = propieties;

		setObjectAnimation( newObjPtr , anim);
		objects->active++;

		return newObjPtr;
}

//==========================================================================

void freeObject( GameObject *object)
{
		object->func = NULL;
		if (object->attributes != NULL)
		{
			free(object->attributes);
			object->attributes = NULL;
		}
		return;
}

void freeANDpushObj( GameObjectArray *objArray , GameObject *object)
{
		if( objArray->nxtFree <= 0)
        {
#ifdef DEBUG
		    printf("[WARNING] The Free objects stack is full\n");
#endif
			return;
        }

		//Push Obj pointer to the free stack
		objArray->nxtFree--;
		*(objArray->freeObjStack+objArray->nxtFree) = object;

		//freeObject data
		freeObject(object);

#ifdef DEBUG
		//printfGameObjectArray(stdin , "--Free&Push--\n" , &objArray);
#endif
		objArray->active--;
		return;
}

void freeNobjects( GameObject *objects , int num)
{
	for( num-- ;  num >= 0 ; num--)
		freeObject(objects+num);

	return;
}

void manageObjectsCollisions( GameObject *objects1 , int obj1Num ,GameObject *objects2 , int obj2Num)
{
	register int i;
	register int j;

	for( i = obj1Num-1 ; i >= 0 ; i--)
	{
		if( (objects1+i)->func == NULL)
		continue;

		for(j = obj2Num-1; j >= 0 ; j--)
		{
			if( (objects2+j)->func == NULL)
			continue;

			if(objCollision( objects1+i , objects2+j))
			{
				(objects1 + i)->collision += (objects2 + j)->power;
				(objects2 + j)->collision += (objects1 + i)->power;
			}
		}
	}

	return;
}

void manageObjects(GameObjectArray *objects)
{

	//Execute object functions
	int i;
	for(i = 0 ; i < objects->size ; i++)
	{
		if( (objects->object+i)->func != NULL)
		{ 
			( *(void (*)(void *))((objects->object+i)->func) )(objects->object+i);
		}
	}
	return;
}

int searchEmptyObject( GameObjectArray *objects)
{
	//If there are not free slots , return -1
	//Else return object number
	return objects->nxtFree < objects->size ? objects->nxtFree : -1;
}

void drawColBox( GameObject *obj , SDL_Surface *screen)
{
    SDL_Surface *colBox = SDL_CreateRGBSurface( SDL_SWSURFACE
                                              , obj->colBox.w
                                              , obj->colBox.h
                                              , SCREEN_BPP
					                          , 0, 0, 0, 0
                                              );
    SDL_Rect boxPos =
    { 
        obj->pos.x + obj->colBox.x,
        obj->pos.y + obj->colBox.y,
        0,
        0,
    };

    SDL_FillRect(colBox , NULL , SDL_MapRGB(colBox->format ,255,0,0));
    SDL_BlitSurface( colBox , NULL , screen , &boxPos);
    SDL_FreeSurface(colBox);
    return;
}

void drawObjects( GameObjectArray *objsArray , SDL_Surface *screen)
{
	GameObject *objects = objsArray->object;
	int objNum = objsArray->size;

	register charSet *ObjChrIndex;
	SDL_Rect sprite , dest;
	register i= 0;
	for( ; i < objNum ; i++)
	{
		ObjChrIndex = sprites_charSets.array + (objects+i)->chrData;
		if( (objects+i)->func == NULL )
			continue;

		sprite.w = ObjChrIndex->tileW;
		dest.w = sprite.w;
		sprite.h = ObjChrIndex->tileH;
		dest.h = sprite.h;
		dest.x = ((objects+i)->pos).x;
		dest.y = ((objects+i)->pos).y;

		sprite.x = ((objects+i)->tile 
			   %  ObjChrIndex->tilesPerLine )
			   *  ObjChrIndex->tileW;
		sprite.y = ((objects+i)->tile 
			   / ObjChrIndex->tilesPerLine )
			   * ObjChrIndex->tileH;

#ifdef DRAW_COLBOX_UNDER
        if(DRAW_POWERLESS_COLBOX || (objects+i)->power)
        {
            drawColBox( objects+i , screen);
        }
        
#endif
        //Blit object
		SDL_BlitSurface( ObjChrIndex->gfx , &sprite
				, screen , &dest);

#ifdef DRAW_COLBOX_OVER
        if(DRAW_POWERLESS_COLBOX || (objects+i)->power)
        {
            drawColBox( objects+i , screen);
        }
#endif

	}
	return;
}

void manageObjectsAnim( GameObjectArray *objsArray)
{

	GameObject *objects = objsArray->object;
	int objNum = objsArray->size;

	register i;
	register GameObject  *cObject;
	for( i = objNum-1 ; i >= 0 ; i--)
	{
		cObject = objects+i; //Current object
		//if the object is empty
		//or the animation has ended
		if( cObject->func == NULL 
		    || cObject->anim.counter == 0 )
			continue;

		//Check if there is a frame active
		if( cObject->anim.timer > 0)
		{
			cObject->anim.timer--;
			continue;
		}

		cObject->anim.counter--;
		//If the animation has ended
		if( cObject->anim.counter <= 0)
		{
			//If the repeat flag is unset , process next object
			if(!(cObject->anim.flags & ANIMATION_REPEAT))
			continue;

			//reset animation values:
			cObject->anim.counter = cObject->anim.frames;
			cObject->anim.timer = cObject->anim.delay;
			cObject->tile = cObject->anim.base;
			continue;
		}

		//Update animation
		cObject->anim.timer = cObject->anim.delay;
		cObject->tile++;

	}
	return;
}

/********************************************
    freeTileObject:

    Clean the object slot fields
    and free the attributes memory
/********************************************/

void freeTileObject( GameObject *object)
{
        if( object->attributes != NULL)
        {
            free(object->attributes);
        }

        SDL_Rect tilePos = object->pos;
        //Reset the object slot
        memset ( object, 0x00, sizeof(GameObject));
        //Restore the tile position
        object->pos = tilePos;

        return;
}

//--------------------------------------------

/********************************************
    objAnimEnded:

    Returns a value != 0 if the animation has ended
    returns 0 otherwise.
/********************************************/


int objAnimEnded( GameObject *object)
{
    return object->anim.counter <= 0 ? 1 : 0;
}


//--------------------------------------------

