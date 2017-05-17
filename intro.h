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
    intro.h
*********************************************/

#ifndef INTRO_H
#define INTRO_h


int logoFade(char *logoPath, SDL_Rect *srcrect, SDL_Rect *dstrect 
             , long unsigned tfadeIn /*fade-in duration(milliseconds)*/
             , long unsigned tStill  /*Still time ( milliseconds)*/
             , long unsigned tfadeOut); /*fade-out duration(milliseconds)*/

void DDL_logo(void);
void intro(void);

#endif
