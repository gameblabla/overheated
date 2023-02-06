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
    SDL_utils.h
*******************************************/

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

int SDL_setpixel( SDL_Surface *dest , unsigned x , unsigned y , Uint32 color);
Uint32 SDL_getPixel( SDL_Surface *source , unsigned x , unsigned y);
int SDL_zoom( SDL_Surface *source , SDL_Surface *dest 
             , SDL_Rect *destRect ,unsigned zoom);
SDL_Surface *SDL_optimizeSurface(SDL_Surface *surface);
#define MAX_FADE_VALUE 255
SDL_Surface *SDL_fadeSurface( SDL_Surface *mask , int value , SDL_Surface *dest);
SDL_Surface *createScanLineMask( SDL_Rect maskRect , int value);

SDL_Surface *loadIMG( char *path , Uint16 flags);
//load IMG flags:
#define OPTIMIZE_IMG 0x01

int check_SDL_Version(void);

#endif
