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
   trackPlayer.h
*********************************************/

#ifndef TRACK_PLAYER_H
#define TRACK_PLAYER_H

#define MAX_SECTION_PATH_LEN 1024
struct trackSection_t
{
    char filePath[MAX_SECTION_PATH_LEN]; //Music file path (e.g ./data/*.ogg)
    int  loops;                          //Number of loops for this section 
                                         //n >= 0 : Play n times
                                         //-1     : Infinite loop
    
    void *next;                          //Next segment
};
typedef struct trackSection_t trackSection_t;

struct track_t
{
    trackSection_t *sections;     //Linked List of segments
    trackSection_t *lastSection;
    //
    int loops;                   //Number of loops for this track
                                 //n >= 0: play n times
                                 //-1    : Infinite loops
    //Track status
    trackSection_t *cSection;     //Current section
    int  s_loopCounter;           //Section loop counter
    int  t_loopCounter;           //Track loop counter
    long sectionTicks;            //position of the currently playing section
                                  //in millisecond
};
typedef struct track_t track_t;

/********************************************
    Private:
********************************************/

#ifdef TRACK_PLAYER_C
void addTrackSection(track_t *track ,const trackSection_t *section);
void freeTrack(track_t *track);
void updateTrackPlayer(void);
void updateSectionTicks(void); //Update the track's time counters
void decLoopCounter(int *counter);
void advanceNextSection(track_t *track);
void playCurrentSection(void);
void haltTrack(void);

#endif

/********************************************
    Public:
********************************************/

track_t *loadTrack(char *filePath); //Load track from file ( .track)
void     freeTrack(track_t *track); //Free track from memory
/**/
void 	 playTrack(track_t *track); //Load track on the player and start playback
track_t *ejectTrack(void);          /*
                                      ejectTrack:
                                        halt playback and eject track from the player
										(like when you eject a tape from a VCR)
                                     RETURNS:
                                        a pointer to the track that was "inside" the player
										NULL if the player was "empty".
                                     NOTES:
                                        *You SHOULD use this before freeing a track.
                                         Freeing a track when is still loaded on the player is
                                         a bad idea.
										*if you don't catch the pointer being returned
										 and you don't have the track's address stored in other place
                                         you will have a MEMORY LEAK.
									 */
/**/
track_t *getCurrentTrack(void);     //Returns a pointer to the track loaded on the player
                                    //NULL if there is no track
/**/
void    pauseTrack(void);           //Pause playback;
void    resumeTrack(void);          //Resume trackPlayback
/**/
int     trackPaused(void);        //Returns value != 0 if the playback is paused
int     trackHalted(void);          //Returns value != 0 if the playback is halted
int     trackInsidePlayer(void);    //Returns value != 0 if there is a track inside the player


void    setTrackPlayerVolume(int volume);
int     getTrackPlayerVolume(void);
void    incMusicVolume(int amount);  //Increment music volume by the specified amount 
                                     //*You can also use negative numbers to decrement it
									
/*DEBUG/Info*/
void printfTrackInfo(const track_t *track);


#endif
