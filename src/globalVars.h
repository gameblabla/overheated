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
    globalVars.h
********************************************/

#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

//game.c
extern int gameType;

//OAM.c:

#ifdef D_GAME_OBJECT_H
extern GameObjectArray OAM_enemies;
extern GameObjectArray OAM_PlBullets;
extern GameObjectArray OAM_enemiesBullets;
extern GameObjectArray OAM_bonus;
extern GameObjectArray OAM_effects;
extern GameObjectArray OAM_events;
extern GameObjectArray OAM_player;
extern GameObject *playerObj;
#endif

#ifdef D_CHARSET_H
extern charSetArray sprites_charSets;
#endif


#endif
