#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "globalDefines.h"
#include "sleep_delay.h"
#include "video.h"
#include "audio.h"
static int sleep_delay = 0;
static int ms = 0;

void Start_Sleep_Delay()
{
	#ifndef NODELAY
	ms = SDL_GetTicks();
	#endif
}

/*
 * Gameblabla :
 * We also use this to hook up some stuff that would run in a loop
 * as we have no real other way to do it.
 * 
*/

void Sleep_Delay()
{
	#ifndef NOEXIT
	Uint8 *keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_ESCAPE])
	{
		closeAudio();
		stopVideo();
		SDL_Quit();
		exit(1);
	}
	#endif
	
	
	sleep_delay = mspf - (SDL_GetTicks()-ms);
	#ifndef NODELAY
	if(sleep_delay > 0)
	{
		SDL_Delay( sleep_delay );
	}
	#endif
}
