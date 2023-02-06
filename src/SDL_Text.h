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
   SDL_Text.h
*********************************************/

#ifndef SDL_TEXT_H
#define SDL_TEXT_H

//
void SDL_TextExpansion (  char *str , struct bmFont *cfont 
                        , int xPadding , int yPadding , int expansion , SDL_Surface *screen);
//
void SDL_Text(char *str , struct bmFont *cfont, int x , int y , SDL_Surface *screen);
//
int bmFontIsFree( bmFont *font);
int getFontHSpacing( bmFont *font);
int getFontVSpacing( bmFont *font);
int getFontHeight( bmFont *font);
int getFontWidth( bmFont *font);

#endif
