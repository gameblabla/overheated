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
    input.c
*********************************************/

#ifndef INPUT_H
#define INPUT_H

extern int initInputSystem(void);       //Init the game input system

#ifdef D_INPUT_H

void initJoypad( joypadStruct *controller);
int readJoypad( joypadStruct *controller);
int joypadPressed( joypadStruct *pressed , joypadStruct rawInput , joypadStruct lastFrameInput ); //    Determine which buttons has been pressed on this frame
//
void initController( controllerStruct *controller);
int readController( controllerStruct *controller);
int noControllerInput( controllerStruct *controller); //Returns 1 if no button, key or stick is pressed or held, 0 otherwise

int loadJoypadRecording( joypadRecording *recording , char *filePath);
int freeJoypadRecording( joypadRecording *recording);

int loadGameReplay(char *filePath);
int saveGameReplay(char *filePath);
int freeGameReplay(void);

int startJoypadRecording(void);
int stopJoypadRecording(void);
//
int startJoypadReplay(void);
int stopJoypadReplay(void);
//
int readJoypadRecording( joypadStruct *joypadData , joypadRecording *recording);

//DEBUG
int printGameReplayInfo(void);
int printJoypadRecording(joypadRecording *recording);

#endif

#endif
