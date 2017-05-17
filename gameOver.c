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
    gameOver.c
    Game over screens
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_bmFont.h"
#include "globalVars.h"
#include "score.h"
#include "scoreboard.h"
//
#include "SDL_Text.h"

#define STR_BUFFER_SIZE 100

#define SLASH_ANIM_CHARS 4
#define SLASH_ANIM_FPC 5 // Frames per CHARacter
#define SLASH_ANIM_FRAMES (SLASH_ANIM_CHARS * SLASH_ANIM_FPC )
int slashAnimFrame = 0;
char slashAnimChars[] =
{
    '\\',
    '-',
    '/',
    '|'
};


int gameOverScreen(char *header , bmFont *font , SDL_Surface *screen)
{
	static char strBuffer[STR_BUFFER_SIZE];
	int x,y = 50;
    slashAnimFrame = ++slashAnimFrame % SLASH_ANIM_FRAMES; /*Update animation counter*/
    char *cSlashAnimChar = slashAnimChars + (slashAnimFrame / SLASH_ANIM_FPC); /*Current slash animation character*/

    //Draw title
	sprintf(strBuffer,"%c%s%c", *cSlashAnimChar , header , *cSlashAnimChar);
	x = GM_SCREEN_W/2 - strlen(strBuffer)*font->wSpacing/2;
	SDL_Text(strBuffer , font , x , y , screen);
	y+= font->hSpacing*2;

    //Draw score
	sprintf(strBuffer,"HIGH SCORE:    %06d\n"
			          "YOUR SCORE:    %06d\n",getHighScore(),getScore());
	

	SDL_Text(strBuffer , font , 90 , y , screen);
	y+= font->hSpacing*3;

    //DRAW RANK
    int rank = rankScore( getScore() , whichScoreBoard(gameType) );

    if(rank)
    {
        sprintf(strBuffer,"YOUR RANK IS %2dTH",rank);
    }
    else
    {
        sprintf(strBuffer,"YOU ARE OUT OF RANK");
    }

	x = GM_SCREEN_W/2 - strlen(strBuffer)*font->wSpacing/2;
	SDL_Text(strBuffer , font , x , y , screen);
    y += font->hSpacing*2;

    //HIGH SCORE CONGRATULATION!
    if(rank == 1)
    {
        sprintf(strBuffer,"CONGRATULATIONS!");
        x = GM_SCREEN_W/2 - strlen(strBuffer)*font->wSpacing/2;
	    SDL_Text(strBuffer , font , x , y , screen);
    }

	return 0;
}

int missionCompleteScreen( bmFont *font , SDL_Surface *screen)
{
	static char strBuffer[STR_BUFFER_SIZE];
	int x,y = 50;
    slashAnimFrame = ++slashAnimFrame % SLASH_ANIM_FRAMES; /*Update animation counter*/
    char *cSlashAnimChar = slashAnimChars + (slashAnimFrame / SLASH_ANIM_FPC); /*Current slash animation character*/

    //Draw title
	sprintf(strBuffer,"%cMISSION COMPLETE!%c", *cSlashAnimChar , *cSlashAnimChar);
	x = GM_SCREEN_W/2 - strlen(strBuffer)*font->wSpacing/2;
	SDL_Text(strBuffer , font , x , y , screen);
	y+= font->hSpacing*2;

    //Draw score
	sprintf(strBuffer,"HIGH SCORE:    %06d\n"
			          "YOUR SCORE:    %06d\n",getHighScore(),getScore());
	
	SDL_Text(strBuffer , font , 90 , y , screen);
	y+= font->hSpacing*3;

     //DRAW RANK
    int rank = rankScore( getScore() , whichScoreBoard(gameType) );

    if(rank)
    {
        sprintf(strBuffer,"YOUR RANK IS %2dTH",rank);
    }
    else
    {
        sprintf(strBuffer,"YOU ARE OUT OF RANK");
    }

	x = GM_SCREEN_W/2 - strlen(strBuffer)*font->wSpacing/2;
	SDL_Text(strBuffer , font , x , y , screen);
    y += font->hSpacing*2;

    //HIGH SCORE CONGRATULATION!
    if(rank == 1)
    {
        sprintf(strBuffer,"CONGRATULATIONS!");
        x = GM_SCREEN_W/2 - strlen(strBuffer)*font->wSpacing/2;
	    SDL_Text(strBuffer , font , x , y , screen);
    }

    return 0;
}

