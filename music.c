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
    char *filePath = getTrackFilePath(trackNumber);
    if( filePath == NULL)
        return;

    freeMusic();
    track_t *track = loadTrack(filePath);
    playTrack(track);

    return 0;
}
/********************************************
    freeMusic:
    Stop music playback
    and unload the music data from memory
*********************************************/
void freeMusic(void)
{
    track_t *track = ejectTrack();
    if(track != NULL)
        freeTrack(track);
}
