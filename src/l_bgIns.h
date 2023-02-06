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
    l_bgIns.h
    Background instructions    
*********************************************/

int getBgInsCode( char *tag);		           //get instruction number
int getBgInsOperandsNum( int insCode);         //get instruction operands number
void *getBgInsFunc( int insCode);	           //get the instruction function
int execBgScript( background *bg , int frame); //Execute stage script
void resetBgScript( background *bg);		   //Reset script


//Private
#ifdef L_BG_INS_C

void bgSetSpeed( background *bg , int *operands);
void bgSetLoopRepeat( background *bg , int *operands);
void bgSetLoopStart( background *bg , int *operands);
void bgSetLoopEnd( background *bg , int *operands);

#endif
