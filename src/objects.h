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


/*******************************************
    objects.h
    Functions to manipulate single objects
*******************************************/

//NOTE: One "GameObject array"(GameObject[]) is not the same that "GameObjectArray" , see d_GameObject.h

//private ( only used by the objects.c module)
#define executeObj(objPtr) ( *(void (*)(void *))(objPtr->func) )(objPtr)

//DON'T use this functions outside "objects.c":
#ifdef OBJECTS_C

void initObjects( GameObject *objects , int objNum);
void freeNobjects( GameObject *objects , int num);
void freeObject( GameObject *object);

//newObject: Initialize the GameObject fields with the given parameters
GameObject* newObject( GameObject *object , SDL_Rect *pos , SDL_Rect *colBox
		, int chrData , int tile
		, Uint8 propieties , int power, GmObjectAnim *anim);

#endif

//Public
SDL_Rect getObjectPos( GameObject *object);
void setObjectPos( GameObject *object , const SDL_Rect *pos);

//----------
//Functions to manipulate "GameObjectArray" structures
//----------

void manageObjects( GameObjectArray *objects);

//initObjectArray:
//Set the array size , allocate memory for all the objects data & init all the data.
int  initObjectArray(GameObjectArray *objArray , int  size);
void freeObjectArray(GameObjectArray *objArray);

//newObjANDpull:
//Make a new object and pull his address from the free objects stack
GameObject* newObjANDpull( GameObjectArray *objects  , const SDL_Rect *pos , SDL_Rect *colBox
		, int chrData , int tile
		, Uint8 properties , int power, GmObjectAnim *anim);

int searchEmptyObject( GameObjectArray *objects);
void freeANDpushObj( GameObjectArray *objArray , GameObject *object);	//Free one object and push it on the free objects stack

void drawObjects( GameObjectArray *objsArray , SDL_Surface *screen);
void setObjectAnimation( GameObject *obj , GmObjectAnim *anim);
void manageObjectsAnim( GameObjectArray *objsArray);

//--------------------------------------------

void freeTileObject( GameObject *object);
int  objAnimEnded( GameObject *object); // Returns a value != 0 if the animation has ended , returns 0 otherwise


