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
    intro.c
********************************************/

#define INTRO_C
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "SDL_utils.h"
#include "d_bmFont.h"
#include "d_input.h"
#include "globalVars.h"
//
#include "intro.h"
#include "video.h"
#include "audio.h"
#include "music.h"
#include "l_music.h"
#include "input.h"
#include "SDL_Text.h"
#include "SDL_utils.h"
#include "game.h"
#include "loadData.h"
#include "sleep_delay.h"

#define DDL_LOGO_GFX_PATH DREAMCAST_CD_PATH"data/gfx/menu/ddl_logo1.bmp"

/********************************************
intro
*********************************************/

void intro(void)
{
    DDL_logo();
    return;
}

/********************************************
 logoFade
*********************************************/

int logoFade(char *logoPath, SDL_Rect *srcrect, SDL_Rect *dstrect 
             , long unsigned tfadeIn
             , long unsigned tStill 
             , long unsigned tfadeOut)
{
	int loop_exit = 1;
	SDL_Surface *gameScreen = getGameFb();

    SDL_Surface *logo = loadIMG( logoPath , OPTIMIZE_IMG);
	SDL_Surface *bgBuffer  =  SDL_CreateRGBSurface( SDL_SWSURFACE, 
							GM_SCREEN_W, GM_SCREEN_H , SCREEN_BPP, 
							0, 0, 0, 0);
	SDL_BlitSurface(logo , srcrect , bgBuffer , dstrect);
	SDL_FreeSurface(logo);

    SDL_Surface *fadeMask = SDL_CreateRGBSurface( SDL_SWSURFACE|SDL_RLEACCEL|SDL_SRCALPHA, 
					   GM_SCREEN_W, GM_SCREEN_H , SCREEN_BPP, 
					   0, 0, 0, 0);
    SDL_SetAlpha( fadeMask , SDL_RLEACCEL | SDL_SRCALPHA , 0);

	int menu_status = 0;

    long fadeInFrames = tfadeIn/mspf;
    double  fadeInStep = SDL_ALPHA_OPAQUE/(double)fadeInFrames;
    double  fadeInValue  = SDL_ALPHA_OPAQUE;

    long fadeOutFrames = tfadeOut/mspf;
    double fadeOutStep = SDL_ALPHA_OPAQUE/(double)fadeOutFrames;
    double  fadeOutValue = SDL_ALPHA_TRANSPARENT;
    
    long stillFrames =  tStill/mspf;
    
	while(loop_exit)
	{
		//Frame counter
		Start_Sleep_Delay();

        blitToGameFb( bgBuffer , NULL , NULL);

        /*Fade logic*/
        if(fadeInValue > SDL_ALPHA_TRANSPARENT)
        {
            SDL_fadeSurface( fadeMask , fadeInValue , gameScreen);
            fadeInValue -= fadeInStep;
        }
        else if(stillFrames)
        {
            stillFrames--;
        }
        else if(fadeOutValue < SDL_ALPHA_OPAQUE)
        {
            SDL_fadeSurface( fadeMask , fadeOutValue , gameScreen);
            fadeOutValue += fadeOutStep;
        }
        else
        {
			loop_exit = 0;
		}

		updateVideoScreen();

		Sleep_Delay( );
    }

    SDL_FreeSurface(bgBuffer);
    SDL_FreeSurface(fadeMask);
}

/********************************************
DDL_logo
*********************************************/

void DDL_logo(void)
{
    SDL_Rect logoPos = { 100 , 50 , 0 , 0};

    logoFade(DDL_LOGO_GFX_PATH, NULL ,&logoPos
             ,500,1500,500);
    return;
}

