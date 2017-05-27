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
   gameConfig.c
   Configure and initialize the engine sybsystems
*********************************************/


//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//
#include "d_gameConfig.h"
#include "globalDefines.h"
#include "globalVars.h"
#include "loadData.h"
#include "audio.h"
#include "music.h"
#include "video.h"

char config_gamepath[128];
char dataPath[128];
char scorePath[128];
char replayPath[128];
char configPath[128];

//-------------------------------------------
#define GAME_CFG_FILE_PATH "./data/config/game.cfg"
//-------------------------------------------

gameCfgStruct gameConfiguration;

#define VIDEO_RES_MODES 5
int resModesTable[VIDEO_RES_MODES][5] =
{
      {/*X resolution*/ 320 ,/*Y resolution*/240  ,/*Scale Factor*/ 1 ,/*Game Area X,Y*/  00, 00} 
    , {/*X resolution*/ 640 ,/*Y resolution*/480  ,/*Scale Factor*/ 2 ,/*Game Area X,Y*/  00, 00}  
    , {/*X resolution*/1024 ,/*Y resolution*/768  ,/*Scale Factor*/ 3 ,/*Game Area X,Y*/  32, 24}  
    , {/*X resolution*/1280 ,/*Y resolution*/720  ,/*Scale Factor*/ 3 ,/*Game Area X,Y*/ 160, 00} 
    , {/*X resolution*/1920 ,/*Y resolution*/1080 ,/*Scale Factor*/ 4 ,/*Game Area X,Y*/ 320, 00} 
};

char *resModesStrInfo[VIDEO_RES_MODES] =
{
     "320x240"
    ,"640x480"
    ,"1024x768"
    ,"1280x720"
    ,"1920x1080"
};

/*********************************
setResMode
Set resolution mode
Returns <0 if the mode is invalid
**********************************/
int setResMode(unsigned mode)
{
    if(mode > VIDEO_RES_MODES-1)
        return -1;

    setResolution( resModesTable[mode][0]  //X
                  ,resModesTable[mode][1]);//Y
    setScaleFactor(resModesTable[mode][2]); 
    setGameAreaPos( resModesTable[mode][3]
                   ,resModesTable[mode][4]);

    printf("[INFO] Using resolution mode %u (%ux%u)\n"
            ,mode
            ,resModesTable[mode][0]
            ,resModesTable[mode][1]
           );

    return 0;
}

/**********************************
incResMode
**********************************/
void incResMode(void)
{
    gameConfiguration.resMode = (gameConfiguration.resMode+1)
                                %VIDEO_RES_MODES;
    return;
}

/**********************************
decResMode
**********************************/
void decResMode(void)
{
    gameConfiguration.resMode = (gameConfiguration.resMode-1)
                                %VIDEO_RES_MODES;
    return;
}

/**********************************
Get the information string ("name")
of each mode
***********************************/
char *getResModesStrInfo(void)
{
    if(gameConfiguration.resMode > VIDEO_RES_MODES-1)
        return NULL;

    return resModesStrInfo[gameConfiguration.resMode];
}

/**********************************
Get information about the resolution mode
( X and Y resolution values)
Returns <0 if the mode is invalid
***********************************/

int getResModeInfo(unsigned mode , int *x , int *y)
{
    if( x == NULL || y == NULL)
        return;

    if(mode > VIDEO_RES_MODES-1)
        return -1;

    *x = resModesTable[mode][0];
    *y = resModesTable[mode][1];

    return 0;
}

/********************************************
    loadGameCfg
    Load game configuration

    *You should call this function before
     using any other function in this module!
*********************************************/

int loadGameCfg(void)
{
#ifdef LINUX
	char homePath[128];
	char configPath[128];
	snprintf(homePath, sizeof(homePath), "%s", getenv("HOME"));
	snprintf(configPath, sizeof(configPath), "%s/.config", configPath);
	snprintf(config_gamepath, sizeof(config_gamepath), "%s/.config/overheated", homePath);
	snprintf(dataPath, sizeof(dataPath), "%s/data", config_gamepath);
	snprintf(scorePath, sizeof(scorePath), "%s/data/score", config_gamepath);
	snprintf(replayPath, sizeof(replayPath), "%s/data/replay", config_gamepath);
	snprintf(configPath, sizeof(configPath), "%s/data/config", config_gamepath);
	
	if(access( configPath, F_OK ) == -1)  mkdir(configPath, 0755);
	if(access( config_gamepath, F_OK ) == -1)  mkdir(config_gamepath, 0755);
	if(access( dataPath, F_OK ) == -1)  mkdir(dataPath, 0755);
	if(access( scorePath, F_OK ) == -1)  mkdir(scorePath, 0755);
	if(access( replayPath, F_OK ) == -1)  mkdir(replayPath, 0755);
	if(access( configPath, F_OK ) == -1)  mkdir(configPath, 0755);
#else
	snprintf(config_gamepath, sizeof(config_gamepath), ".");
#endif
	snprintf(dataPath, sizeof(dataPath), "%s/data", config_gamepath);
	snprintf(scorePath, sizeof(scorePath), "%s/data/score", config_gamepath);
	snprintf(replayPath, sizeof(replayPath), "%s/data/replay", config_gamepath);
	snprintf(configPath, sizeof(configPath), "%s/data/config", config_gamepath);
	
    if(loadGameCfgFile( GAME_CFG_FILE_PATH , &gameConfiguration))
    {
        fprintf( stderr , "[ERROR] Loading game configuration\n");
        exit(1);
    }
    
	gameConfiguration.fullscreen = gameConfiguration.fullscreen ? 1 : 0;
	gameConfiguration.scanlines = gameConfiguration.scanlines ? 1 : 0;
#ifdef GCW0
    gameConfiguration.scanlines = 0;
#endif
    /*Config video subsystem*/
    gameConfiguration.fullscreen ? setVideoFullscreen() : setVideoWindowed();
    gameConfiguration.scanlines  ? videoEnableScanlines() : videoDisableScanlines();
    setVideoScanLines(gameConfiguration.slValue);
    
    if(setResMode(gameConfiguration.resMode))
    {
        fprintf( stderr , "[ERROR] Invalid resolution mode\n"
                          "        mode %u , %ux%u"
                          "[INFO] Using default resolution mode\n"
                          ,gameConfiguration.resMode
                          ,resModesTable[gameConfiguration.resMode][0]
                          ,resModesTable[gameConfiguration.resMode][1]
                );
        setResMode(DEFAULT_VIDEO_RES_MODE);
    }

#ifdef GCW0
	setResMode(GCW0_DEFAULT_RES_MODE);
#endif

    /*Config audio subsystem*/
    setSFX_Volume(gameConfiguration.SFX_Volume);
    setMusicVolume(gameConfiguration.MUS_Volume);

    return 0;
}

//-------------------------------------------

/********************************************
    saveGameCfg
    Save the game configuration on a file
*********************************************/

int saveGameCfg(void)
{
    //Retrieve current volume values
    gameConfiguration.SFX_Volume = getSFX_Volume();
    gameConfiguration.MUS_Volume = getMusicVolume();
    //Retrieve current video setings
    gameConfiguration.slValue = getVideoScanLines();
    

    unsigned slValue = getVideoScanLines();

    FILE *outFp;
    outFp = fopen(GAME_CFG_FILE_PATH,"wb");
    if(!outFp)
    {
        fprintf( stderr , "[ERROR] can not write to the game configuration file\n"
                          "        \"%s\""
                        , GAME_CFG_FILE_PATH);
        return -1;
    }

    //Header comment
    fputs("#Game general configuration file\n" , outFp );

    //Save sound options
    fputs( "\n"
           "#SOUND\n"
           "\n"
          ,outFp);

    fprintf( outFp
            ,"MUS_Volume:%3d #Music volume\n"
             "SFX_Volume:%3d #Sound effects volume\n"
            , gameConfiguration.MUS_Volume
            , gameConfiguration.SFX_Volume);

    //VIDEO OPTIONS
    fputs( "\n"
           "#VIDEO\n"
           "\n"
          ,outFp);

    fprintf( outFp
            ,"FULLSCREEN:%3d\n"
            , gameConfiguration.fullscreen);

    fprintf( outFp
            ,"RESOLUTION_MODE:%3d\n"
            , gameConfiguration.resMode);

    fprintf( outFp
            ,"SCANLINES:%3d\n"
            , gameConfiguration.scanlines);

    fprintf( outFp
            ,"SL_VALUE:%3d\n"
            , gameConfiguration.slValue);
    

    fclose( outFp);
    return 0;
}

//-------------------------------------------

/********************************************
    getFullscreenFlag
*********************************************/

int  getFullscreenFlag(void)
{
    return gameConfiguration.fullscreen;
}

//-------------------------------------------

/********************************************
    setFullscreenFlag
*********************************************/
void setFullscreenFlag(void)
{
    gameConfiguration.fullscreen = 1;
    return;
}

//-------------------------------------------

/********************************************
    resetFullscreenFlag
*********************************************/
void resetFullscreenFlag(void)
{
    gameConfiguration.fullscreen = 0;
    return;
}

//-------------------------------------------

/********************************************
    getScanlinesFlag
*********************************************/

int  getScanlinesFlag(void)
{
    return gameConfiguration.scanlines;
}

//-------------------------------------------

/********************************************
    setScanlinesFlag
*********************************************/
void setScanlinesFlag(void)
{
    gameConfiguration.scanlines = 1;
    return;
}

//-------------------------------------------

/********************************************
    resetScanlinesFlag
*********************************************/
void resetScanlinesFlag(void)
{
    gameConfiguration.scanlines = 0;
    return;
}

//-------------------------------------------
