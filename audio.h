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
   audio.h
   Audio subsystem functions
*********************************************/

#ifndef AUDIO_H
#define AUDIO_H

int  check_mixer_version(void);  //-Check the SDL_Mixer version
int  openAudio(void);	         //-Open the audio device
void closeAudio(void);	         //-Close the audio device

void setSFX_Volume( int volume);
int  getSFX_Volume(void);
void incSFX_Volume(void);
void decSFX_Volume(void);

#ifdef D_AUDIO_H
int  loadChunkArray(char *filePath , Mix_ChunkArray *array);	//-Load a list of samples
void freeChunkArray(Mix_ChunkArray *array);			            //-Free a Mix_ChunkArray
#endif

#endif
