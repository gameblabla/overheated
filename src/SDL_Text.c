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
   SDL_Text.c
*********************************************/

#ifndef SDL_TEXT_C
#define SDL_TEXT_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "d_bmFont.h"
#include "str_utils.h"
#include "SDL_Text.h"
//
#define getCharTileNumber(x) x-32

//------------------------------------------------

/********************************************
    getFontHSpacing
*********************************************/

int getFontHSpacing( bmFont *font)
{
    return font->wSpacing;
}

//------------------------------------------------

/********************************************
    getFontWSpacing
*********************************************/

int getFontWSpacing( bmFont *font)
{
    return font->hSpacing;
}

//------------------------------------------------

/********************************************
    getFontWidth
*********************************************/

int getFontWidth( bmFont *font)
{
    return font->size.w;
}

//------------------------------------------------

/********************************************
    getFontHeight
*********************************************/

int getFontHeight( bmFont *font)
{
    return font->size.h;
}

//------------------------------------------------

/********************************************
    bmFontIsFree
*********************************************/

int bmFontIsFree( bmFont *font)
{
    return font->gfx == NULL ? 1 : 0;
}

//------------------------------------------------

/********************************************
    SDL_TextExpansion
        Text expansion effect
*********************************************/

void SDL_TextExpansion (char *str , struct bmFont *cfont , int xPadding , int yPadding , int expansion , SDL_Surface *screen)
{
    //Expanded font
    struct bmFont expFont = *cfont;

    double  XYratio = (double)cfont->hSpacing / (double)cfont->wSpacing ;
    expFont.wSpacing = expansion;
    expFont.hSpacing = expansion * XYratio;

    //Get text dimensions ( in character units)
    int textHeight;
    int textWidth;
    strTxtDimension( str , &textWidth , &textHeight );

	//Destination rectangle of the text
    SDL_Rect initialChRect =
    { 
         screen->w/2  + xPadding -  ( cfont->size.w + expansion * (textWidth - 1) ) / 2 //X
       , screen->h/2  + yPadding -  ( cfont->size.h + (expansion * XYratio) * (textHeight - 1) ) / 2 //Y
       , 0 //W
       , 0 //H
    };

	SDL_Rect dstRectFont = initialChRect;

	//Character tile in the bitmap
	SDL_Rect chTile = cfont->size;
    
	unsigned int a , chrTile;

	for(a= 0 ; a < strlen(str) ; a++)
	{
		// new line
		if(*(str+a) == '\n')
		{
			dstRectFont.y += expFont.hSpacing;
			dstRectFont.x = initialChRect.x ;
			continue;
		}

		chrTile= getCharTileNumber(*(str+a));

		//Get the tilemap coords of the character
		//tile
		chTile.x= (chrTile % expFont.chrPerLine) * expFont.size.w;
		chTile.y= (chrTile / expFont.chrPerLine) * expFont.size.h;

		//Printf it
		SDL_BlitSurface(cfont->gfx,&chTile,screen,&dstRectFont);

		//Add space between characters
		dstRectFont.x+= expFont.wSpacing;
	}
}


//------------------------------------------------

/********************************************
    SDL_Text
*********************************************/

void SDL_Text(char *str , struct bmFont *cfont, int x , int y , SDL_Surface *screen)
{
	//Destination rectangle of the text
	SDL_Rect dstRectFont = { x , y , 0 , 0};
    SDL_Rect chrPos;
	//Character tile in the bitmap
	SDL_Rect chTile = cfont->size;

	register unsigned int a , chrTile;

	for(a= 0 ; a < strlen(str) ; a++)
	{
		// new line
		if(*(str+a) == '\n')
		{
			dstRectFont.y += cfont->hSpacing;
			dstRectFont.x = x;
			continue;
		}

		chrTile= getCharTileNumber(*(str+a));

		//Get the tilemap coords of the character
		//tile
		chTile.x= (chrTile % cfont->chrPerLine) * cfont->size.w;
		chTile.y= (chrTile / cfont->chrPerLine) * cfont->size.h;

		//Printf it
        chrPos = dstRectFont;
		SDL_BlitSurface(cfont->gfx,&chTile,screen,&chrPos);

		//Add space between characters
		dstRectFont.x+= cfont->wSpacing;
	}
}
