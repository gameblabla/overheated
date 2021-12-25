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
   video.c
   Video subsystem
*********************************************/

#ifndef VIDEO_C
#define VIDEO_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

//
#include "globalDefines.h"
#include "d_GameObject.h"
#include "d_video.h"
#include "SDL_utils.h"

//
#define VIDEO_ERR_MSG_SIZE 1024
char videoErrMsg[VIDEO_ERR_MSG_SIZE] = "\0";
videoConf_t videoConf = //Video configuration structure
{/*Default conf*/
     320 //X
    ,240 //Y
    ,2   //Scale factor
    ,0   //Fullscreen
    ,0   //Game area X
    ,0   //Game area Y
    ,0   //Scanlines
    ,0   //Scanlines Intensity
};
int videoEnabled = 0; //
SDL_Surface *screen = NULL; //Screen surface
SDL_Surface *gameFb = NULL; //Game Frame buffer
SDL_Surface *scanlinesMask = NULL;

/********************+++++***+******************
    videoEnableScanlines
************************************************/

void videoEnableScanlines(void)
{
    videoConf.scanlines = 1;
    return;
}

/***********************************************
    videoDisableScanlines
************************************************/

void videoDisableScanlines(void)
{
    videoConf.scanlines = 0;
    return;
}

/***********************************************
    getVideoScanLines
************************************************/

int getVideoScanLines(void)
{
    return videoConf.slValue;
}

/***********************************************
    setVideoScanLines
************************************************/

void setVideoScanLines(unsigned value)
{
    videoConf.slValue = value % (MAX_SC_VALUE+1);
    return;
}

/***********************************************
    incScanlines
************************************************/
void incScanlines(void)
{
    videoConf.slValue++;
    setVideoScanLines(videoConf.slValue);

    return;
}

/***********************************************
    decScanlines
************************************************/
void decScanlines(void)
{
    videoConf.slValue--;
    setVideoScanLines(videoConf.slValue);

    return;
}


/***********************************************
    setVideoFullscreen
************************************************/

void setVideoFullscreen(void)
{
    if(1 == videoEnabled)
        return;
    videoConf.fullscreen = 1;
}

//-------------------------------------------

/***********************************************
    setVideoWindowed
************************************************/

void setVideoWindowed(void)
{
    if(1 == videoEnabled)
        return;
    videoConf.fullscreen = 0;
}

//-------------------------------------------

/***********************************************
    setResolution
************************************************/

void setResolution(int x , int y)
{
    if( 1 == videoEnabled)
        return;
    videoConf.screenW = x;
    videoConf.screenH = y;
}

//-------------------------------------------

/***********************************************
    set game framebuffer scale factor
************************************************/
void setScaleFactor(unsigned value)
{
    if(1 == videoEnabled)
        return;
    videoConf.scaleFactor = value;
}

/***********************************************
 set game area position on the screen
************************************************/
void setGameAreaPos(int x , int y)
{
    if(1 == videoEnabled)
        return;
    videoConf.gameAreaX = x;
    videoConf.gameAreaY = y;
}

/***********************************************
    setVideoWindowed
************************************************/

int initVideo(void)
{
	Uint32 SDL_SetVideoMode_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN ;

    if(0 == videoConf.fullscreen)
    {    /*Unset fullscreen flag*/
         SDL_SetVideoMode_flags ^= SDL_FULLSCREEN;
    }

    //Create a new window
    screen = SDL_SetVideoMode(   videoConf.screenW
                               , videoConf.screenH
                               , SCREEN_BPP
	                           , SDL_SetVideoMode_flags
                             );
    /*Set Window title*/
    SDL_WM_SetCaption("OverHeated v0.2a", NULL);

	if ( !screen )
	{
        sprintf(videoErrMsg
                ,"[ERROR][VIDEO] Unable to set %dx%d video mode:\n%s\n"
			    , videoConf.screenW , videoConf.screenH , SDL_GetError());
        return -1;
	}

    /*Create game frame buffer*/
    gameFb = SDL_CreateRGBSurface( SDL_SWSURFACE
                                  , GM_SCREEN_W
                                  , GM_SCREEN_H
                                  , SCREEN_BPP
					              , 0, 0, 0, 0
                                 );
    /*Create scanlines mask*/
    /*if(videoConf.scanlines )
    {
        SDL_Rect maskRect = { 0 , 0 , screen->w , screen->h };
        scanlinesMask = createScanLineMask( maskRect , videoConf.slValue );
    }*/
    
    videoEnabled = 1;

    return 0;
}

/***********************************************
    blitToGameFb
    Blit surface to the game framebuffer
************************************************/

int blitToGameFb(  SDL_Surface *src
                 , const SDL_Rect *srcRect
                 , SDL_Rect *dstrect
                 )
{
    
    SDL_BlitSurface( src , (SDL_Rect *)srcRect , gameFb , dstrect );
    return 0;
}

/***********************************************
    updateVideoScreen
************************************************/

void updateVideoScreen(void)
{
	    //int zoom = videoConf.scaleFactor;
        SDL_Rect gamePos =
        {
             videoConf.gameAreaX
            ,videoConf.gameAreaY
            ,0,0
        };

        /**/
        SDL_BlitSurface( gameFb , NULL , screen , &gamePos );
		/*if(zoom)
        {
			SDL_zoom( gameFb , screen , &gamePos , zoom);
        }
		else
        {
			SDL_BlitSurface( gameFb , NULL , screen , &gamePos );
        }*/

       /* if(NULL != scanlinesMask)
            SDL_fadeSurface(  scanlinesMask 
                            , videoConf.slValue 
                            , screen);*/

		SDL_Flip(screen);// update the screen
		//SDL_UpdateRect(screen,0,0,0,0);
}

//-------------------------------------------

/***********************************************
    stopVideo
    Disable Video
************************************************/

void stopVideo(void)
{
    if( gameFb != NULL)
    {
	    SDL_FreeSurface(gameFb);
        gameFb = NULL;
    }
    /*if( scanlinesMask != NULL)
    {
	    SDL_FreeSurface(scanlinesMask);
        scanlinesMask = NULL;        
    }*/
    return;
}

//-------------------------------------------

/***********************************************
    getVideoScreen
************************************************/
SDL_Surface *getVideoScreen(void)
{
    return screen;
}

//-------------------------------------------

/***********************************************
    getGameFb
************************************************/
SDL_Surface *getGameFb(void)
{
    return gameFb;
}

//-------------------------------------------

/*************************************************
    getVideoErrMsg
    Return video error message string
*************************************************/

char *getVideoErrMsg(void)
{
    return videoErrMsg;
}

//-------------------------------------------

/*************************************************
    clearGameFb
*************************************************/


void clearGameFb(void)
{
    SDL_FillRect(gameFb, NULL, SDL_MapRGB(gameFb->format, 0, 0, 0));
}
