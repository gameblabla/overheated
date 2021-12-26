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
#include <libadx/libadx.h> /* ADX Decoder Library */
#include <libadx/snddrv.h> /* Direct Access to Sound Driver */
#endif

#include "music.h"
#include "l_music.h"
#include "trackPlayer.h"

/********************************************
    setMusicVolume
*********************************************/

void setMusicVolume(int volume)
{
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
}

/********************************************
    playMusicTrack:
    Load and play music from a ".track" file
*********************************************/
int playMusicTrack(int trackNumber)
{
	//0 Menu, 1 2min, 2 5min
#ifdef DREAMCAST
	//adx_stop();
	switch(trackNumber)
	{
		case 0:
			cdrom_cdda_play(1, 2, 15, CDDA_TRACKS);
			//adx_dec( "/cd/intro.adx", 1 );
		break;
		case 1:
			cdrom_cdda_play(2, 3, 15, CDDA_TRACKS);
			//adx_dec( "/cd/2min.adx", 1 );
		break;
		case 2:
			cdrom_cdda_play(3, 4, 15, CDDA_TRACKS);
			//adx_dec( "/cd/5min.adx", 1 );
		break;
	}
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
    /* Spin down the CD */
	//cdrom_cdda_pause();
	//adx_stop();
#else
    track_t *track = ejectTrack();
    if(track != NULL)
        freeTrack(track);
#endif
}
