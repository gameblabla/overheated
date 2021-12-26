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
   musicTest.c
*********************************************/


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
#include "audio.h"
#include "music.h"
#include "input.h"
#include "SDL_Text.h"
#include "loadData.h"
#include "trackPlayer.h"
#include "sleep_delay.h"

#define STR_BUFFER_MAX_SIZE 50

#define RETURN(x)\
		SDL_FreeSurface(screenBuffer);\
		freeBmFont(&menuFont);\
		return x;


int musicTest(SDL_Surface *screen)
{
	//Screen buffer
	SDL_Surface *screenBuffer  =  SDL_CreateRGBSurface( SDL_SWSURFACE, 
							GM_SCREEN_W, GM_SCREEN_H , SCREEN_BPP, 
							0, 0, 0, 0);

	bmFont menuFont;
	loadBmFont(DREAMCAST_CD_PATH"data/gfx/fonts/cell_phone.font" , &menuFont);

	int zoom = DEFAULT_ZOOM;
	int menu_status = 0;

	unsigned select = 0;
	char strBuffer[STR_BUFFER_MAX_SIZE];

	SDL_Event event;
	controllerStruct controller;
	initController(&controller);

    track_t *testTrack = loadTrack(DREAMCAST_CD_PATH"data/bgm/testTrack.track");
    printfTrackInfo(testTrack);
    //testTrack->s_loopCounter = 0;
    playTrack(testTrack);

    long playMs = 0;
    long lastTicks = SDL_GetTicks();
    int ejected = 0;
    int pause = 0;

	while(!menu_status)
	{
		//Frame counter
		Start_Sleep_Delay();

		//Read controller input
        readController(&controller);

		if(controller.pressed.LEFT)
		{
            if( ejected == 0)
            {
                ejectTrack();
                printf("[EJECT] Track time: %d\n", testTrack->sectionTicks/1000);
                ejected = 1;
            }
            else
            {
                ejected = 0;
                playTrack(testTrack);
                printf("[PLAY TRACK] Track time: %d\n", testTrack->sectionTicks/1000);
            }
		}
		else if(controller.pressed.RIGHT)
		{

		}
		else if(controller.pressed.START)
		{
            ;
		}
        //Clean surface
        SDL_FillRect(screenBuffer, 0, SDL_MapRGB(screenBuffer->format, 0, 0, 0));
        //
       
        if( pause == 0)
        {
            playMs += (SDL_GetTicks() - lastTicks);
            sprintf(strBuffer,"play time: %lds , Section Pos: %lds",playMs/1000 , testTrack->sectionTicks/1000);
            SDL_Text(strBuffer , &menuFont, 0 , 50 , screenBuffer);
        }
        lastTicks = SDL_GetTicks();
        //
        sprintf(strBuffer,"track loops:%d\nsection loops:%d\n"
                         ,testTrack->t_loopCounter,testTrack->s_loopCounter);
        SDL_Text(strBuffer , &menuFont, 0 , 20 , screenBuffer);

		if(zoom)
			SDL_zoom( screenBuffer , screen , NULL , zoom);
		else
			SDL_BlitSurface( screenBuffer , NULL , screen , NULL);

		SDL_Flip(screen);// update the screen
		//SDL_UpdateRect(screen,0,0,0,0);

		Sleep_Delay( );
	}

	RETURN(0)

    
    return 0;
}
