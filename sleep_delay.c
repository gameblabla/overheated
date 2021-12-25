#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "globalDefines.h"
#include "sleep_delay.h"

static int sleep_delay = 0;
static int ms = 0;

void Start_Sleep_Delay()
{
	#ifndef NODELAY
	ms = SDL_GetTicks();
	#endif
}

void Sleep_Delay()
{
	#ifndef NODELAY
	sleep_delay = mspf - (SDL_GetTicks()-ms);
	if(sleep_delay > 0)
	{
		SDL_Delay( sleep_delay );
	}
	#endif
}
