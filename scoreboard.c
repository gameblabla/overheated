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
 scoreBoard.c
 Manage scoreBoards
*********************************************/

#define SCOREBOARD_C

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include "system_var_type.h"
#include "d_scoreboard.h"
#include "d_gameConfig.h"
#include "str_utils.h"
#include "scoreboard.h"
//
#define TERMINAL_ERROR_MESSAGES
#define TERMINAL_STATUS_MESSAGES

#define SYSTEM_VARS_NUMBER 0 //See str_utils.h
#define STR_BUFFER_MAXSIZE 100 //See str_utils.h

#define ERROR_HANDLING exit(-1);

#define DEBUG

scoreboard gameScoreBoards[SCOREBOARDS_NUMBER];

char *scoreBoardsPath[SCOREBOARDS_NUMBER] =
{
     "data/score/twoMinMode.score",
     "data/score/fiveMinMode.score"
};

/********************************************
    whichScoreBoard:

    Return the scoreBoard number 
    that correspond to the given game mode
*********************************************/


#define WHICH_TABLE_SIZE 5
int whichBoardTable[] = 
{
    -1
    ,SCOREBOARD_2MIN  //TWO_MINUTES_MODE  
    ,SCOREBOARD_5MIN  //FIVE_MINUTES_MODE 2
    ,SCOREBOARD_2MIN  //TWO_MINUTES_REPLAY 3
    ,SCOREBOARD_5MIN  //FIVE_MINUTES_REPLAY 4
};

int whichScoreBoard(unsigned mode)
{
    if( mode >= WHICH_TABLE_SIZE)
        return -1;

    return whichBoardTable[mode];
}

/********************************************
    initScoreboard
    initialize scoreboards to default values
********************************************/
int initScoreboard(scoreboard *board)
{   
    int scoreEntry;
    scoreNode *newScore = NULL;
    void **lastScoreNext = NULL;

    lastScoreNext = &(board->first);

    for( scoreEntry = SCORE_BOARD_SIZE ; scoreEntry != 0 ; scoreEntry--)
    {
        //Add the score to the scoreboard
        newScore = malloc( sizeof( scoreNode));
        if(newScore == NULL)
            return -1;

        strncpy( newScore->name , "EMPT" , SCORE_NAME_SIZE);
        newScore->score = 000000;
        newScore->next = NULL;
        *lastScoreNext = newScore;
        lastScoreNext = &(newScore->next);            
    }
    return;
}

/********************************************
    loadScoreData
    Load scoreboard data from file
********************************************/
int loadScoreData(FILE *inFp , scoreboard *board)
{
	systemVar_t *systemVars = NULL;
	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer
    register scoreEntry;
    //
    char scoreName[SCORE_NAME_SIZE];
    int  scoreValue;
    scoreNode *newScore = NULL;
    void **lastScoreNext = NULL;

    //Load score data
    if( searchString(inFp,"scoreData",':') != 0)
    {
#ifdef DEBUG
        printf("\t[!] \"scoreData\" tag not found on file\n");
#endif
        return 0;
    }

    lastScoreNext = &(board->first);

    for( scoreEntry = SCORE_BOARD_SIZE ; scoreEntry != 0 && feof(inFp) == 0 ; scoreEntry--)
    {
        //Read score name
        FETCH_NEXT_STRING
        READ_NEXT_STRING

        strncpy( scoreName , StrBuffer , SCORE_NAME_SIZE);

        //Read score value
        FETCH_NEXT_STRING
        READ_NEXT_STRING

        scoreValue = atoi(StrBuffer);
        addScoreRecord( board , scoreName , scoreValue );
            
    }

    return 0;
}

/********************************************
    loadScoreboards
    Load scoreboards from file
********************************************/
int loadScoreboards(void)
{
    int boardNumber = 0;
    FILE *inFp = NULL;
    char filePath[128];

    for( boardNumber = 0 ; boardNumber < SCOREBOARDS_NUMBER ; boardNumber++)
    {
		snprintf(filePath, sizeof(filePath), "%s/%s", config_gamepath, scoreBoardsPath[boardNumber]);
        //filePath = *(scoreBoardsPath+boardNumber);
#ifdef DEBUG
        printf("[DEBUG]---- Loading scoreboard \"%s\" ----\n", filePath );
#endif
        //Create a default scoreboard
        initScoreboard( gameScoreBoards + boardNumber);

        //Open score file
        if( ( inFp = fopen( filePath , "rb")) == NULL)
        {
#ifdef DEBUG
            printf("\t[!] Can't open score file\n\tCreating a new scoreboard\n");
#endif
        }
        else
        {
            loadScoreData( inFp , gameScoreBoards + boardNumber);
            fclose(inFp);
        }
    }
    return 0;
}

//-------------------------------------------

/********************************************
    freeScoreData
     Unload scoreboard/Free scoreboards memory
********************************************/

int freeScoreData( scoreboard *board)
{
    register scoreNode *currentNode = NULL;
    register scoreNode *nextNode = board->first;
    while(nextNode != NULL)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;
        free(currentNode);
    }
    board->first = NULL;

    return 0;
}

/********************************************
    freeScoreboards
    Unload scoreboards/Free scoreboards memory
********************************************/

int freeScoreboards(void)
{
    register i;
    for( i = 0 ; i < SCOREBOARDS_NUMBER ; i++)
    {
        freeScoreData( gameScoreBoards + i);
    }
    return 0;
}

//-------------------------------------------

/********************************************
    saveScoreData
    save scoreboard data to file
********************************************/

int saveScoreData( FILE *inFp , scoreboard *board)
{
    scoreNode *cNode; //cNode
    fprintf( inFp , "scoreData:\n");

    for( cNode = board->first ; cNode != NULL ; cNode = cNode->next )
    {
        fprintf(inFp , "%0-4s:%06d\n", cNode->name , cNode->score);
    }

    return 0;
}

//-------------------------------------------

/********************************************
    saveScoreboard
    save scoreboard to file
********************************************/
int saveScoreboard( int boardNumber)
{
    scoreboard *board = gameScoreBoards + boardNumber;
    //char *filePath = scoreBoardsPath[boardNumber];
    char filePath[128]; 
    FILE *outFp;
  
    snprintf(filePath, sizeof(filePath), "%s/%s", config_gamepath, scoreBoardsPath[boardNumber]);

 //Open score file
    if( ( outFp = fopen( filePath , "w")) == NULL)
    {
        printf("Board number: %d\n" , boardNumber);
#ifdef DEBUG
        printf("\t[ERROR] Can't save score file %s\n",filePath);
#endif
            return -1;
    }

    saveScoreData( outFp , board);
    fclose(outFp);
    
    return 0;
}

/********************************************
    printScoreboard:
    Fill the string with the board information
*********************************************/

void printScoreboard( char *str , unsigned strSize , int boardNumber)
{
    *(str) = '\0';
    if( boardNumber >= SCOREBOARDS_NUMBER)
        return;

    register strIndex = 0;
#define SCORE_ENTRY_STR_SIZE 50
    char strBuffer[SCORE_ENTRY_STR_SIZE] = "\0";
    scoreNode *cNode = NULL;
    int rank = 1;

    for( cNode = gameScoreBoards[boardNumber].first , strIndex = 0 ;
         cNode != NULL && strIndex < strSize ; 
         cNode = cNode->next , strIndex += SCORE_ENTRY_STR_SIZE)
    {
        sprintf( strBuffer , "%2dth %0-4s-%06d\n" , rank , cNode->name , cNode->score);
        strcat( str , strBuffer);
        rank++;
    }
    return;
}

/*********************************************
    addScoreData:

    Add a new score record to the scoreBoard

    Returns:
    New score position on ranking
    (0 if is out of rank)
**********************************************/

int addScoreData( scoreboard *board , char *scoreName , int scoreValue)
{
    scoreNode *cNode; //Current node
    scoreNode *newNode;
    void **lastScoreNext = &(board->first);
    int i;
    for( cNode = board->first , i = 1 ;
         cNode != NULL ;
         cNode = cNode->next , i++)
    {
        if( cNode->score <= scoreValue)
        {
            //Make the new node and add it to the list
            newNode = malloc( sizeof(scoreNode));
            strncpy( newNode->name , scoreName , SCORE_NAME_SIZE-1);
            *(newNode->name + SCORE_NAME_SIZE-1) = '\0';
            newNode->score = scoreValue;

            newNode->next = cNode;
            *lastScoreNext = newNode;
            return i;
        }

        lastScoreNext = &(cNode->next);
        
    }
        
    return 0;
}

//-------------------------------------------

/*********************************************
    removeLastScore:

    Remove the last ( lowest ) score on the table
**********************************************/

void removeLastScore( scoreboard *board)
{
    register i;
    void **lastNext= &(board->first);
    scoreNode *cNode = board->first;
    while( cNode != NULL)
    {
        if( cNode->next == NULL)
        {/*remove score node*/
            free(cNode);
            cNode = NULL;
            break;
        }

        lastNext = &(cNode->next);
        cNode = cNode->next;
    }

    *lastNext = NULL;
    return;
}

//-------------------------------------------


/*********************************************
    addScoreRecord:

    Add a new score record to the scoreBoard
    Returns:
    -New score position on ranking
**********************************************/

int addScoreRecord( scoreboard *board , char *scoreName , int scoreValue )
{
    int i = addScoreData( board , scoreName , scoreValue);
    if(i)
        removeLastScore(board);
        
    return i;
}

//-------------------------------------------


/*********************************************
    addScore:
    Add a new score record to the scoreBoard
    Returns:
    -New score position on ranking
     (0 if is out of rank)
    -"-1" if the selected board does not exist
**********************************************/

int addScore( char *scoreName , int scoreValue , int boardNumber)
{
    if( boardNumber >= SCOREBOARDS_NUMBER)
        return -1;
    int i;
    i = addScoreData( gameScoreBoards + boardNumber , scoreName , scoreValue);
    if(i)
        removeLastScore(gameScoreBoards + boardNumber);

    return i;
}

/********************************************
    rankScore:
    Return the place on the ranking of a given score

    Returns:

    -Position on ranking
    (0 if is out of rank)
    -"-1" if the selected board does not exist
********************************************/

int rankScoreData( scoreboard *board , int scoreValue )
{
    scoreNode *cNode;
    int i = 1;

    for( cNode = board->first ;
         cNode != NULL ;
         cNode = cNode ->next)
    {
        if( cNode->score <= scoreValue)
            return i;
        i++;
    }
    return 0;
}


//-------------------------------------------

int rankScore( int scoreValue , int boardNumber)
{
    if( boardNumber >= SCOREBOARDS_NUMBER)
        return -1;

    return rankScoreData( gameScoreBoards + boardNumber , scoreValue);

}
//-------------------------------------------
/*******************************************
    getBoardHighScore:
    Return the board high score
    Returns -1 if the selected board does not exist
********************************************/
int getBoardHighScore(int boardNumber)
{
    if( boardNumber >= SCOREBOARDS_NUMBER)
        return -1;
    if( (gameScoreBoards+boardNumber)->first != NULL)
        return (*(scoreNode *)(gameScoreBoards+boardNumber)->first).score;

    return 0;
}

//-------------------------------------------
