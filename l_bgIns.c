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
 l_bgIns.c:
 Background instructions
*********************************************/

#ifndef L_BG_INS_C
#define L_BG_INS_C
#endif

//
#include <stdio.h>
#include <SDL/SDL.h>

//
#include "globalDefines.h"
#include "d_Charset.h"
#include "d_GameObject.h"
#include "charset.h"
#include "d_background.h"
#include "l_bgIns.h"

//----------------------------

struct bgInsTmp
{
	char *tag;		    //opcode name
	void *func;		    //Pointer to the op function
	int  operandsNum;	//number of operands
};

typedef struct bgInsTmp bgInsTmp;

//----------------------------
//Instructions list:

bgInsTmp bgInsList[] =
{
	//set delay
	{
		"set_speed", &bgSetSpeed, 2
	},
	//set loop repeat
	{
		"set_loopRepeat", &bgSetLoopRepeat, 1
	},
	//set loop start
	{
		"set_loopStart", &bgSetLoopStart, 1
	},
	//set loop end
	{
		"set_loopEnd", &bgSetLoopEnd, 1
	},
	//list end
	{
		NULL,NULL,0
	}
};

/********************************************
    getBgInsCode:

    Get instruction code/number
    Return -1 if the instruction was not found
*********************************************/

int getBgInsCode( char *tag)
{
	register i=0;
	while( (bgInsList+i)->tag != NULL)
	{//Search the tag
		if( strcmp( (bgInsList+i)->tag , tag) == 0)
			return i;
		i++;
	}
	return -1;
}
//----------------------------

/********************************************
    getBgInsOperandsNum:

    Get instruction operands number
*********************************************/

int getBgInsOperandsNum( int insCode) 
{
	return (bgInsList+insCode)->operandsNum;
}
//----------------------------

/********************************************
    getBgInsFunc:

    Get instruction function address
*********************************************/

void *getBgInsFunc( int insCode)
{
	return (bgInsList+insCode)->func;
}

//----------------------------

/********************************************
    execBgScript

    Execute background script
*********************************************/


#define CURRENT_SCRIPT(x) (x->script+x->currentScript)
int execBgScript( background *bg , int frame)
{
	if( bg->currentScript >= bg->scriptSize)
		return 0;//End of the script reached

	bgScript *cScript = bg->script + bg->currentScript;
	if( cScript->frame != frame
		|| cScript->frame < 0)
    {
	    return 0; //Not in the line
    }

	register i = cScript->insNum-1;
	for( ; i >= 0 ; i--)//execute instruction function
	{
		((void (*)(background *, int *))(cScript->ins+i)->func)( bg , (cScript->ins+i)->operands);
	}
	bg->currentScript++;
	return 0;
}
//----------------------------

void resetBgScript( background *bg)
{
	bg->currentScript = 0;
	return;
}

//bg instructions
void bgSetSpeed( background *bg , int *operands)
{
	bg->scroll.inc = *operands;
	bg->scroll.framesDelay = *(operands+1);
	bg->scroll.framesCounter = 0;

	return;
}

void bgSetLoopRepeat( background *bg , int *operands)
{
	bg->scroll.loopRepeat = *operands;
	return;
}

void bgSetLoopStart( background *bg , int *operands)
{
	bg->scroll.loopStart = *operands;
	return;
}

void bgSetLoopEnd( background *bg , int *operands)
{
	bg->scroll.loopEnd = *operands;
	return;
}
