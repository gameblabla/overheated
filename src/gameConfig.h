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
   gameConfig.h
*********************************************/


int loadGameCfg(void); //You should call this function before
                       //using any other function on this module!

int saveGameCfg(void);

int  getFullscreenFlag(void);
int  getResModeInfo(unsigned mode , int *x , int *y);    //Get information about the resolution mode
                                                          // ( X and Y resolution values)

char *getResModesStrInfo(void); // Get the information string("name") of the current resolution mode
void incResMode(void);
void decResMode(void);

void setFullscreenFlag(void);
void resetFullscreenFlag(void);

int  getScanlinesFlag(void);
void setScanlinesFlag(void);
void resetScanlinesFlag(void);


