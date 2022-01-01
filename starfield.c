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
    starfield.c
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_star.h"

/*******************************************
 Init star
********************************************/

void initStar( star* star)
{
		star->x = rand()%GM_SCREEN_W;
		star->y = rand()%GM_SCREEN_H;
		star->tile = 0;
}

//-------------------------------------------


/*******************************************
 initStars
********************************************/
void initStars( star* stars , int num)
{
	uint_fast32_t i = num;
	for( ; i < num ; i++)
		initStar(stars+i);
}

//-------------------------------------------

/*******************************************
 updateStars
********************************************/

void updateStar( star* star)
{
        //If the star is out of the GM_SCREEN , give it a new position
		if(   star->x < -STAR_SIZE
		   || star->x > GM_SCREEN_W + STAR_SIZE
		   || star->y < -STAR_SIZE
		   || star->y > GM_SCREEN_H + STAR_SIZE)
		{
			initStar(star);
			return;
		}

        //Update star position
		double dx = star->x - GM_SCREEN_X_CENTER;
		double dy = star->y - GM_SCREEN_Y_CENTER;
		star->x += STAR_VEL *  dx;
		star->y += STAR_VEL *  dy;
}

//-------------------------------------------

/*******************************************
 drawStar
********************************************/

void drawStar(SDL_Surface *starGfx , star* star , SDL_Surface *GM_SCREEN)
{
	static SDL_Rect destRect =
	{
		 0,0
		,STAR_SIZE
		,STAR_SIZE
	};

	static SDL_Rect starTile =
	{
		 0,0
		,STAR_SIZE
		,STAR_SIZE
	};

	destRect.x = star->x - STAR_CENTER;
	destRect.y = star->y - STAR_CENTER;
	starTile.x = star->tile * STAR_SIZE;

	SDL_BlitSurface(starGfx , &starTile , GM_SCREEN , &destRect);
}

//-------------------------------------------

int updateStarfield(SDL_Surface *starsGfx , SDL_Surface *GM_SCREEN , star* stars , int num)
{
	uint_fast32_t i= 0;
	for( ; i < num ; i++)
	{
		updateStar( stars+i);
		drawStar(starsGfx , stars+i , GM_SCREEN);
	}
}
