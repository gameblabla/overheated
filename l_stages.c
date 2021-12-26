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
    l_stages.c
    Stages list
*********************************************/
//
#include <stdio.h>
#include "l_stages.h"
#include "globalDefines.h"
//
#define STAGES_NUMBER 2
//

static char *stages[STAGES_NUMBER] =
{
     DREAMCAST_CD_PATH"data/stages/city/city.stage"
    ,DREAMCAST_CD_PATH"data/stages/space/space.stage"
};

/*******************************************
    getStagePath:
    Returns the stage file path

    -RETURNS NULL if number is an invalid
     stage number
********************************************/

char *getStagePath( unsigned number)
{
    if( number < STAGES_NUMBER)
        return *(stages+number);
    else
        return NULL;
}

/**************************************
    isFinalStage:

    -Returns 1 if is the last stage or
    if number is out of the map list range

    -Returns 0 otherwise
**************************************/

int isFinalStage( unsigned number)
{
    return number >= STAGES_NUMBER-1 ? 1 : 0; 
}

/***************************************
    stagesNumber:

    Return number of stages
****************************************/
int stagesNumber( void)
{
    return STAGES_NUMBER;
}
