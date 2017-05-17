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
   d_video.h
   video data types and constants
*********************************************/

#ifndef D_VIDEO_H
#define D_VIDEO_H

struct videoConf_t
{
    /*Screen Width and Height*/
    int screenW;
    int screenH;
    /**/
    int scaleFactor; //Frame buffer scale factor
    /**/
    int fullscreen; // 0 = Windowed , 1 = Fullscreen
    /**/
    int gameAreaX; //Game area position on the screen
    int gameAreaY;
    /**/
    int scanlines; // 0 = Scanlines disabled , 1 = Scanlines enabled
    /**/
    int slValue; //Scan lines intensity 
};

typedef struct videoConf_t videoConf_t;

#endif
