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
   video.h
   Video subsystem
*********************************************/

#ifndef VIDEO_H
#define VIDEO_H

/*NOTE:
  All the 'set*' functions only work before calling initVideo,
  video settings CAN'T be changed after calling initVideo
  if video is ENABLED, any call to 'set*' functions won't have any effect
*/

int setResolution(int x , int y);
//void getResolution(int *x , int *y);
void setVideoFullscreen(void);
void setVideoWindowed(void);    
void setScaleFactor(unsigned value); //set game area scale factor
void setGameAreaPos(int x , int y); //set game area position on the screen
int getVideoScanLines(void); //Get scanlines Intensity value
void setVideoScanLines(unsigned value); // set scanlines intensity value
void videoEnableScanlines(void);
void videoDisableScanlines(void);
void incScanlines(void);
void decScanlines(void);

int initVideo(void); /* Init video with the current Video Configuration
                        Returns:
                          0  : Everything OK
                         -1  : Can't Initialize video 
                               with the current settings
                     */

char *getVideoErrMsg(void); //Return video error message string

void updateVideoScreen(void);   /*Update the screen*/

#ifdef _SDL_H           
                                         
SDL_Surface *getVideoScreen(void); /* Return a pointer to the Screen surface
                                      Returns NULL if the video subsystem hasn't been initialized.
                                   */

SDL_Surface *getGameFb(void);    /* Return a pointer to the game frame buffer
                                    Returns NULL if the video subsystem hasn't been initialized.
                                 */

/************************************** 
   blitToGameFb
   Blit surface to the game frame buffer
   Returns 0 if the blit is successful 
   or a negative number as an error code
****************************************/
int blitToGameFb(  SDL_Surface *src
                 , const SDL_Rect *srcrect
                 , SDL_Rect *dstrect
                 );

void clearGameFb(void);

void stopVideo(void);

#endif
//
#endif



