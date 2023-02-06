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
   audio.c
   Audio subsystem functions
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
//
#include "system_var_type.h"
#include "str_utils.h"
#include "d_audio.h"
#include "audio.h"

#ifdef DREAMCAST
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
sfxhnd_t SEPool[8];
#endif

//-------------------------------------------


#define SYSTEM_VARS_NUMBER 0 //See str_utils.h
#define STR_BUFFER_MAXSIZE 100 //See str_utils.h

#define ERROR_HANDLING exit(-1);

int sfxVolume = MIX_MAX_VOLUME; //Sound effects volume

//-------------------------------------------

/********************************************
    check_mixer_version

    Compare the runtime version of SDL_mixer with the version
    that the program was compiled with.
    Issues a warning if they don't match

    Returns:
        0 if the versions match
        otherwise returns a value DIFFERENT from 0

*********************************************/

int check_mixer_version(void)
{
     	
    return 0;
}

//-------------------------------------------

/********************************************
    openAudio
*********************************************/

int openAudio(void)
{
#ifndef DREAMCAST
	//SDL_Mixer open audio device
	if( Mix_OpenAudio( AUDIO_FREQ ,  AUDIO_FMT  , 2 , AUDIO_CHUNKSIZE) < 0)
	{
		fprintf(stderr,"[ERROR] Mix_OpenAudio: %s\n", Mix_GetError());
		ERROR_HANDLING
	}

	//Load support for ogg
	if (MIX_INIT_OGG & Mix_Init(MIX_INIT_OGG) == 0)
	{
		fprintf(stderr,"[ERROR] SDL_mixer can't load ogg support\n");
		ERROR_HANDLING
	}
	
	//Set channels volume
	Mix_AllocateChannels(MIXER_CHANNELS_NUMBER);
	Mix_Volume(-1, sfxVolume);
#else
	snd_stream_init();
#endif
	return 0;
}

//-------------------------------------------

/********************************************
    closeAudio:

    Close audio subsystem
*********************************************/

void closeAudio(void)
{
#ifndef DREAMCAST
	//Unload support modules
	while(Mix_Init(0)) 
		Mix_Quit();

	//Close Audio device
	while ( Mix_QuerySpec(NULL , NULL , NULL))
		Mix_CloseAudio();
#else
	snd_stream_shutdown();
#endif
	return ;
}

//-------------------------------------------

/********************************************
    loadChunkArray:
      Load a list of samples onto a ChunkArray
*********************************************/

#ifdef DREAMCAST
const char sfx_arr[8][32] =
{
	"/cd/data/sfx/explosion1.wav",
	"/cd/data/sfx/powerUp.wav",
	"/cd/data/sfx/coin.wav",
	"/cd/data/sfx/bossWarning.wav",
	"/cd/data/sfx/specialBonusv1.wav",
	"/cd/data/sfx/warning.wav",
	"/cd/data/sfx/powerDown.wav",
	"/cd/data/sfx/playerExplosion.wav"
};
#endif

int loadChunkArray(char *fileName , Mix_ChunkArray *array)
{
#ifdef DREAMCAST
	int i;
	for(i=0;i<8;i++)
	{
		SEPool[i] = snd_sfx_load(sfx_arr[i]); 
	}
#else
	systemVar_t *systemVars = NULL;

	FILE *inFp;
	if( ( inFp = fopen( fileName , "rb")) == NULL)	
	{
		fprintf(stderr , "[Error] opening %s\n",fileName);
		ERROR_HANDLING
	}

	char finStrBuffer[64]; //File string input buffer
	char StrBuffer[64]; //String buffer

	//Get number of samples
	array->size = 0;
	while( !searchString(inFp,"sample",':'))
		array->size++;

	//Allocate samples chunks
	array->sample = malloc( sizeof(Mix_Chunk *) * array->size);
	fseek(inFp,0,SEEK_SET);

	//load samples
    //-----

	int i = 0;
	while( !searchString(inFp,"sample",':'))
	{
		FETCH_NEXT_STRING
		READ_NEXT_STRING
		*(array->sample+i) = Mix_LoadWAV(StrBuffer);
		if(!*(array->sample+i))
		{
			fprintf(stderr,"[ERROR] loading sample %s\n",StrBuffer);
			ERROR_HANDLING
		}

		Mix_VolumeChunk( *(array->sample+i) , MIX_MAX_VOLUME);
		i++;

#ifdef DEBUG
		printf("[DEBUG] Sample: \"%s\" loaded\n",StrBuffer);
#endif
	}
    //-----

#ifdef DEBUG
		printf("[DEBUG] %d samples loaded in total\n",array->size);
#endif

	fclose(inFp);
#endif
	return 0;
}

//-------------------------------------------

void freeChunkArray(Mix_ChunkArray *array)
{
// Handled directly in sfx.c for DC
#ifndef DREAMCAST
	if( array->sample == NULL
	   || array->size <= 0)
	{
		return;
	}

	int32_t i;
	for( i = array->size-1 ; i >= 0 ; i--)
		Mix_FreeChunk(*(array->sample+i));

	free( array->sample);
#endif
	return;
}

//-------------------------------------------

/********************************************
    setSFX_Volume:
      Set effects volume
*********************************************/

void setSFX_Volume( int volume)
{
    if( volume < 0)
        volume = 0;
    else if( volume > MIX_MAX_VOLUME)
        volume = MIX_MAX_VOLUME;

    sfxVolume = volume;
    #ifndef DREAMCAST
	Mix_Volume(-1, sfxVolume);
	#endif
}

//-------------------------------------------

/********************************************
    incSFX_Volume:
      increment effects volume
*********************************************/

void incSFX_Volume(void)
{
    if( sfxVolume < MIX_MAX_VOLUME)
#ifdef DREAMCAST
        sfxVolume+=15;
#else
        sfxVolume++;
#endif

#ifndef DREAMCAST
	Mix_Volume(-1, sfxVolume);
#endif
}

//-------------------------------------------

/********************************************
    decSFX_Volume:
      decrement effects volume
*********************************************/

void decSFX_Volume(void)
{
    if( sfxVolume > 0)
#ifdef DREAMCAST
        sfxVolume-=15;
#else
        sfxVolume--;
#endif

#ifndef DREAMCAST
	Mix_Volume(-1, sfxVolume);
#endif
}

//-------------------------------------------

/********************************************
    getSFX_Volume:
      Get effects volume value
*********************************************/

int getSFX_Volume(void)
{
    return sfxVolume;
}

//-------------------------------------------






