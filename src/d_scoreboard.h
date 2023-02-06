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
    d_scoreboard.h
*********************************************/

#ifndef D_SCOREBOARD_H
#define D_SCOREBOARD_H

#define SCORE_NAME_SIZE 5
#define SCORE_BOARD_SIZE 15

struct scoreNode
{
    void *next; //Next scoreNode
    int score;
    char name[SCORE_NAME_SIZE];

};

typedef struct scoreNode scoreNode;


struct scoreboard
{
    //(...)
    void *first; //First scoreNode
};

typedef struct scoreboard scoreboard;

#endif 
