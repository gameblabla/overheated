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

#ifndef MUSIC_H
#define MUSIC_H

/********************************************
    Private:
********************************************/

#ifdef MUSIC_C

#endif

/********************************************
    Public:
********************************************/

int playMusicTrack(int trackNumber); //Load and play music from a ".track" file

void freeMusic(void);               //Stop music playback
                                    // and unload the music data from memory

int pauseMusic(void);
int resumeMusic(void);

int musicPaused(void);
int musicHalted(void);
int musicLoaded(void);              //There is music data loaded on memory

void setMusicVolume(int volume);
int getMusicVolume(void);


#endif
