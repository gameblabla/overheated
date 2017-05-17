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
   game.c
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "SDL_utils.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "d_spawn.h"
#include "d_background.h"
#include "d_stage.h"
#include "d_bmFont.h"
#include "d_input.h"
#include "globalVars.h"
#include "d_filePaths.h"
//
#include "audio.h"
#include "video.h"
#include "sfx.h"
#include "l_music.h"
#include "music.h"
#include "loadData.h"
#include "input.h"
#include "objects.h"
#include "enemyBullet.h"
#include "enemies.h"
#include "player.h"
#include "spawn.h"
#include "background.h"
#include "SDL_Text.h"
#include "announcer.h"
#include "time.h"
#include "gameOver.h"
#include "l_stages.h"
#include "collisions.h"
#include "score.h"
#include "scoreboard.h"
#include "nameInput.h"

//
#define DEBUG
#define FADE_STEP 5

/*******************************************/

int  gameType;
static int  gameStatus;

#define ON_GOING 0
#define TIME_UP 1
#define GAME_OVER 2
#define MISSION_COMPLETE 3
#define END_STAGE 4

#define END_GAME -1
#define EXIT_STAGE -2

static long gameTime; //Time in Frames

static int modeTimes[]= //Time in milliseconds
{
    00,
    2*60*1000, // 2 MINUTES MODE
    5*60*1000, // 5 MINUTES MODE
    2*60*1000, // 2 MINUTES REPLAY MODE
    5*60*1000  // 5 MINUTES REPLAY MODE
};

static char *modeReplayFiles[] =
{
     "INVALID MODE"
    ,TWO_MINMODE_REPLAY_FILE
    ,FIVE_MINMODE_REPLAY_FILE 
    ,TWO_MINMODE_REPLAY_FILE
    ,FIVE_MINMODE_REPLAY_FILE 
};

static int modeMusicTracks[] = 
{
     -1              // INVALID MODE
    ,TWO_MIN_TRACK  // 2 MINUTES MODE
    ,FIVE_MIN_TRACK // 5 MINUTES MODE
    ,TWO_MIN_TRACK  // 2 MINUTES MODE
    ,FIVE_MIN_TRACK // 5 MINUTES MODE
};

/*******************************************
    endStage
*******************************************/
void endStage(void)
{
    if(   gameStatus != GAME_OVER
       && gameStatus != TIME_UP   )
    {
        lockScore();
        gameStatus = END_STAGE;
    }
    return;
}

//-------------------------------------------

/*******************************************
    startGame
*******************************************/

int startGame(int type)
{
	//init game variables
    gameType = type;
    gameStatus = ON_GOING;
    // Convert time from ms to frames
	gameTime = time_mstf(modeTimes[gameType]);
    //
    initGameScore(gameType);
    //
    setPlayerLives(PL_INIT_LIVES);

    //delete previous game replay on memory
    freeGameReplay();
    
    if(   gameType == TWO_MINUTES_MODE
       || gameType == FIVE_MINUTES_MODE)
    {
        startJoypadRecording();
#ifdef DEBUG
        printf("[DEBUG] Start game recording\n");
#endif
    }

    if(   gameType == TWO_MINUTES_REPLAY
       || gameType == FIVE_MINUTES_REPLAY)
    {
        if(loadGameReplay(modeReplayFiles[gameType]) != 0)
        {
            return -1;
        }
#ifdef DEBUG
        printGameReplayInfo();
#endif
        startJoypadReplay();
    }

	//start the game
	initPlayer();

    playMusicTrack(modeMusicTracks[gameType]);

    int i;
    char *startMsg = "GET READY!";
    for( i = 0 ; 
         i < stagesNumber() && gameStatus != END_GAME ;
         i++)
    {
        gameStatus = ON_GOING;
        setAdvertTime(0); //Clear advertisements
        unlockScore();
	    game( startMsg , i);
        startMsg = NULL;
        
    }

    freeMusic();

#ifdef DEBUG
    printGameReplayInfo();
#endif

    if(   gameType == TWO_MINUTES_MODE
       || gameType == FIVE_MINUTES_MODE)
    {
        stopJoypadRecording();
        if( rankScore(getScore(),whichScoreBoard(gameType)) == 1)
            if(saveGameReplay(modeReplayFiles[gameType]) == 0)
            {
#ifdef DEBUG
            printf("[DEBUG] Game replay saved\n");
#endif
            }
    }

    if(   gameType == TWO_MINUTES_REPLAY
       || gameType == FIVE_MINUTES_REPLAY)
    {
        stopJoypadReplay();
    }

    freeGameReplay();

    //Name Input screen
    if(    gameType != TWO_MINUTES_REPLAY
        && gameType != FIVE_MINUTES_REPLAY
        && rankScore(getScore(),whichScoreBoard(gameType)) > 0)
    {
        nameInputScreen(getScore());
    }

}

//-------------------------------------------

/*******************************************
    game
*******************************************/

int game(char *startMsg , int stageNumber)
{

	//get the game surface
	SDL_Surface *gameScreen = getGameFb();

    //Init fade value and create mask surface
    unsigned fadeValue = SDL_ALPHA_OPAQUE;
    SDL_Surface *fadeMask = SDL_CreateRGBSurface( SDL_SWSURFACE|SDL_RLEACCEL|SDL_SRCALPHA, 
					   GM_SCREEN_W, GM_SCREEN_H , SCREEN_BPP, 
					   0, 0, 0, 0);
    SDL_SetAlpha( fadeMask , SDL_RLEACCEL|SDL_SRCALPHA , fadeValue);

	//Load game stage
	GMstage stage;

    char *stagePath = getStagePath( stageNumber);
    if( stagePath == NULL)
    {
        return -1;
    }
	loadStage( stagePath ,&stage);
    /*Load audio effects*/
	loadAudioSfx(SFX_FILE_PATH);

	//init objects
	spawn_INIT();
	initObjectArray( &OAM_enemies , ENEMIES_OAM_SIZE );
	initObjectArray( &OAM_PlBullets , PL_BULLETS_OAM_SIZE );
	initObjectArray( &OAM_effects , EFFECTS_OAM_SIZE  );
	initObjectArray( &OAM_enemiesBullets , ENEMIES_BULLETS_OAM_SIZE);
	initObjectArray( &OAM_bonus , BONUS_OAM_SIZE);
	initObjectArray( &OAM_events , EVENTS_OAM_SIZE);

    //Load game fonts
	bmFont scoreFont;
	loadBmFont(SCORE_FONT_FILE , &scoreFont);

	bmFont advertsFont;
	loadBmFont(ADVERTS_FONT_FILE,&advertsFont);
	loadAdvertsFont(ADVERTS_FONT_FILE);

    //Stage start message
    if( startMsg != NULL)
    {
	    setAdvert(  startMsg
			      , gameScreen->w/2 - strlen(startMsg)*advertsFont.size.w/2
			      , 100, 80);
    }

	char stageLineStr[100];
#define SCREEN_TXT_BUFF_SIZE 100
    char screenTextBuffer[SCREEN_TXT_BUFF_SIZE];

    enum { STD_GAMEOVER_FRAMES = 30*FPS, REPLAY_GAMEOVER_FRAMES= 10*FPS};
    long gameOverFrames;

    switch(gameType)
    {
        case TWO_MINUTES_MODE:
        case FIVE_MINUTES_MODE:
            gameOverFrames =  STD_GAMEOVER_FRAMES;
        break;

        case TWO_MINUTES_REPLAY:
        case FIVE_MINUTES_REPLAY:
            gameOverFrames = REPLAY_GAMEOVER_FRAMES;
    }

	int mspf = 1000/FPS; //milisecond per frame
	long ms = 0; //milisecond passed since SDL was initialized
	int sleep_delay = 0;

	controllerStruct controller;
	initController(&controller);

	int spawnInd=0;		//Normal spawn index
	int espSpawnInd = 0;	//SPecial spawn index
	int i=0;
	int tmpInt= 0;

    //Main loop
	while(gameStatus >= 0)
	{
		//Frame counter
		ms = SDL_GetTicks();

		//Read controller input
		readController(&controller);

		//clear game screen
		//SDL_FillRect(gameScreen, 0, SDL_MapRGB(gameScreen->format, 0, 0, 0));
	
		//Backgrounds
		for(i= 0 ; i < stage.backgrounds.number ; i++)
		{
            //
            manageBackgroundCollisions( &OAM_PlBullets , stage.backgrounds.bg+i);
            //
			execBgScript( stage.backgrounds.bg+i , stage.frame);
			updateBackgroundScroll( stage.backgrounds.bg+i , gameScreen);
		}

        //Screen Fade in/out
        if( fadeValue != SDL_ALPHA_TRANSPARENT)
        {
           SDL_fadeSurface( fadeMask , fadeValue , gameScreen);
        }

		//Player
        manageObjectsCollisions( OAM_enemies.object , OAM_enemies.size
						        , OAM_player.object , OAM_player.size);
	    runPlayer(&controller);

        drawObjects( &OAM_player , gameScreen);

		//Events
		manageObjects(&OAM_events);

		//Player bullets
		manageObjectsCollisions(  OAM_PlBullets.object , OAM_PlBullets.size
					, OAM_enemies.object , OAM_enemies.size);
		manageObjects(&OAM_PlBullets);

		//bonus
		manageObjectsCollisions(  OAM_bonus.object , OAM_bonus.size
					, OAM_player.object , OAM_player.size);
		manageObjects(&OAM_bonus);
		manageObjectsAnim( &OAM_bonus);

		//Enemy bullets
		manageObjectsAnim( &OAM_enemiesBullets);
		manageObjectsCollisions( OAM_enemiesBullets.object , OAM_enemiesBullets.size
					, OAM_player.object , OAM_player.size);
		manageObjects(&OAM_enemiesBullets);

		//Enemies
		manageObjects(&OAM_enemies);
		manageObjectsAnim( &OAM_enemies);

        //Especial effects
		manageObjectsAnim( &OAM_effects );
		manageObjects(&OAM_effects);

		//Special spawns

		tmpInt = spawnObjTable(stage.specialSpawns+espSpawnInd , stage.espSpawnCount , UNSTOPPABLE);
		if( tmpInt != SPAWN_TABLE_END)
			stage.espSpawnCount++;
		if( tmpInt == OBJ_SPAWNED)
			espSpawnInd++; //Advance to the next spawn

		//Normal spawns

		tmpInt = spawnObjTable(stage.spawnTable+spawnInd , stage.spawnCounter , STOPPABLE);

		if(!spawnIsHalted())
			stage.spawnCounter++;
		if( tmpInt == OBJ_SPAWNED)
			spawnInd++; //Advance to the next spawn
		if( tmpInt == SPAWN_TABLE_END)
		{
			spawnInd = 0; //Reset normal spawns
			stage.spawnCounter = 0;
		}

        //Update stage frames
	    stage.frame++;

		//Draw objects
		drawObjects( &OAM_enemies , gameScreen);
		drawObjects( &OAM_bonus , gameScreen);
		drawObjects( &OAM_effects , gameScreen);
		drawObjects( &OAM_PlBullets , gameScreen);
        drawObjects( &OAM_enemiesBullets , gameScreen);

        //Time
		sprintf(screenTextBuffer,"-%02d\"%02d'%02d-"
			,(int)( time_ftms(gameTime)/(1000*60))
			,(int)( (time_ftms(gameTime)%(1000*60))/1000)
			,(int)( ((time_ftms(gameTime)%(1000*60))%1000)/10));
		SDL_Text(screenTextBuffer , &scoreFont , 20 , 26 , gameScreen);

        //Player Speed
        memset( screenTextBuffer , '>' , getPlSpeedLevel());
        screenTextBuffer[getPlSpeedLevel()] = '\0';
		SDL_Text(screenTextBuffer , &scoreFont , 20 , 40 , gameScreen);

        //Score board
        sprintf(screenTextBuffer,"SC: %06d", getScore() );
        SDL_Text(screenTextBuffer,  &scoreFont , 20 , 16 ,  gameScreen);

        sprintf(screenTextBuffer,"HI: %06d", getHighScore() );
        SDL_Text(screenTextBuffer,  &scoreFont , 200 , 16 ,  gameScreen);

        //Player lives
        sprintf(screenTextBuffer,"Lx%02d" , getPlayerLives());
        SDL_Text(screenTextBuffer, &scoreFont , 270 , 220 , gameScreen);

        //Replay mode
        if(   gameType == TWO_MINUTES_REPLAY
           || gameType == FIVE_MINUTES_REPLAY)
        {
            if(controller.pressed.START)	//End replay
                gameStatus = END_GAME;

            sprintf(screenTextBuffer,">>REPLAY" , getPlayerLives());
            SDL_Text(screenTextBuffer, &scoreFont , 20 , 220 , gameScreen);
        }

		announcer(gameScreen);
        //Game state
        switch( gameStatus)
        {
                case ON_GOING:

                    //Initial fade in
                    if( fadeValue > SDL_ALPHA_TRANSPARENT)
                        fadeValue -= FADE_STEP;

                    //DEBUG
			        //if( controller.pressed.SELECT)
                        //endStage();

                    if(getPlayerLives() == 0)
                    {
                        lockScore();
                        gameStatus = GAME_OVER;
                    }
                    else if( gameTime > 0)
                    {
                        gameTime--;
                    }
			        else
			        {
                        //Time up
                        lockScore();
                        gameStatus = TIME_UP;
                        playerLeave();
			        }

                    break;

                case GAME_OVER:

                    //
                    gameOverScreen( "GAME OVER" , &advertsFont , gameScreen);
			        if( controller.pressed.START
                        || 0 == gameOverFrames )	//End the game
                    {
                        gameStatus = END_GAME;
                    }
                    else gameOverFrames--;
                        

                    break;

                case TIME_UP:

                    gameOverScreen( "TIME UP" , &advertsFont , gameScreen);
			        if( controller.pressed.START
                        || 0 == gameOverFrames )	//End the game
                    {
                        gameStatus = END_GAME;
                    }
                    else gameOverFrames--;
        
                    break;

                case MISSION_COMPLETE:

                    missionCompleteScreen( &advertsFont , gameScreen);
			        if( controller.pressed.START
                        || 0 == gameOverFrames )	//End the game
                    {
                        gameStatus = END_GAME;
                    }
                    else gameOverFrames--;

                    break;

                case END_STAGE:

                    //Mission complete!
                    if( isFinalStage(stageNumber))
                    {
                            gameStatus = MISSION_COMPLETE;
                            playerLeave();
                            break;
                    }

                    //Fade the background out and
                    //end the stage
                    if( fadeValue >= SDL_ALPHA_OPAQUE)
                    {
                        gameStatus = EXIT_STAGE;
                        break;
                    }
                    fadeValue += FADE_STEP;

                    break;
        }

        /*Update screen*/
        updateVideoScreen();

        //timing delay
		sleep_delay = mspf - (SDL_GetTicks()-ms);
		if(sleep_delay > 0)
			SDL_Delay( sleep_delay );

	}//End main loop

	freeStage( &stage);
	freeBmFont(&scoreFont);
	freeBmFont(&advertsFont);
	freeAdvertsFont();
	freeAudioSfx();

	freeObjectArray( &OAM_enemies);
	freeObjectArray( &OAM_PlBullets);
	freeObjectArray( &OAM_effects);
	freeObjectArray( &OAM_enemiesBullets);
	freeObjectArray( &OAM_bonus);
	freeObjectArray( &OAM_events);

    SDL_FreeSurface(fadeMask);
	return 0;
}
