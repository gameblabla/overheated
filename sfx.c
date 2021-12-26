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
    sfx.c
*******************************************/

#ifdef SFX_C
#define SFX_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
//
#include "audio.h"
#include "d_audio.h"
#include "l_sfx.h"
//
#include "system_var_type.h"
#include "str_utils.h"
//
#define ERROR_HANDLING exit(-1);
#define CHECK_SAMPLE_NUM

//-------------------------------------------
#ifndef DREAMCAST
Mix_ChunkArray AudioSfx; //Sound effects samples
#endif
//-------------------------------------------

/********************************************
 loadAudioSfx:
    Load a list of samples from file (.sfx) to AudioSfx
********************************************/

int loadAudioSfx(char *filePath)
{
#ifndef DREAMCAST
	return loadChunkArray(filePath , &AudioSfx);
#else
	return 0;
#endif
}

//-------------------------------------------

/********************************************
  freeAudioSfx
    Unload the sound effect samples
********************************************/

void freeAudioSfx(void)
{
#ifndef DREAMCAST
	freeChunkArray(&AudioSfx);
#else
#endif
	return;
}

//-------------------------------------------

/********************************************
  playEffect
    Play sample on a regular audio channel
********************************************/

void playEffect(int num)
{
#ifndef DREAMCAST
	static int channelNum = 0;

#ifdef CHECK_SAMPLE_NUM
	if( num >= AudioSfx.size)
		return;
#endif

	Mix_HaltChannel( channelNum);
	Mix_PlayChannel( channelNum , *(AudioSfx.sample+num) , 0);
	channelNum++;
	if( channelNum >=  MIXER_SFX_CHANELS )
	channelNum = 0;
#endif
	return;
}

//-------------------------------------------

/********************************************
  playAnnouncement
    The "announcer chanel" is reserved for announcements (duh)
    Regular effects WILL NOT intefere with the samples in this channel
********************************************/

void playAnnouncement(int num)
{
#ifndef DREAMCAST
	Mix_HaltChannel( MIXER_ANNOUNCER_CHANEL );
	Mix_PlayChannel( MIXER_ANNOUNCER_CHANEL , *(AudioSfx.sample+num) , 0);
#endif
	return;
}




