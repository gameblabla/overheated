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
    main.c
*********************************************/

//
#include "license.c"

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

//
#include "globalDefines.h"
#include "d_GameObject.h"

//
#include "audio.h"
#include "video.h"
#include "menu.h"
#include "game.h"
#include "intro.h"
#include "scoreboard.h"
#include "gameConfig.h"
#include "SDL_utils.h"
#include "debug.h"
#include "input.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------------

/********************************************
    printHelp:
*********************************************/

static void printHelp(char **argv)
{
			printf( "Usage: %s [options]\n"
                    "\n"
				    "[OPTIONS]\n"
                    "\n"
				    "   -f, --fullscreen\tSet Full screen mode\n"
                    "                   \t *This switch takes priority\n"
                    "                   \t  over the configuration file\n"
                    "\n"
                    "   -w, --window    \tEnable window mode\n"
                    "                   \t *This switch takes priority\n"
                    "                   \t over the configuration file\n"
                    "\n"
                    "   -rm[MODE] --resmode[MODE]   \tSet resolution mode"
                    "\n"
				    "   -h, --help      \tDisplay this help and exit\n"
                    "\n"
                    ,*argv);
    return;
}

//-------------------------------------------

/***********************************************
    main
************************************************/

static int mainloop(void)
{
	#ifdef DEBUG
	printf("[DEBUG] INTRO\n");
	#endif
	intro();
	#ifdef DEBUG
	printf("[DEBUG] MENU\n");
	#endif
	menu();
}

int main(int argc , char **argv)
{

    int windowedSwitch = 0;
    int fullscreenSwitch = 0;
    int resModeSwitch = -1;
	//Read arguments
	int i=0;
#ifdef PC
	while(i < argc)
	{
		if(    strcmp("-w" , *(argv+i)) == 0
		    || strcmp("--window" , *(argv+i)) == 0)
        {
	        windowedSwitch = 1;
        }

		//Fullscreen
		if(    strcmp("-f" , *(argv+i)) == 0
		    || strcmp("--fullscreen" , *(argv+i)) == 0)
        {
             fullscreenSwitch = 1;
        }

        //Resolution mode
		if(strncmp("-rm" , *(argv+i) , sizeof("-rm")-1) == 0)
        {
            resModeSwitch = atoi( *(argv+i)+sizeof("-rm")-1 );
        }
        else if(strncmp("--resmode" , *(argv+i) , sizeof("--resmode")-1 ) == 0)
        {
            resModeSwitch = atoi( *(argv+i)+sizeof("--resmode")-1 );
        }

		//Help
		if(    strcmp("-h" , *(argv+i)) == 0
		    || strcmp("--help" , *(argv+i)) == 0)
		{
            printHelp(argv);
			return;
		}
		i++;
	}
#endif

	#ifndef DREAMCAST
    //Check SDL version
    check_SDL_Version();
    
    //Check SDL_Mixer version
    check_mixer_version();
    #endif

	// initialize SDL subsystems
	#ifdef DREAMCAST
    unsigned SDL_init_flags =  SDL_INIT_VIDEO;
	#else
    unsigned SDL_init_flags =  SDL_INIT_VIDEO 
                             | SDL_INIT_AUDIO
                             | SDL_INIT_JOYSTICK;
	#endif

	if ( SDL_Init( SDL_init_flags ) < 0 )
	{
		printf( "Unable to initialize SDL: %s\n", SDL_GetError() );
		return 1;
	}

	//Disable cursor
	SDL_ShowCursor(0);
	#ifndef DREAMCAST
	//Open joystick 0
	SDL_JoystickOpen(0);
	#endif
	//Open audio device
	openAudio();
	//Clean up before exit
	#ifndef DREAMCAST
	atexit(SDL_Quit);
	atexit(closeAudio);
	#endif

    //Load game configuration
    loadGameCfg();

#ifdef PC
    /*NOTE: The window option has priority over
    /       the fullscreen option*/
    if( 1 == windowedSwitch)
    {
        setVideoWindowed();
    }
    else if( 1 == fullscreenSwitch)
    {
         setVideoFullscreen();
    }
    /**/
    if( resModeSwitch >= 0)
         setResMode(resModeSwitch);
#endif

    //Init Video Subsystem
    if( -1 == initVideo())
    {
        fprintf(stderr,"%s",getVideoErrMsg());
        exit(-1);
    }

	SDL_Surface* screen = getVideoScreen();

    //Initialize game subsystems
    loadScoreboards();
    initInputSystem();

    //musicTest(screen);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mainloop, 60, 1);
#else
	mainloop();
#endif

	return 0;
}

