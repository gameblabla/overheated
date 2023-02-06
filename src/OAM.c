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
    OAM.c
    Object array memory (aka: Object atribute memory)
********************************************/

#ifndef OAM_C
#define OAM_C

//
#include <SDL/SDL.h>

//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_Charset.h"

//
GameObjectArray OAM_enemies;
GameObjectArray OAM_PlBullets;
GameObjectArray OAM_enemiesBullets;
GameObjectArray OAM_effects;
GameObjectArray OAM_bonus;
GameObjectArray OAM_events;

charSetArray sprites_charSets;

//----------------------------

int activeEnemies( void)
{
	return OAM_enemies.active;
}

#endif
