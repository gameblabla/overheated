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
#ifndef DREAMCAST
#include <SDL/SDL_mixer.h>
#endif

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

#ifdef DREAMCAST
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
extern sfxhnd_t SEPool[8];
extern int sfxVolume;
#endif

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
#ifdef DREAMCAST
	return loadChunkArray(filePath , NULL);
#else
	return loadChunkArray(filePath , &AudioSfx);
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
	snd_sfx_unload_all();
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
#ifdef DREAMCAST
	snd_sfx_stop(num+4);
	snd_sfx_play_chn(num+4, SEPool[num], sfxVolume, 0x80);
#else
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
#ifdef DREAMCAST
	snd_sfx_stop(MIXER_ANNOUNCER_CHANEL);
	snd_sfx_play_chn(MIXER_ANNOUNCER_CHANEL, SEPool[num], sfxVolume, 0x80);
#else
	Mix_HaltChannel( MIXER_ANNOUNCER_CHANEL );
	Mix_PlayChannel( MIXER_ANNOUNCER_CHANEL , *(AudioSfx.sample+num) , 0);
#endif
	return;
}
