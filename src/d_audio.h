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
   d_audio.h
   Audio data types and constants
*********************************************/

#ifndef D_AUDIO_H
#define D_AUDIO_H

//-------------------------------------------

#define AUDIO_FREQ 44100
#define AUDIO_CHUNKSIZE 2048
#define AUDIO_FMT AUDIO_S16SYS 
#define MIXER_CHANNELS_NUMBER 16
#define MIXER_SFX_CHANELS MIXER_CHANNELS_NUMBER - 1
#define MIXER_ANNOUNCER_CHANEL MIXER_CHANNELS_NUMBER - 1

//-------------------------------------------

struct Mix_ChunkArray
{
	unsigned size;
	Mix_Chunk **sample;
};

typedef struct Mix_ChunkArray Mix_ChunkArray;

#endif
