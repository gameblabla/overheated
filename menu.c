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
 menu.c
********************************************/

#define MENU_C
//
#include <stdio.h>
#include <stdlib.h>
#include <cMath.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "SDL_utils.h"
#include "d_bmFont.h"
#include "d_input.h"
#include "globalVars.h"
//
#include "video.h"
#include "audio.h"
#include "music.h"
#include "l_music.h"
#include "input.h"
#include "SDL_Text.h"
#include "game.h"
#include "loadData.h"
#include "ranking.h"
#include "service.h"
#include "staffRoll.h"
#include "sleep_delay.h"

#define STRBUFFER_MAX_SIZE 50

#define DRAW_OPTION(x)\
    strcpy(strBuffer,optionsStr[x]);\
    optionX = GM_SCREEN_W/2-strlen(strBuffer)*menuFont.wSpacing/2;\
    SDL_Text(strBuffer , &menuFont, optionX , 204 , gameFb);

#define RETURN(x)\
    SDL_FreeSurface(bgBuffer);\
    freeBmFont(&menuFont);\
    return x;

enum {
         TWO_MIN_MODE = 0
        ,FIVE_MIN_MODE
        ,TWO_MIN_REPLAY
        ,FIVE_MIN_REPLAY
        ,SERVICE
        ,SCORE
        ,STAFF_ROLL
        ,EXIT_MODE
        ,OPTIONS_NUM
     };

char *optionsStr[]=
	{
		  "2 MINUTES MODE"
		, "5 MINUTES MODE"
        , "2 MINUTES REPLAY"
        , "5 MINUTES REPLAY"
		, "SERVICE"
		, "SCORE"
		, "STAFF ROLL"
		, "EXIT"
		, NULL
	};

int menu(void)
{
    SDL_Surface *screen = getVideoScreen();
    SDL_Surface *gameFb = getGameFb();
	//Menu Background buffer
	SDL_Surface *bgBuffer  =  SDL_CreateRGBSurface( SDL_SWSURFACE, 
							GM_SCREEN_W, GM_SCREEN_H , SCREEN_BPP, 
							0, 0, 0, 0);

	#ifdef DEBUG
	printf("[DEBUG] MENU()\n");
	#endif

	SDL_Surface *background = SDL_LoadBMP(DREAMCAST_CD_PATH"data/gfx/menu/background.bmp");
	if(!background)
	{
		fprintf(stderr,"[ERROR] loading menu background.bmp\n");
		exit(1);
	}
	
	#ifdef DEBUG
	printf("[DEBUG] BACKGROUND()\n");
	#endif

	SDL_BlitSurface(background , NULL , bgBuffer , NULL);
	SDL_FreeSurface(background);

	bmFont menuFont;
	#ifdef DEBUG
	printf("[DEBUG] loadBmFont()\n");
	#endif
	loadBmFont(DREAMCAST_CD_PATH"data/gfx/fonts/cell_phone.font" , &menuFont);

	int menu_status = 0;

	unsigned select = 0;
	char strBuffer[STRBUFFER_MAX_SIZE];

	SDL_Event event;
	controllerStruct controller;
	#ifdef DEBUG
	printf("[DEBUG] initController()\n");
	#endif
	initController(&controller);

	int optionX;
	int optionY;

    enum {DEMO_DELAY = 40 * FPS};
    long demoDelay = DEMO_DELAY;

	//MENU INTRODUCTION ANIMATION
	//(...)
	
	#ifdef DEBUG
	printf("[DEBUG] ABOUT TO PLAY TRACK\n");
	#endif
	
    playMusicTrack(INTRO_TRACK);
    
	#ifdef DEBUG
	printf("[DEBUG] TRACK SUCEEDED\n");
	#endif

	while(!menu_status)
	{   
		Start_Sleep_Delay();

        readController(&controller);

        if(noControllerInput(&controller))
        { /*NO INPUT*/
            if(demoDelay)
                demoDelay--;
            else
            {
                demoDelay = DEMO_DELAY;
                if(c_rand()%2)
                    startGame(TWO_MINUTES_REPLAY);
                else
                    startGame(FIVE_MINUTES_REPLAY);
                RETURN(0)            
            }        
        }
        else
        { /*INPUT*/
            demoDelay = DEMO_DELAY;
            if(controller.pressed.LEFT)
		    {
                if(select != 0)
                    select--;
            }
            else if(controller.pressed.RIGHT)
            {
                if(optionsStr[select+1] != NULL)
                    select++;
            }
		    else if(controller.pressed.START)
		    {
                switch(select)
			    {
				    case TWO_MIN_MODE:
                        startGame( TWO_MINUTES_MODE);
					break;
				    case FIVE_MIN_MODE:
					    startGame( FIVE_MINUTES_MODE);
                    break;
                    case TWO_MIN_REPLAY:
                         startGame( TWO_MINUTES_REPLAY);
					break;
                    case FIVE_MIN_REPLAY:
                         startGame( FIVE_MINUTES_REPLAY);
                    break;
                    case SCORE:
                         rankingScreen();
                    break;
                    case SERVICE:
                         serviceMenu();
                    break;
                    case STAFF_ROLL:
                         staffRoll();
                    break;
				    case EXIT_MODE:
					     exit(1);
			    }
                continue;
            }
        }
        
        blitToGameFb( bgBuffer , NULL , NULL);
        DRAW_OPTION(select);
		updateVideoScreen();

		Sleep_Delay();
	}

	RETURN(0)
}
