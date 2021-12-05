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
    staffRoll.h
    Staff Roll / Credits
********************************************/

#ifndef STAFF_ROLL_C
#define STAFF_ROLL_C
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

//#define STAFF_ROLL_BG_PATH   "data/gfx/menu/staffRoll.bmp"
#define STAFF_ROLL_FONT_PATH "data/gfx/fonts/staffRoll.font"
static char *staffRollText[] =
{
    #include "staffRollText.inc"
};

int staffRoll(void)
{
    static const float scrollSpeedY = 0.4;
    //
	SDL_Surface *gameScreen = getGameFb();
    //SDL_Surface *background = loadIMG( STAFF_ROLL_BG_PATH, 0);
    SDL_Rect bgRect = { 0 , 0 , gameScreen->w, gameScreen->h };

	bmFont textFont;
	loadBmFont(STAFF_ROLL_FONT_PATH , &textFont);
    int textLines =  sizeof(staffRollText)/sizeof(char *);
    int textHeight = getFontHSpacing(&textFont) * textLines;
    int fontHeight = getFontHSpacing(&textFont);

    float scrollY = bgRect.y;
    unsigned cLine = 0; //Current text line
    int linePosY = 0;
    float lineOffset = 0.0;
    float textSpeed = 0.2;

	int mspf = 1000/FPS; //milisecond per frame
	int ms = 0;
	int sleep_delay = 0;
	int menu_status = 0;

	SDL_Event event;
	controllerStruct controller;
	initController(&controller);

	while(1)
	{
		//Frame counter
		ms = SDL_GetTicks();

        readController(&controller);
        if(controller.pressed.START)
            break;

        clearGameFb();
        //blitToGameFb( background , &bgRect , NULL);

        int i;
        for( i= cLine, linePosY =  -lineOffset;
             linePosY < gameScreen->h && i < textLines;
             i++, linePosY+=fontHeight )
        {
            SDL_Text(staffRollText[i] , &textFont, 0, linePosY, gameScreen);
        }

		updateVideoScreen();

        //Image scroll logic
        /*
        if( background->h - scrollY > gameScreen->h)
        {
            scrollY += scrollSpeedY;
            bgRect.y = scrollY;
        }
        */
        //Text scroll logic
        if( (textLines-cLine)*fontHeight - lineOffset > gameScreen->h)
        {
            lineOffset += textSpeed;
            if( ((int)lineOffset)/fontHeight)
            {
                lineOffset -= fontHeight;
                cLine++;
            }
        }

		/*sleep_delay = mspf - (SDL_GetTicks()-ms);
		if(sleep_delay > 0)
			SDL_Delay( sleep_delay );*/
    }
    //SDL_FreeSurface(background);
    freeBmFont(&textFont);
}
