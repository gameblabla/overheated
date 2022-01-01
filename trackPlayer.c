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
   trackPlayer.c
*********************************************/

#ifndef TRACK_PLAYER_C
#define TRACK_PLAYER_C
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
//
#include "system_var_type.h"
#include "str_utils.h"
#include "d_audio.h"
#include "audio.h"
#include "trackPlayer.h"

/*********************************************/

#define TERMINAL_ERROR_MESSAGES


#define SYSTEM_VARS_NUMBER 0 //See str_utils.h
#define STR_BUFFER_MAXSIZE 100 //See str_utils.h
#define ERROR_HANDLING exit(-1);

#define INF_LOOP -1

/*********************************************/

int trackPlayerVolume = MIX_MAX_VOLUME / 2;

static track_t *currentTrack = NULL;
#ifndef DREAMCAST
static Mix_Music *music = NULL;
#endif
static int playbackHalted = 1 ;
static int playbackPaused = 0 ;
static int trackInside = 0 ;                        //Track loaded/inside player
static long startTicks = 0 ;                        //SDL_GetTicks() value 
                                                    //the last time the current section started/resumed playing

char lastFilePath[MAX_SECTION_PATH_LEN] = "\0";     //Path of the last music file loaded

/********************************************
    addTrackSection
    add section to the section list
*********************************************/

void addTrackSection(track_t *track ,const trackSection_t *section)
{
    if(  track == NULL
       ||section == NULL)
    {
        return;
    }

    trackSection_t *newSection = malloc(sizeof(trackSection_t));
    *newSection = *section;
    newSection->next = NULL;

    if( track->sections == NULL)
    {
        track->sections = newSection; 
    }
    else
    {
        track->lastSection->next = newSection;
    }

    track->lastSection = newSection;
    
    return;
}


/********************************************
    loadTrack
    Load Track from file
*********************************************/

track_t *loadTrack(char *filePath)
{
	#ifndef DREAMCAST
	systemVar_t *systemVars = NULL;

#ifdef DEBUG
        printf("[DEBUG] Loading track file \"%s\"\n",filePath);
#endif

    FILE *inFp;
	if( ( inFp = fopen( filePath , "rb")) == NULL)	
	{
		fprintf(stderr , "[ERROR] opening %s\n",filePath);
		ERROR_HANDLING
	}
    track_t *newTrack = calloc(sizeof(track_t),1);
    trackSection_t newSection;

	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer    

    //
    if( searchString(inFp,"trackFmtVer",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[ERROR] \"trackFmtVer\" missing\n");
#endif
		    ERROR_HANDLING
		}

    FETCH_NEXT_STRING
    READ_NEXT_STRING

#ifdef DEBUG
        printf("\tTrack file format version: %d\n",atoi(StrBuffer));
#endif

    if( searchString(inFp,"header",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[ERROR] \"header\" missing\n");
#endif
		    ERROR_HANDLING
		}

    if( searchString(inFp,"trackLoops",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[ERROR] \"trackLoops\" missing\n");
#endif
		    ERROR_HANDLING
		}

    FETCH_NEXT_STRING
    READ_NEXT_STRING
    
    newTrack->loops = atoi(StrBuffer);

    int sectionsNum = 0;
    while( searchString(inFp,"section",':') == 0)
    {        
        if( searchString(inFp,"file",':') != 0)
        {
#ifdef TERMINAL_ERROR_MESSAGES
    			fprintf(stderr,"[ERROR] Bad track section entry\n");
#endif
    		    ERROR_HANDLING
        }

    		FETCH_NEXT_STRING
    		READ_NEXT_STRING

            strncpy( newSection.filePath
                    , StrBuffer
                    , MAX_SECTION_PATH_LEN);

        if( searchString(inFp,"loops",':') != 0)
        {
#ifdef TERMINAL_ERROR_MESSAGES
    			fprintf(stderr,"[ERROR] Bad track section entry\n");
#endif
    		    ERROR_HANDLING
        }

    		FETCH_NEXT_STRING
    		READ_NEXT_STRING

            newSection.loops = atoi(StrBuffer);

        addTrackSection( newTrack , &newSection);
        sectionsNum++;        
    }/*End while*/
#ifdef DEBUG
        printf("\t%d Sections Loaded\n",sectionsNum);
#endif

    //Init track status variables
    newTrack->cSection = newTrack->sections;
    newTrack->s_loopCounter = newTrack->cSection->loops;
    newTrack->t_loopCounter = newTrack->loops;
    newTrack->sectionTicks = 0;
    

    fclose(inFp);
    return newTrack;
    #else
    return 0;
    #endif
}

/********************************************
    printfTrackInfo
*********************************************/

void printfTrackInfo(const track_t *track)
{
    trackSection_t *cSection; //Current section
    int sectionsNum = 0;
    for( cSection = track->sections;
         cSection != NULL ; 
         cSection = cSection->next , sectionsNum++)
    {
        printf("\tSection %d:\n"
               "\t\tFile: \"%s\"\n"
               "\t\tLoops: %d\n"
               ,sectionsNum
               ,cSection->filePath
               ,cSection->loops);                
    }
    return;
}


/********************************************
    freeTrack
*********************************************/

void freeTrack(track_t *track)
{
    trackSection_t *cSection; //Current section
    trackSection_t *nextSection; //Next section

    for( cSection = track->sections;
         cSection != NULL ; 
         cSection = nextSection)
    {
        nextSection = cSection->next;
        free(cSection);
    }

    free(track);
    return;
}
/*********************************************
    pauseTrack
**********************************************/
void pauseTrack(void)
{
    if(trackHalted() == 0)
    {
        updateSectionTicks();
        playbackPaused = 1;
        #ifndef DREAMCAST
        Mix_PauseMusic();
        #endif
    }
}

/*********************************************
    resumeTrack
**********************************************/
void resumeTrack(void)
{
    if(trackHalted() == 0)
    {
        startTicks = SDL_GetTicks();
        playbackPaused = 0;
        #ifndef DREAMCAST
        Mix_ResumeMusic();
        #endif
    }
}

/********************************************
    updateTrackTime
/********************************************/
void updateSectionTicks(void)
{
    if(     trackPaused() == 0
        &&  trackHalted() == 0)
    {
        currentTrack->sectionTicks += SDL_GetTicks() - startTicks;    
        startTicks = SDL_GetTicks();
    }
}

/********************************************
    playCurrentSection
*********************************************/

void playCurrentSection(void)
{
	#ifndef DREAMCAST
    if( currentTrack->s_loopCounter == 0)
    {
        playbackHalted = 1;
        return;
    }

    if(   music == NULL
       || strcmp(   lastFilePath 
                   , currentTrack->cSection->filePath) != 0)
    {
        if(music != NULL)
        {
            Mix_FreeMusic(music);
            music = NULL;
        }        
        music = Mix_LoadMUS(currentTrack->cSection->filePath);
    }

    Mix_PlayMusic(music, 1);

    //DEV
    /*Seems like setMusiPos is bugged or something
      So for the time being all sections will start at 0*/
    currentTrack->sectionTicks = 0;
    //END DEV

    //Mix_SetMusicPosition(currentTrack->sectionTicks/1000);
    //
    strncpy( lastFilePath
            ,currentTrack->cSection->filePath
            ,MAX_SECTION_PATH_LEN);
    
    startTicks = SDL_GetTicks();
    playbackHalted = 0;
    #endif
}

/********************************************
    decLoopCounter
*********************************************/

void decLoopCounter(int *counter)
{
    if( *counter > 0)
        (*counter)--;  
}

/********************************************
    playTrack
/********************************************/

void playTrack(track_t *track)
{
	#ifndef DREAMCAST
    currentTrack = track;
    trackInside = 1;
    //DEV
     Mix_HookMusicFinished(&updateTrackPlayer);
    //DEV
    playCurrentSection(); 
    if(trackHalted())
    {
        updateTrackPlayer();
    }
    #endif
}

/*********************************************
    haltTrack()
**********************************************/
void haltTrack(void)
{
	#ifndef DREAMCAST
    updateSectionTicks();
    Mix_HaltMusic();
    playbackHalted = 1;
    #endif
    return;
}

/********************************************
    ejectTrack
/********************************************/
track_t *ejectTrack(void)
{
	#ifndef DREAMCAST
    trackInside = 0;
    if( currentTrack == NULL)
        return currentTrack;

    haltTrack();
    lastFilePath[0] = '\0';
    if(music != NULL)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
    //    
    track_t *track = currentTrack;
    currentTrack = NULL;
    //
    return track;
    #else
    return 0;
    #endif
    
}

/********************************************
    advanceNextSection
    Advance the track to the next section
/********************************************/

void advanceNextSection(track_t *track)
{
    if(track == NULL)
        return;

    if( track->cSection->next == NULL)
        return;

    track->cSection = track->cSection->next;
    track->s_loopCounter = track->cSection->loops;
    track->sectionTicks = 0;
}
/*********************************************
    trackPaused
**********************************************/
int trackPaused(void)
{
    return playbackPaused;
}
/*********************************************
    trackHalted
**********************************************/
int trackHalted(void)
{
    return playbackHalted;
}
/*********************************************
    trackInsidePlayer
**********************************************/
int trackInsidePlayer(void)
{
    return trackInside;
}

/********************************************
    updateTrackPlayer
*********************************************/

void updateTrackPlayer(void)
{
    if( trackHalted())
        return;

    if( currentTrack == NULL)
        return;

	#ifndef DREAMCAST
    if(Mix_PlayingMusic())
        return;
	#endif

    decLoopCounter(&currentTrack->s_loopCounter);
    if(currentTrack->s_loopCounter)
    {   //Current section loop
        playCurrentSection();   
        return;
    }
    else
    {   
        if( currentTrack->cSection->next == NULL)
        {   //End of track reached
            decLoopCounter(&currentTrack->t_loopCounter);
            if( currentTrack->t_loopCounter)
            {
                //Next Track loop
                currentTrack->cSection = currentTrack->sections;
                currentTrack->s_loopCounter = currentTrack->cSection->loops;
                playCurrentSection();
                return;               
            }
            else
            {
                //No more music
                haltTrack();
                return;
            }
        }
        //Load the next section
        advanceNextSection(currentTrack);
        playCurrentSection();
        if(trackHalted())
        {
            updateTrackPlayer();
        }       
    }
}

/********************************************
    setTrackPlayerVolume
*********************************************/

void setTrackPlayerVolume(int volume)
{
    if( volume < 0)
        volume = 0;
    else if( volume > MIX_MAX_VOLUME)
        volume = MIX_MAX_VOLUME;

    trackPlayerVolume = volume;
    #ifndef DREAMCAST
    Mix_VolumeMusic(volume);
    #endif
}

/********************************************
    getTrackPlayerVolume
*********************************************/

int getTrackPlayerVolume(void)
{
    return trackPlayerVolume;
}

