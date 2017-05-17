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
   announcer.c
   Display messages on the screen
   (START , Bonus , Warning , etc...)
*********************************************/

#ifndef ANNOUNCER_C
#define ANNOUNCER_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_bmFont.h"
#include "announcer.h"
#include "d_advert.h"
#include "SDL_Text.h"
#include "loadData.h"
#include "l_sfx.h"
#include "sfx.h"

//-------------------------------------------

advert msg;         //Advertisement message
bmFont advertsFont; //Advertisement font

/********************************************
    loadAdvertsFont
*********************************************/

void loadAdvertsFont( char *fontDir)
{
    /*Free the previous contents if any*/
    if(!bmFontIsFree(&advertsFont))
    	freeBmFont(&advertsFont);
    /*Load the new font data*/
	loadBmFont(fontDir , &advertsFont);
	return;
}

//-------------------------------------------

/********************************************
    freeAdvertsFont
*********************************************/

void freeAdvertsFont(void)
{
	freeBmFont(&advertsFont);
	return;
}

//-------------------------------------------

/********************************************
    setAdvert
*********************************************/

void setAdvert( char *str , int x , int y , unsigned frames)
{
	setAdvertCustomFont( str , NULL , x , y , frames);
	return;
}

//-------------------------------------------

/********************************************
    setAdvertCustomFont
*********************************************/

void setAdvertCustomFont( char *str , bmFont * font, int x , int y , unsigned frames)
{
	if( font == NULL)
		font = &advertsFont;

	strncpy ( msg.str, str , ADVERT_STR_SIZE );
	msg.font = font;
	msg.x = x;
	msg.y = y;
	msg.frames = frames;

	return;
}

//-------------------------------------------

/*******************************************
    setSpecialBonusMsg
*******************************************/

#define SPECIAL_BONUS_MSG "SPECIAL BONUS"

void setSpecialBonusMsg(int bonus)
{
        int msgX; //Message X position

        msgX = GM_SCREEN_W/2 
               - ((sizeof(SPECIAL_BONUS_MSG)-1)/2) * getFontHSpacing(&advertsFont);

        sprintf( msg.str , SPECIAL_BONUS_MSG "\n   %4d" ,bonus);

		setAdvert(  msg.str 
                  , msgX    //X
                  , 50      //Y
                  , 80      //MSG FRAMES
                 );

        playAnnouncement(SFX_SPECIALBONUS);

		return;
}

//-------------------------------------------

/*******************************************
    setBonusMsg
*******************************************/

#define BONUS_MSG_STR_SIZE 40

void setBonusMsg(int bonus)
{
        //playAnnouncement(SFX_BOSSWARNING);
        char msg[BONUS_MSG_STR_SIZE];
        sprintf(msg,"BONUS %d",bonus);
		setAdvert( msg, 120 , 50 , 60);
		return;
}

//-------------------------------------------

/*******************************************
    setWarningMsg
*******************************************/

void setWarningMsg(void)
{
		setAdvert(  "WARNING!"
                  , 128
                  , 80
                  , 80 );
        playAnnouncement(SFX_WARNING);
		return;
}

//-------------------------------------------

/*******************************************
    setBossWarningMsg
*******************************************/

void setBossWarningMsg(void)
{
        playAnnouncement(SFX_BOSSWARNING);
		setAdvert( "           WARNING!\n"
		   "A HUGE BATTLE SHIP IS APPROACHING", 40 , 50 , 120);

		return;
}

//-------------------------------------------

/********************************************
    setAdvertTime
*********************************************/

void setAdvertTime( unsigned frames)
{
	msg.frames = frames;
	return;
}

//-------------------------------------------

/********************************************
    announcer
*********************************************/

void announcer(SDL_Surface *screen)
{
	if(msg.frames)
	{
		SDL_Text( msg.str , msg.font , msg.x , msg.y , screen);
		msg.frames--;
	}
	return;
}
//-------------------------------------------

