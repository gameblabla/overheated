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
    score.c
    Manage the game Score
********************************************/


#ifndef SCORE_C
#define SCORE_C
#endif

//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//
#include "globalDefines.h"
#include "d_score.h"
//
#include "score.h"
#include "scoreboard.h"



static unsigned lock = 0; //Score update lock
//Current game score:
static scoreBoard gameScore;

/*******************************************
    Init Game Score
********************************************/

int initGameScore( int gameMode)
{
    //TO DO: load high score from score charts
    gameScore.gameType = gameMode;

    if(   gameMode == TWO_MINUTES_MODE 
       || gameMode == TWO_MINUTES_REPLAY)
    {
        gameScore.highScore = getBoardHighScore(SCOREBOARD_2MIN);
    }
    else if(   gameMode == FIVE_MINUTES_MODE 
            || gameMode == FIVE_MINUTES_REPLAY)
    {
        gameScore.highScore = getBoardHighScore(SCOREBOARD_5MIN);
    }

    gameScore.score = 0;
    lock = 0;

    return 0;
}

//-------------------------------------------
/*******************************************
    getScore
********************************************/

int getScore(void)
{
    return gameScore.score;
}

//--------------------------------------------
/*******************************************
    getHigh
********************************************/

int getHighScore(void)
{
    return gameScore.highScore;
}

//--------------------------------------------

/*******************************************
    lockScore
    Dissable score updates (incScore dissable)
*******************************************/
void lockScore(void)
{
    lock = 1;
}

//--------------------------------------------

/*******************************************
    unlockScore
    Re-enable score updates
*******************************************/

void unlockScore(void)
{
    lock = 0;
}

//--------------------------------------------

/*******************************************
    scoreLocked
    Get lock Value
    UnLocked = 0 , Locked = 1
*******************************************/

int scoreLocked(void)
{
    return lock;
}

/*******************************************
    incScore:

    Increment score
********************************************/

void incScore(unsigned value)
{
    if(lock)
        return;

    gameScore.score += value;
    if(gameScore.score > gameScore.highScore)
        gameScore.highScore = gameScore.score;

    return;
}

//--------------------------------------------

