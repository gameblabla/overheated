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
    service.c
    Game options service menu
********************************************/

#ifndef SERVICE_C
#define SERVICE_C
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
#include "video.h"
#include "audio.h"
#include "music.h"
#include "input.h"
#include "SDL_Text.h"
#include "loadData.h"
#include "scoreboard.h"
#include "gameConfig.h"
#include "trackPlayer.h"
#include "sleep_delay.h"
//-------------------------------------------

#define STR_BUFFER_SIZE 1024
#define MENU_FONT_W 10
#define OPT_X_START 60
#define OPT_Y_START 60
#define OPT_Y_SPACING 20

#define CURSOR_W 16
#define CURSOR_H 16

#define RETURN(x)\
    SDL_FreeSurface(screenBuffer);\
    SDL_FreeSurface(cursorGfx);\
    SDL_FreeSurface(menuBgGfx);\
    freeBmFont(&optionsFont);\
    return x;\

#define CENTER_OPTION_X(x)\
    GM_SCREEN_W / 2 - strlen(x) * MENU_FONT_W / 2


//
#define OPT_CURSOR_GFX_PATH DREAMCAST_CD_PATH"data/gfx/menu/opt_cursor.bmp"
#define MENU_BG_GFX_PATH DREAMCAST_CD_PATH"data/gfx/menu/service_menu_bg.bmp"

//Menu status

#define EXIT_MENU -1

//MENU STRINGS
#define MENU_HEADER "~Service menu~"

char *yesNoStrConst[] =
{
     "NO"
    ,"YES"
};

//-------------------------------------------
//Menu Options
//-------------------------------------------

//Numerical codes
enum ServiceOptions
{ 
      OPTION_MUS_VOL= 0
    , OPTION_SFX_VOL
#ifdef PC
    , OPTION_FULLSCREEN
    , OPTION_RESOLUTION
    , OPTION_SCANLINES
    , OPTION_SL_VALUE
#endif
    , OPTION_RETURN
    , OPTIONS_NUMBER //Number of options available
};

//Options text strings
char *optText[OPTIONS_NUMBER] =
{
    "MUS Volume"
   ,"SFX Volume"
#ifdef PC
   ,"FULLSCREEN"
   ,"RESOLUTION"
   ,"SCANLINES"
   ,"SL VALUE:"
#endif
   ,"RETURN"
};

//Option info text
char *optInfText[OPTIONS_NUMBER]=
{
     "\0"
    ,"\0"
#ifdef PC
    ,"Reset to apply this option"
    ,"Reset to apply this option"
    ,"Reset to apply this option"
    ,"Scanlines Intensity"
#endif
    ,"\0"
};

//-------------------------------------------

/********************************************
    serviceMenu
*********************************************/

int serviceMenu(void)
{

    SDL_Surface *screen = getVideoScreen();
    SDL_Surface *gameFb = getGameFb();

    char strBuffer[STR_BUFFER_SIZE] = "\0";

	//Screen buffer
	SDL_Surface *screenBuffer  =  SDL_CreateRGBSurface( SDL_SWSURFACE, 
							GM_SCREEN_W, GM_SCREEN_H , SCREEN_BPP, 
							0, 0, 0, 0);

    //Cursor sprite

    SDL_Surface *cursorGfx = loadIMG(OPT_CURSOR_GFX_PATH , OPTIMIZE_IMG);
    SDL_SetColorKey(   cursorGfx
                     , SDL_SRCCOLORKEY
                     , SDL_MapRGB( cursorGfx->format, 255,0,255));


    /******************************
    /* Cursor
    /*****************************/

    SDL_Rect cursorRect =
    {
          OPT_X_START - CURSOR_W*2
        , OPT_Y_START
        , CURSOR_W
        , CURSOR_H
    };
    
    typedef struct 
    {
        const int frames;       //Numer of animation frames
              int cframe;       //Current frame
        const int delay;        //Animation delay
              int dcounter;     //Anim delay counter
    }cursorAnim_t;

    cursorAnim_t cursorAnim = { 2 , 0 , 30 , 30 };
    cursorAnim.dcounter = cursorAnim.delay;

    SDL_Rect cursorAnimFrames[] =
    {
         {        0 , 0 , CURSOR_W , CURSOR_H } //FRAME 1
        ,{CURSOR_W  , 0 , CURSOR_W , CURSOR_H } //FRAME 2
    };
    
    SDL_Rect cursorSprite = cursorAnimFrames[0];

    /******************************
    /* Service menu background
    /*****************************/

    SDL_Surface *menuBgGfx = loadIMG(MENU_BG_GFX_PATH , OPTIMIZE_IMG);

    //Load text font
    bmFont optionsFont;
	loadBmFont(DREAMCAST_CD_PATH"data/gfx/fonts/cell_phone.font" , &optionsFont);

    //Input variables init
	controllerStruct controller;
	initController(&controller);

    //
	int menu_status = 0;

    int optSelection = 0; //Option selection


    while( menu_status != EXIT_MENU)
	{
        //Frame counter
		Start_Sleep_Delay();


        /******************************
        /* Manage Input
        /*****************************/

		//Read controller input
        readController(&controller);

        if (controller.pressed.DOWN)
        {
            optSelection++;
            optSelection %= OPTIONS_NUMBER;
        }
        else if(controller.pressed.UP)
        {
            optSelection--;
            if( optSelection < 0)
                optSelection = OPTIONS_NUMBER-1;
        }

        switch ( optSelection )
        {
            //Change music volume
            case OPTION_MUS_VOL:
#ifdef DREAMCAST
                if(controller.pressed.LEFT) incMusicVolume(-15);
                else if(controller.pressed.RIGHT)incMusicVolume(15);
#else
                if(controller.pressed.LEFT) incMusicVolume(-1);
                else if(controller.pressed.RIGHT) incMusicVolume(1);
#endif
            break;

            //Change sfx volume
            case OPTION_SFX_VOL:

                if(controller.pressed.LEFT)
                    decSFX_Volume();
                
                else if(controller.pressed.RIGHT)
                    incSFX_Volume();

            break;

#ifdef PC
            //Fullscreen option
            case OPTION_FULLSCREEN:

                if(    controller.pressed.LEFT
                    || controller.pressed.RIGHT)
                {
                    if(getFullscreenFlag())
                        resetFullscreenFlag();
                    else
                        setFullscreenFlag();
                }

            break;
            //Resolution option
            case OPTION_RESOLUTION:

                if(controller.pressed.LEFT)
                   decResMode();
                
                else if(controller.pressed.RIGHT)
                   incResMode();


            break;

            //Fullscreen option
            case OPTION_SCANLINES:

                if(    controller.pressed.LEFT
                    || controller.pressed.RIGHT)
                {
                    if(getScanlinesFlag())
                        resetScanlinesFlag();
                    else
                        setScanlinesFlag();
                }

            break;

            //Scanlines value
            case OPTION_SL_VALUE:

                if(controller.pressed.LEFT)
                    decScanlines();
                
                else if(controller.pressed.RIGHT)
                    incScanlines();

            break;
#endif                 
            
            //Return to main menu
            case OPTION_RETURN:

                if(   controller.pressed.START
                    || controller.pressed.A )
                {
                    //Update and save game config
                    saveGameCfg();
                    RETURN(0);
                }

            break;

        }

        /*****************************/

        //Draw menu background
        blitToGameFb( menuBgGfx, NULL , NULL);

        //Cursor animation
        cursorAnim.dcounter--;
        if (cursorAnim.dcounter <= 0 )
        {
            cursorAnim.dcounter = cursorAnim.delay;
            cursorAnim.cframe++;
            cursorAnim.cframe %= cursorAnim.frames;
            cursorSprite = cursorAnimFrames[cursorAnim.cframe];
        }

        //Draw cursor
        cursorRect.y = OPT_Y_START + ( optSelection * OPT_Y_SPACING );
        blitToGameFb( cursorGfx, &cursorSprite , &cursorRect);

        //Draw menu header
        SDL_Text(  MENU_HEADER
                 , &optionsFont
                 , CENTER_OPTION_X(MENU_HEADER)
                 , 10
                 , gameFb);

        //Option info/help text

        SDL_Text(  optInfText[optSelection]
                 , &optionsFont
                 , CENTER_OPTION_X(optInfText[optSelection])
                 , 30
                 , gameFb);

        /******************************
        /* Draw menu options
        /*****************************/

        //Music Volume
        sprintf(strBuffer, "Music Volume  %02d" , getMusicVolume() );
        SDL_Text(  strBuffer , &optionsFont 
                 , OPT_X_START , OPT_Y_START+ (OPTION_MUS_VOL*OPT_Y_SPACING)   ,gameFb);

        //SFX Volume
        sprintf(strBuffer, "SFX Volume    %02d" , getSFX_Volume() );
        SDL_Text(  strBuffer , &optionsFont 
                 , OPT_X_START , OPT_Y_START+ (OPTION_SFX_VOL*OPT_Y_SPACING)  , gameFb);
#ifdef PC
        //Full screen option
        sprintf(strBuffer, "FULLSCREEN    %s" , yesNoStrConst[getFullscreenFlag()] );
        SDL_Text(  strBuffer , &optionsFont 
                 , OPT_X_START , OPT_Y_START+ (OPTION_FULLSCREEN * OPT_Y_SPACING)  , gameFb);

        //Screen resolution
        sprintf(strBuffer, "RESOLUTION    %s" , getResModesStrInfo() );
        SDL_Text(  strBuffer , &optionsFont 
                 , OPT_X_START , OPT_Y_START+ (OPTION_RESOLUTION * OPT_Y_SPACING)  , gameFb);

        //Full screen option
        sprintf(strBuffer, "SCANLINES     %s" , yesNoStrConst[getScanlinesFlag()] );
        SDL_Text(  strBuffer , &optionsFont 
                 , OPT_X_START , OPT_Y_START+ (OPTION_SCANLINES * OPT_Y_SPACING)  , gameFb);

        //Screen resolution
        sprintf(strBuffer, "SL Value      %02d" , getVideoScanLines() );
        SDL_Text(  strBuffer , &optionsFont 
                 , OPT_X_START , OPT_Y_START+ (OPTION_SL_VALUE * OPT_Y_SPACING)  , gameFb);
#endif

        //Return option
        SDL_Text(  optText[OPTION_RETURN] , &optionsFont
                 , OPT_X_START , OPT_Y_START+ ( OPTION_RETURN * OPT_Y_SPACING) , gameFb);

        /*****************************/

		updateVideoScreen();

		Sleep_Delay( );
    }

    RETURN(0);
}

//-------------------------------------------
