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
    nameInput.c
    Name Input screen
*********************************************/

#ifndef NAME_INPUT_C
#define NAME_INPUT_C
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

#define NAME_INPUT_FPS 60
#define STARS_NUMBER 100

#define RETURN(x)\
                    freeBmFont(&msgFont);\
                    freeBmFont(&inputFont);\
                    return x;

#define STR_BUFFER_SIZE 100
#define NAME_LENGTH 4

#define FIRST_CHAR_VALUE 32 
#define LAST_CHAR_VALUE 126

//Menu status
enum {
        READING_NAME = 0
        ,INPUT_DONE
     };


/*********************************************
    playerNameInput
**********************************************/

int playerNameInput( unsigned char *name , unsigned *nameChr , joypadStruct *controller)
{
        if(controller->LEFT)
        {
            if('\0' != name[(*nameChr-1) % NAME_LENGTH ])
            *nameChr = --(*nameChr) % NAME_LENGTH;
        }
        else if(controller->RIGHT)
        {
            if('\0' != name[*nameChr])
            *nameChr = ++(*nameChr) % NAME_LENGTH; 
        }
        else if(controller->UP)
        {
            if(name[*nameChr] == '\0')
                name[*nameChr] =  '@';

            name[*nameChr]++;
            if( name[*nameChr] > LAST_CHAR_VALUE)
                name[*nameChr] =  FIRST_CHAR_VALUE;
        }
        else if(controller->DOWN)
        {
            if(name[*nameChr] == '\0')
                name[*nameChr] =  '@';

            name[*nameChr]--;

            if( name[*nameChr] < FIRST_CHAR_VALUE)
                name[*nameChr] =  LAST_CHAR_VALUE;
        }
        else if(controller->START)
        {   //Add the score to the table and end
            if(name[0] != '\0')
                return INPUT_DONE;
        }

        return READING_NAME;
}

//-------------------------------------------

/*********************************************
    nameInputScreen:
    Screen where the player enters his/her name
    for the records!
**********************************************/

int nameInputScreen(int score)
{
#ifdef DEBUG
		printf("[DEBUG] getVideoScreen sc\n");
#endif
    SDL_Surface *screen = getVideoScreen();
    SDL_Surface *gameFb = getGameFb();
    
#ifdef DEBUG
		printf("[DEBUG] loadBmFont sc\n");
#endif
    
    //
    bmFont msgFont;
    loadBmFont( ADVERT_FONT , &msgFont);
    //
    bmFont inputFont;
	loadBmFont( SMALL_FONT , &inputFont);
    //
    int rank = rankScore( score , whichScoreBoard(gameType));

#ifdef DEBUG
		printf("[DEBUG] rankScore sc\n");
#endif

    char rankText[STR_BUFFER_SIZE];
    sprintf( rankText , "  YOUR RANK IS %2dTH\n\n\n%s"
                      , rank
                      , rank == 1 ? "   CONGRATULATIONS\n ON YOUR HIGH SCORE!" : "\0");

    char inputPrompt[STR_BUFFER_SIZE] = "ENTER YOUR NAME:";

    unsigned char name[NAME_LENGTH+1] = "\0\0\0\0" "\0";
    unsigned char cursorStr[NAME_LENGTH+1] = "____" "\0";
    unsigned  nameChr = 0;
    
    controllerStruct controller;
    
#ifdef DEBUG
		printf("[DEBUG] initController sc\n");
#endif
	initController(&controller);

    //
    int i;
	int menu_status = 0;

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

#ifdef DEBUG
		printf("[DEBUG] SDL_FillRect sc\n");
#endif

    while(READING_NAME == menu_status)
	{
		//Frame counter
		Start_Sleep_Delay();

		//Read controller input
		readController(&controller);
        
        //Player name input
        menu_status = playerNameInput( name , &nameChr , &controller.pressed);


        //Clean screen
        SDL_FillRect( gameFb , 0, SDL_MapRGB( gameFb->format, 0, 0, 0));

		// Crashes Dreamcast. Why ? No idea
#ifndef DREAMCAST
        //Draw starfield
		updateStarfield(starsGfx , gameFb , stars , STARS_NUMBER);
#endif
        //Print Text
        SDL_Text( rankText    , &msgFont   , 80 , 20 , gameFb);
        SDL_Text( inputPrompt , &inputFont , 80 , 150 , gameFb);
        SDL_Text( name , &inputFont , 140 , 170 , gameFb);
        memset(cursorStr , ' ' , 4);
        cursorStr[nameChr] = '_';
        SDL_Text( cursorStr , &inputFont , 140 , 175 , gameFb);

        updateVideoScreen();
        //
		Sleep_Delay( );
	}

    //Remove NULs ( except for the string terminator NUL!)
    for( i = 0; i < NAME_LENGTH ; i++)
        name[i] = (name[i] == '\0' ? ' ' : name[i]);

    //Save score on the scoreBoard
    addScore( name , score , whichScoreBoard(gameType));
    saveScoreboard(whichScoreBoard(gameType));

    RETURN(0)
}
