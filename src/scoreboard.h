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
    scoreBoard.h
    Manage scoreBoards
*********************************************/

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

//Scoreboards numbers
#define SCOREBOARDS_NUMBER 2
//
#define SCOREBOARD_2MIN 0
#define SCOREBOARD_5MIN 1

//private:
#ifdef SCOREBOARD_C

int addScoreData( scoreboard *board , char *scoreName , int scoreValue);
void removeLastScore( scoreboard *board);
int addScoreRecord( scoreboard *board , char *scoreName , int scoreValue );

#endif

//public:

int whichScoreBoard(unsigned mode); //Return the scoreBoard number for the given game mode

int loadScoreboards(void);  //Load Score boards from file
int freeScoreboards(void);  //Unload Score boards/Free scoreBoards memory

int saveScoreboard( int boardNumber);  //Save score board to file

void printScoreboard( char *str , unsigned strSize , int boardNumber); //Fill the string with the board information

int rankScore( int scoreValue , int boardNumber); //Return the place on the ranking of a given score
int getBoardHighScore(int boardNumber);
int addScore( char *scoreName , int scoreValue , int boardNumber); //Add a new score record to the scoreBoard

#endif

