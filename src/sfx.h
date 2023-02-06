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
    sfx.h
*******************************************/

/*Audio special effects*/

#ifndef SFX_H
#define SFX_H

void playEffect(int num);		//Play sample on a regular audio channel
					            //num = Sample number\index ( see l_sfx.h and sfx.c)

void playAnnouncement(int num); //Play sample on the announcer chanel
                                //*The "announcer chanel" is reserved for announcements (duh)
                                // Regular effects WILL NOT intefere with the samples in this channel

int loadAudioSfx(char *filePath);	//Load audio effects from a ".sfx" file( see sfx.c)
					                //".sfx" : list of samples and sound effects

void freeAudioSfx(void);		    //Free sound effects

#endif
