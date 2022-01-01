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
   music.h
*********************************************/

#ifndef MUSIC_C
#define MUSIC_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <SDL/SDL.h>
//#include <SDL/SDL_mixer.h>
//
#ifdef DREAMCAST
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#ifdef ADX_PLAY
#include <libadx/libadx.h> /* ADX Decoder Library */
#include <libadx/snddrv.h> /* Direct Access to Sound Driver */
#endif
#ifdef OPUS_DC
#include <opusplay/opusplay.h>
#endif

#endif

#include "music.h"
#include "l_music.h"
#include "trackPlayer.h"
#include "d_filePaths.h"

/********************************************
    setMusicVolume
*********************************************/
#ifdef DREAMCAST
static uint_fast8_t mute = 0;
static uint_fast8_t cd_vol = 15;
#endif

void setMusicVolume(int volume)
{
    #ifdef DREAMCAST
    #ifdef ADX_PLAY
    extern snd_stream_hnd_t shnd;
    if (shnd) snd_stream_volume(shnd, volume);
    if (volume < 1) mute = 1;
    else mute = 0;
    #else
    int left = 15;
    if (volume > 254) left = 15;
    else left = volume * 0.058;
	spu_cdda_volume(left, left);
	cd_vol = left;
	#endif
	#endif
    setTrackPlayerVolume(volume);
    return;
}

/********************************************
    getMusicVolume
*********************************************/

int getMusicVolume(void)
{
    return getTrackPlayerVolume();
}

/********************************************
    incMusicVolume
*********************************************/

void incMusicVolume(int amount)
{
    int volume = getTrackPlayerVolume()+amount;
    setTrackPlayerVolume(volume);
    #ifdef ADX_PLAY
    extern snd_stream_hnd_t shnd;
    if (shnd) snd_stream_volume(shnd, volume);
    if (volume < 1) mute = 1;
    else mute = 0;
	#endif
}

/********************************************
    playMusicTrack:
    Load and play music from a ".track" file
*********************************************/
int playMusicTrack(int trackNumber)
{
	//0 Menu, 1 2min, 2 5min
#ifdef DREAMCAST
	snd_sfx_stop_all();
	if (mute == 1) return 0;
	#ifdef ADX_PLAY
	adx_stop();
	switch(trackNumber)
	{
		case 0:
			adx_dec( DREAMCAST_CD_PATH "intro.adx", 0 );
		break;
		case 1:
			adx_dec( DREAMCAST_CD_PATH "2min.adx", 1 );
		break;
		case 2:
			adx_dec( DREAMCAST_CD_PATH "5min.adx", 1 );
		break;
	}
	#elif defined(OPUS_DC)
	switch(trackNumber)
	{
		case 0:
			opusplay_play_file(DREAMCAST_CD_PATH "intro.opus", 0);
		break;
		case 1:
			opusplay_play_file(DREAMCAST_CD_PATH "2min.opus", 0);
		break;
		case 2:
			opusplay_play_file(DREAMCAST_CD_PATH "5min.opus", 0);
		break;
	}
	#else
	switch(trackNumber)
	{
		case 0:
			cdrom_cdda_play(1, 1, 0, CDDA_TRACKS);
		break;
		case 1:
			cdrom_cdda_play(2, 2, 0xF, CDDA_TRACKS);
		break;
		case 2:
			cdrom_cdda_play(3, 3, 0xF, CDDA_TRACKS);
		break;
	}
	spu_cdda_volume(cd_vol, cd_vol);
	spu_cdda_pan(cd_vol, cd_vol);
	#endif
#else
    char *filePath = getTrackFilePath(trackNumber);
    if( filePath == NULL)
        return 1;

    freeMusic();
    track_t *track = loadTrack(filePath);
    playTrack(track);
#endif

    return 0;
}
/********************************************
    freeMusic:
    Stop music playback
    and unload the music data from memory
*********************************************/
void freeMusic(void)
{
#ifdef DREAMCAST
	#ifdef ADX_PLAY
		adx_stop();
	#elif defined(OPUS_DC)
	    if (opusplay_is_playing())
	    {
			opusplay_stop();
		}
	#else
		cdrom_cdda_pause();
	#endif
#else
    track_t *track = ejectTrack();
    if(track != NULL)
        freeTrack(track);
#endif
}
