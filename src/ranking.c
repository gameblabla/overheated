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
    ranking.c
    Ranking screen
*********************************************/

#ifndef RANKING_C
#define RANKING_C
#endif
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
#include "d_star.h"
#include "globalVars.h"
//
#include "video.h"
#include "audio.h"
#include "music.h"
#include "input.h"
#include "SDL_Text.h"
#include "loadData.h"
#include "scoreboard.h"
#include "starfield.h"
#include "sleep_delay.h"


#define RETURN(x)\
    freeBmFont(&rankingFont);\
    return x;\

#define STR_BUFFER_SIZE 1024
#define STARS_NUMBER 100
#define RANKING_FONT_PATH SMALL_FONT 

//
#define MAX_EXPANSION  10.0
#define MIN_COMPRESSION 0
#define EXPANSION_RATE  0.5
#define BOARD_NAME_SIZE 20
//Menu status
enum {
         EXIT_MENU=-1
        ,CHART_FULLY_EXPANDED= 1
        ,CHART_EXPANDING
        ,CHART_COMPRESSING
        ,CHART_COMPRESSED
     };
//
char boardNames[SCOREBOARDS_NUMBER][BOARD_NAME_SIZE] =
{
      "2Min mode"
    , "5Min mode"
};

int rankingScreen(void)
{
    SDL_Surface *screen = getVideoScreen();
    SDL_Surface *gameFb = getGameFb();

    char chartStrBuffer[STR_BUFFER_SIZE] = "\0";
    char strBuffer[STR_BUFFER_SIZE] = "\0";

    //
    bmFont rankingFont;
	loadBmFont( RANKING_FONT_PATH , &rankingFont);

    //
	SDL_Event event;
	controllerStruct controller;
	initController(&controller);

	int menu_status = 0;

	/* For some reasons this crashes the console */
	#ifndef DREAMCAST
    //Star field stars
    star stars[STARS_NUMBER];
	initStars( stars , STARS_NUMBER);

    //Create the stars surface
    SDL_Surface *starsGfx = SDL_CreateRGBSurface( SDL_SWSURFACE , STAR_SIZE , STAR_SIZE
                                                 ,SCREEN_BPP , 0 , 0 , 0 , 0);
    //Fill with white pixels
    SDL_FillRect(starsGfx , NULL , SDL_MapRGB( starsGfx->format , 0xFF , 0xFF , 0xFF ));
    #endif

    //Print the contents of the score board on a string
    int boardNumber = SCOREBOARD_2MIN;
    printScoreboard( chartStrBuffer , STR_BUFFER_SIZE , boardNumber );

    //Set initial expansion value and menu status
    double expansion = 0;
    menu_status = CHART_EXPANDING;
    //
	while( menu_status != EXIT_MENU)
	{
		//Frame counter
		Start_Sleep_Delay();

		//Read controller input
        readController(&controller);

        if(    (controller.pressed.START || controller.pressed.A)
            && CHART_FULLY_EXPANDED == menu_status )
        {
            menu_status = CHART_COMPRESSING;
        }

        //Clean screen
        SDL_FillRect(gameFb, 0, SDL_MapRGB(gameFb->format, 0, 0, 0));

		#ifndef DREAMCAST
        //Draw starfield
		updateStarfield(starsGfx , gameFb , stars , STARS_NUMBER);
		#endif

        //Menu state processing
        /*Chart Expansion*/
        if( CHART_EXPANDING == menu_status)
        { 
            if( expansion < MAX_EXPANSION)
                expansion += EXPANSION_RATE;
            else
            {
                menu_status = CHART_FULLY_EXPANDED;
                expansion = MAX_EXPANSION ;    
            }
        }

        /*Chart Compression*/
        else if( CHART_COMPRESSING == menu_status)
        { 
            if( expansion > 0)
            {
                expansion -= EXPANSION_RATE;
            }
            else
            {
                /*Char Compression has ended*/
                expansion = 0; 
                menu_status = CHART_EXPANDING;

                if( boardNumber < SCOREBOARDS_NUMBER-1)
                {
                    /*Change to the next scoreboard*/
                    boardNumber++;
                    printScoreboard( chartStrBuffer , STR_BUFFER_SIZE , boardNumber );                    
                }
                else 
                { 
                    /*Exit the menu*/
                    menu_status = EXIT_MENU;
                }
            }
        }/*END Chart compression*/


        //Draw text
        sprintf( strBuffer , "Heroes of the past - %s" , boardNames[boardNumber]);
        SDL_Text( strBuffer , &rankingFont, 10 , 10 , gameFb);
        SDL_TextExpansion (chartStrBuffer , &rankingFont , 0 , 16 , expansion , gameFb);
        //
        updateVideoScreen();

        //
		Sleep_Delay( );
	}   

    RETURN(0)
}
