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

#ifndef INPUT_C
#define INPUT_C
#endif
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "d_input.h"
#include "loadData.h"
#include "input.h"
#include "globalDefines.h"
#ifdef DREAMCAST
#include <kos.h>
#endif

//
#define ERROR_HANDLING exit(-1);
//#define INPUT_DEBUG

//Controls map(a.k.a controls configuration)
static inputConf controlsMap;
//
static joypadRecording gameReplay;
//Recording joypad input
static int recording;
//Replaying joypad input
static int replaying;


/*********************************************
    initInputSystem:
    Init the game input system
**********************************************/

int initInputSystem(void)
{
    loadInputConf(DREAMCAST_CD_PATH"data/config/controls.cfg" , &controlsMap );
#ifdef DREAMCAST
	cont_btn_callback(0, CONT_START | CONT_A | CONT_B | CONT_X | CONT_Y, (cont_btn_callback_t)arch_reboot);
#endif
    return 0;
}

//-------------------------------------------

/*********************************************
    initJoypad:
    Init joypad struct values
**********************************************/


void initJoypad( joypadStruct *controller)
{
	memset( controller , 0x00 , sizeof(joypadStruct));
	return;
}

//-------------------------------------------

/*********************************************
    initController:
    Init joypad struct values
**********************************************/

void initController( controllerStruct *controller)
{
    initJoypad( &controller->raw);
    initJoypad( &controller->old);
    initJoypad( &controller->held);
    initJoypad( &controller->pressed);

    return;
}

//-------------------------------------------

/*********************************************
    readJoypad:
    Read player input
**********************************************/
int readJoypad(joypadStruct *controller)
{
#ifdef DREAMCAST
	#define MOUSE_SPEED_DC 3
	extern uint_fast8_t mouse_speed;
	static maple_device_t *cont, *kbd,  *mouse;
	mouse_state_t *mstate;
	kbd_state_t* first_kbd_state; 
	cont_state_t *state;	
	uint_fast8_t i;
    controller->UP = 0;
    controller->DOWN = 0;
    controller->LEFT = 0;
    controller->RIGHT = 0;
    controller->START = 0;
	controller->A = 0;
	controller->B = 0;
	controller->SELECT = 0;
	mouse_speed = 0;
    
	if(cont)
	{
		state = (cont_state_t *)maple_dev_status(cont);
		if (state->buttons & CONT_START)
            controller->START = 1;              
		if (state->buttons & CONT_X)
			controller->SELECT = 1;
		if (state->buttons & CONT_A) 
            controller->A = 1;
		if (state->buttons & CONT_B) 
            controller->B = 1;
		if (state->buttons & CONT_DPAD_UP || state->joyy < -64) 
            controller->UP = 1;
		if (state->buttons & CONT_DPAD_DOWN || state->joyy > 64) 
            controller->DOWN = 1;
		if (state->buttons & CONT_DPAD_LEFT || state->joyx < -64) 
            controller->LEFT = 1;
		if (state->buttons & CONT_DPAD_RIGHT || state->joyx > 64) 
            controller->RIGHT = 1;   
	}
	else if (!cont)
	{
		for(i=0;i<4;i++)
		{
			cont = maple_enum_type(i, MAPLE_FUNC_CONTROLLER);
			if (cont) break;
		}
	}
	
	if (kbd)
	{
		first_kbd_state = (kbd_state_t *) maple_dev_status(kbd);
	
		if (first_kbd_state->matrix[KBD_KEY_UP]) controller->UP = 1;
		else if (first_kbd_state->matrix[KBD_KEY_DOWN]) controller->DOWN = 1;
		
		if (first_kbd_state->matrix[KBD_KEY_LEFT]) controller->LEFT = 1;
		else if (first_kbd_state->matrix[KBD_KEY_RIGHT]) controller->RIGHT = 1;
		
		if (first_kbd_state->matrix[KBD_KEY_SPACE] || first_kbd_state->matrix[KBD_KEY_X]) controller->A = 1;
		if (first_kbd_state->matrix[KBD_KEY_C]) controller->B = 1;
		
		if (first_kbd_state->matrix[KBD_KEY_ENTER]) controller->START = 1;
	}
	else if (!kbd)
	{
		for(i=0;i<4;i++)
		{
			kbd = maple_enum_type(i, MAPLE_FUNC_KEYBOARD);
			if (kbd) break;
		}
	}
	
	if(mouse)
	{
        mstate = (mouse_state_t *)maple_dev_status(mouse);
        if (mstate)
        {
			if (mstate->dx < -2)
			{
				mouse_speed = MOUSE_SPEED_DC;
				controller->LEFT = 1;
			}
			else if (mstate->dx > 2)
			{
				mouse_speed = MOUSE_SPEED_DC;
				controller->RIGHT = 1;
			}
			if (mstate->dy < -2)
			{
				mouse_speed = MOUSE_SPEED_DC;
				controller->UP = 1;
			}
			else if (mstate->dy > 2)
			{
				mouse_speed = MOUSE_SPEED_DC;
				controller->DOWN = 1;
			}

			if (mstate->buttons & MOUSE_LEFTBUTTON) controller->A = 1;
			if (mstate->buttons & MOUSE_RIGHTBUTTON) controller->B = 1;
		}
	}
	else if (!mouse)
	{
		for(i=0;i<4;i++)
		{
			mouse = maple_enum_type(i, MAPLE_FUNC_MOUSE);
			if (mouse) break;
		}
	}

#else
	static SDL_Event event;

	if(SDL_PollEvent(&event))
		do//Repeat until the events queue is empty
		{
			if(    event.type == SDL_QUIT)
			{
					printf("[INFO] SDL_QUIT event\n");
					exit(1);
			}

			if(controlsMap.keyboard)
				switch(event.type)
				{
					case SDL_KEYDOWN:

                        if(0);
#ifdef PC
                        else if(event.key.keysym.sym == SDLK_ESCAPE)
                            exit(1);
#endif
						else if(event.key.keysym.sym == controlsMap.key.UP)
							controller->UP = 1;

						else if(event.key.keysym.sym == controlsMap.key.DOWN)
							controller->DOWN = 1;

						else if(event.key.keysym.sym == controlsMap.key.LEFT)
							controller->LEFT = 1;

						else if(event.key.keysym.sym == controlsMap.key.RIGHT)
							controller->RIGHT = 1;

						else if(event.key.keysym.sym == controlsMap.key.A)
							controller->A = 1;

						else if(event.key.keysym.sym == controlsMap.key.B)
							controller->B = 1;

						else if(event.key.keysym.sym == controlsMap.key.START)
							controller->START = 1;

						else if(event.key.keysym.sym == controlsMap.key.SELECT)
							controller->SELECT = 1;
                            
					break;

					case SDL_KEYUP:

						if(event.key.keysym.sym == controlsMap.key.UP)
							controller->UP = 0;

						else if(event.key.keysym.sym == controlsMap.key.DOWN)
							controller->DOWN = 0;

						else if(event.key.keysym.sym == controlsMap.key.LEFT)
							controller->LEFT = 0;

						else if(event.key.keysym.sym == controlsMap.key.RIGHT)
							controller->RIGHT = 0;

						else if(event.key.keysym.sym == controlsMap.key.A)
							controller->A = 0;

						else if(event.key.keysym.sym == controlsMap.key.B)
							controller->B = 0;

						else if(event.key.keysym.sym == controlsMap.key.START)
							controller->START = 0;

						else if(event.key.keysym.sym == controlsMap.key.SELECT)
							controller->SELECT = 0;
					break;
				}

			if(controlsMap.joypad)
				switch(event.type)
				{
					case SDL_JOYBUTTONDOWN:
						if(event.jbutton.button == controlsMap.jbutton.UP)
							controller->UP = 1;

						else if(event.jbutton.button == controlsMap.jbutton.DOWN)
							controller->DOWN = 1;

						else if(event.jbutton.button == controlsMap.jbutton.LEFT)
							controller->LEFT = 1;

						else if(event.jbutton.button == controlsMap.jbutton.RIGHT)
							controller->RIGHT = 1;

						else if(event.jbutton.button == controlsMap.jbutton.A)
							controller->A = 1;

					    else if(event.jbutton.button == controlsMap.jbutton.B)
							controller->B = 1;

						else if(event.jbutton.button == controlsMap.jbutton.START)
							controller->START = 1;

						else if(event.jbutton.button == controlsMap.jbutton.SELECT)
							controller->SELECT = 1;

					break;

					case SDL_JOYBUTTONUP:
						if(event.jbutton.button == controlsMap.jbutton.UP)
							controller->UP = 0;

						else if(event.jbutton.button == controlsMap.jbutton.DOWN)
							controller->DOWN = 0;

						else if(event.jbutton.button == controlsMap.jbutton.LEFT)
							controller->LEFT = 0;

						else if(event.jbutton.button == controlsMap.jbutton.RIGHT)
							controller->RIGHT = 0;

						else if(event.jbutton.button == controlsMap.jbutton.A)
							controller->A = 0;

						else if(event.jbutton.button == controlsMap.jbutton.B)
							controller->B = 0;

						else if(event.jbutton.button == controlsMap.jbutton.START)
							controller->START = 0;

						else if(event.jbutton.button == controlsMap.jbutton.SELECT)
							controller->SELECT = 0;

					break;

					case SDL_JOYAXISMOTION:

					if(event.jaxis.axis == controlsMap.jaxisX)
					{
						if( event.jaxis.value >= AXIS_THRESHOLD)
							controller->RIGHT = 1;
						else if(event.jaxis.value <= -AXIS_THRESHOLD)
							controller->LEFT = 1;
						else
						{
							controller->LEFT = 0;
							controller->RIGHT = 0;
						}
					}
					if(event.jaxis.axis == controlsMap.jaxisY)
					{
						if( event.jaxis.value >= AXIS_THRESHOLD)
							controller->DOWN = 1;
						else if(event.jaxis.value <= -AXIS_THRESHOLD)
							controller->UP = 1;
						else
						{
							controller->UP = 0;
							controller->DOWN = 0;
						}
					}

					break;
				}
#ifdef INPUT_DEBUG
			if( event.type == SDL_JOYAXISMOTION)
				printf("which: %d\naxis %d\nvalue: %d\n"
					,event.jaxis.which,event.jaxis.axis,event.jaxis.value);
#endif

		}while (SDL_PollEvent(&event));
#endif
	return 0;
}

//-------------------------------------------

/*********************************************
    JoypadPressed:
    Determine which buttons had been pressed
    on this frame
**********************************************/


int joypadPressed( joypadStruct *pressed , joypadStruct rawInput , joypadStruct lastFrameInput )
{
    unsigned char *uPressed = (unsigned char *)pressed;
    unsigned char *uRawInput = (unsigned char *)(&rawInput);
    unsigned char *uLastFrameInput = (unsigned char *)(&lastFrameInput);

    uint_fast32_t i;
    uint_fast32_t bytesSize = sizeof(joypadStruct);
    for( i = 0 ; i < bytesSize ; i++)
    {
        *(uPressed + i) = *(uRawInput + i) ^ *(uLastFrameInput + i);
        *(uPressed + i) = *(uPressed + i) & *(uRawInput + i);
    }

    return 0;
}

/**********************************************
    JoypadHeld:
    Determine which buttons are being held
    for more than 2 frames
***********************************************/

int joypadHeld( joypadStruct *held , joypadStruct rawInput , joypadStruct lastFramePressed )
{
    unsigned char *uHeld = (unsigned char *) held;
    unsigned char *uRawInput = (unsigned char *) &rawInput;
    unsigned char *ulastFramePressed = (unsigned char *) &lastFramePressed;

    uint_fast32_t i;
    uint_fast32_t bytesSize = sizeof(joypadStruct);
    for( i = 0 ; i < bytesSize ; i++)
    {
        *( uHeld+i ) = *( uRawInput+i ) & (~*( ulastFramePressed+i ));
    }
    
    return 0;
}

//-------------------------------------------

/*********************************************
    addJoypadRecordEntry:
**********************************************/

static int addJoypadRecordEntry(joypadRecording *recording , joypadStruct joypadData)
{
    joypadRecordEntry *newEntry = malloc(sizeof(joypadRecordEntry));
    newEntry->next = NULL;
    newEntry->data = joypadData;

    if( recording->record == NULL)
    {
        recording->record = newEntry;
        recording->readPos = newEntry;
    }
    else
    {
        recording->last->next = newEntry;
    }

    recording->last = newEntry;

    return 0;
}

/*********************************************
    readController:
    Read the player controller
**********************************************/

int readController(controllerStruct *controller)
{
    controller->old = controller->raw;

    //Read raw input
    readJoypad(&controller->raw);

    //Recording
    if( recording)
    {
        addJoypadRecordEntry(&gameReplay , controller->raw);
    }
    else if( replaying)
    {
        joypadStruct tmpJoypad = controller->raw;
        readJoypadRecording(&controller->raw,&gameReplay);
        //use the START value readed in this frame
        //instead of the recorded one
        controller->raw.START = tmpJoypad.START;
    }

    //Get the buttons pressed this frame
    joypadPressed( &controller->pressed , controller->raw , controller->old);

    //Get held buttons
    joypadHeld( &controller->held , controller->raw , controller->pressed);

    
    return 0;
}

//-------------------------------------------

/*********************************************
    startJoypadRecording
**********************************************/

int startJoypadRecording(void)
{
    if( replaying )
        return -1;

    recording = 1;
    return 0;
}

//-------------------------------------------

/*********************************************
    stopJoypadRecording
**********************************************/

int stopJoypadRecording(void)
{
    recording = 0;
    return 0;
}

//-------------------------------------------

/*********************************************
    startReplay
**********************************************/

int startJoypadReplay(void)
{
    if( recording )
        return -1;

    replaying = 1;
    return 0;
}

//-------------------------------------------

/*********************************************
    stopJoypadRecording
**********************************************/

int stopJoypadReplay(void)
{
    replaying = 0;
    return 0;
}


//-------------------------------------------

/*********************************************
    readJoypadRecording
**********************************************/

int readJoypadRecording( joypadStruct *joypadData , joypadRecording *recording)
{
    static joypadStruct nullJoypad;
    memset( &nullJoypad , 0x00 , sizeof(joypadStruct));

    if(recording->record == NULL)
    {
        *joypadData = nullJoypad;
        return -1;
    }

    if(recording->readPos == NULL)
    {
        *joypadData = nullJoypad;
        return EOF;
    }

    *joypadData = recording->readPos->data;
    recording->readPos = recording->readPos->next;

    return 0;
}

//-------------------------------------------

/*********************************************
    freeJoypadRecording:
**********************************************/

int freeJoypadRecording( joypadRecording *recording)
{
    joypadRecordEntry *cEntry;  //Current entry
    joypadRecordEntry *nextEntry;
    for(cEntry = recording->record;
        cEntry != NULL;
        cEntry = nextEntry)
    {
        nextEntry = cEntry->next;
        free(cEntry);
    }

    recording->record = NULL;
    recording->last = NULL;

    return 0;
}

//-------------------------------------------

/*********************************************
    freeGameReplay:
**********************************************/
int freeGameReplay(void)
{
    return freeJoypadRecording(&gameReplay);
}

//-------------------------------------------

/*********************************************
    loadJoypadRecording:
    Load a joypad recording from file
    RETURNS:
        0 : No errors
       -1 : Failed to load the file
**********************************************/

int loadJoypadRecording( joypadRecording *recording , char *filePath)
{
#ifdef DEBUG
    printf("[DEBUG] Loading replay file \"%s\"\n",filePath);
#endif
    FILE *fp;
    if( (fp = fopen( filePath , "rb")) == NULL)
    {
        fprintf(stderr,"[ERROR] opening replay file: %s\n" , filePath);
        return -1;
    }

    clearerr(fp);
    joypadStruct joypadData;
    unsigned char dataBuffer[sizeof(joypadStruct)];
    int elementsRead;
    while(1)
    {
        elementsRead = fread( &dataBuffer , 1 , sizeof(joypadStruct) , fp);
    
        if(feof(fp))
            break;
        
        if(elementsRead != sizeof(joypadStruct)
           ||  ferror(fp))
        {
            fprintf(stderr,"[ERROR] reading replay file: %s\n", filePath);
            fclose(fp);
            freeGameReplay();
            return -1;
        }   

        //TODO: Deal with endianness
        memcpy( &joypadData , dataBuffer , sizeof(joypadStruct));     

        //Add input to the list
        addJoypadRecordEntry(&gameReplay , joypadData);
        
    }

    fclose(fp);
    return 0;
}

//-------------------------------------------

/*********************************************
    loadGameReplay
**********************************************/

int loadGameReplay(char *filePath)
{
    freeGameReplay();
    return loadJoypadRecording( &gameReplay , filePath);
}

//-------------------------------------------

/*********************************************
    saveJoypadRecording:
    Save joypad recording to file
    RETURNS:
        0 : No errors
       -1 : Failed to save the file
**********************************************/

int saveJoypadRecording( joypadRecording *recording , char *filePath)
{
    FILE *fp;
    if( (fp = fopen( filePath , "wb")) == NULL)
    {
        fprintf(stderr,"[ERROR] opening replay file: %s\n" , filePath);
        return -1;
    }

    clearerr(fp);
    joypadRecordEntry *cEntry;  //Current entry
    int bytesWritten;

    for(cEntry = recording->record;
        cEntry != NULL;
        cEntry = cEntry->next)
    {

        bytesWritten = fwrite(  &cEntry->data 
                              , 1
                              , sizeof(joypadStruct)
                              , fp);

        if(   bytesWritten != sizeof(joypadStruct)
           || ferror(fp))
        {
            fprintf(stderr,"[ERROR] saving replay file: %s\n", filePath);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

//-------------------------------------------

/*********************************************
    saveGameReplay:
**********************************************/

int saveGameReplay(char *filePath)
{
    return saveJoypadRecording( &gameReplay , filePath);
}

/*********************************************
    noControllerInput
    Returns:
        1 if no button, key or stick is pressed or held in this controller
        0 otherwise
/*********************************************/

int noControllerInput( controllerStruct *controller)
{
    char *data = (char *)&controller->raw;
    int i;
    for( i = 0 ; i < sizeof(joypadStruct) ; i++)
    {
        if( 0x00 != *(data+i))
            return 0;
    }
    return 1;
}

//-------------------------------------------


/*********************************************
    **DEBUG**
**********************************************
    printGameReplayInfo
**********************************************/

int  printJoypadRecordingInfo(joypadRecording *recording)
{
#ifndef DREAMCAST
    int i = 0;
    joypadRecordEntry *cEntry;  //Current entry
    for(cEntry = recording->record;
        cEntry != NULL;
        cEntry = cEntry->next)
    {
        i++;
    }
    printf("[DEBUG] Current Replay size: %d Bytes\n",i*sizeof(joypadRecordEntry));
#endif
    return 0;
}

//-------------------------------------------


/*********************************************
    **DEBUG**
**********************************************
    printGameReplayInfo
**********************************************/

int printGameReplayInfo(void)
{
    return printJoypadRecordingInfo(&gameReplay);
}

