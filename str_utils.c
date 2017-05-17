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
   str_utils.c
   Functions for strings manipulation
*********************************************/

//
#include <stdio.h>
#include <string.h>
//
#include "system_var_type.h"
//
#include "str_utils.h"

#define ERROR_CODE 1
#define MAX_STR_BUFFER_SIZE 100

//------------------------------------------------

#define RS_SET_STR_END\
	if(spaceStart) /*if there is a space after the string , ignore it*/\
		*(outStr + spaceStart) = '\0';\
	else\
		*(outStr + strIndex) = '\0';


/********************************************
    strTxtDimension:
        Calculate the X and Y dimensions
        of the text stored on the str
*********************************************/

void strTxtDimension( char *inStr , int *x , int *y)
{
    int maxChPerLine = 0;
    int chPerLine = 0;
    int lines = 0;

    int i;
    for( i = 0  ; i < strlen(inStr) ; i++ )
    {
        if( *(inStr + i ) == '\n')
        {
            if( chPerLine > maxChPerLine)
                maxChPerLine = chPerLine;

            lines++;
            chPerLine = 0;
        }
        else
        {
            chPerLine++;
        }
    }

    if( chPerLine > maxChPerLine)
       maxChPerLine = chPerLine;

    *x = maxChPerLine;
    *y = lines;

    return;   
}

/********************************************
    readString
********************************************/

int readString(FILE *inFp , char *outStr , unsigned outStrMaxSize , char *delimiters)
{
	char chBuffer;
	unsigned strIndex = 0;
	unsigned spaceStart = 0;
	unsigned delimiterNumber = strlen(delimiters);
	unsigned delimiterIndex;

	while(1)
	{

		chBuffer = fgetc(inFp);

		if(ferror(inFp) != 0) //Check I/O errors
		{

			RS_SET_STR_END
			return ERROR_CODE;

		}
		else if(feof(inFp) != 0) //Check EOF
		{
			RS_SET_STR_END
			return EOF;
		}

        if( chBuffer == '\r') //Ignore carriage return
            continue;

		if( chBuffer == '\n') //Check for new line
		{
			RS_SET_STR_END
			return 0;
		}

		//Check all the delimiters
		for( delimiterIndex = 0 ; delimiterIndex < delimiterNumber ; delimiterIndex++)
			if(chBuffer == *(delimiters + delimiterIndex))
			{
				RS_SET_STR_END
				return 0;
			}

		//Check spaces
		if( chBuffer == ' ')
        {
			if(strIndex)
            {
				if (spaceStart == 0) //if there are not spaces before the string
					spaceStart = strIndex;
				else; //VOID , TODO: Make this more elegant
            }
			else //if the space is placed before the string, ignore it
                continue; //while(1)
        }                      
		else
			spaceStart = 0;

		//add chBuffer to the output read string
		*(outStr + strIndex) = chBuffer;
		strIndex++;

		if(strIndex >= outStrMaxSize-1) 
		{
			RS_SET_STR_END
			return 0;	//Limit of the str reached
		}
	}
}

/********************************************
    readComment
*********************************************/

int readComment(FILE *inFp , char *delimiter)
{
	unsigned delimiterLength = 0;
	unsigned strIndex = 0;

	if(delimiter != NULL)
		delimiterLength = strlen(delimiter);

	char strBuffer[MAX_STR_BUFFER_SIZE];	//TODO: Use dynamic allocation , strbuffer = malloc(delimiterLength);

	while(1)
	{

		*(strBuffer+strIndex) = fgetc(inFp);

		if(ferror(inFp) != 0) //Check I/O errors
			return ERROR_CODE;

		else if(feof(inFp) != 0) //Check EOF
			return EOF;

		if(*(strBuffer+strIndex) == '\n') //Check for new line character
			return 0;

		//TODO_COMMENT: With dynamic allocation there is not need of checking
		//if the strbuffer maxsize has been reached
		//because strBuffer size == delimiter_size

		if(delimiterLength 
		   && *(strBuffer+strIndex) == *(delimiter+strIndex) ) //Check for the delimiter str
		{
			strIndex++;

			if(strIndex == delimiterLength)
				return 0;
		}
		else
			strIndex = 0;
	}

}

//Search one string in the file
//Return values:
//  0 : String found
//  EOF
//  Other value : Error

int searchString(FILE *inFp , char *target , char delimiter)
{
	if( target == NULL)
		return 0;

	unsigned targetLength = strlen(target);

	char chBuffer;
	unsigned int strIndex = 0;

	while(1)
	{
		chBuffer = fgetc(inFp);

		if(ferror(inFp) != 0) //Check I/O errors
			return ERROR_CODE;

		else if(feof(inFp) != 0) //Check EOF
			return EOF;

		if( chBuffer == '#') //Check for comments
		{
			readComment(inFp , NULL);
			strIndex = 0;
		}

		else if( chBuffer == *(target + strIndex) )
		{
			strIndex++;
			if(strIndex == targetLength)
			{
				//Check if there is more characters in the str
				//*spaces are ignored
				while(1)
				{
					chBuffer = fgetc(inFp);

					if(ferror(inFp) != 0) //Check I/O errors
						return ERROR_CODE;

					if(feof(inFp) != 0)  //EOF reached when
						return 0; //The string has been found

					if(chBuffer == delimiter
					    || chBuffer == '#'
					    || chBuffer == '\n')
						return 0;  //End of the string reached , string has been found

					if(chBuffer != ' ') //there are more characters in the string
					{
						strIndex = 0;
						break; //Continue the search
					}
				}
			}
		}
		else //Character read does not match with target
			strIndex = 0;
	}
}

int fetchNextString(FILE *inFp)
{
	char chBuffer;
	
while(1)
	{
		chBuffer = fgetc(inFp);
		//		printf("chBuffer: %c \n",chBuffer);
		if(ferror(inFp) != 0) //Check I/O errors
			return ERROR_CODE;

		else if(feof(inFp) != 0) //Check EOF
			return EOF;

		if( chBuffer == '#') //Check for comments
			while(1) //Skip comment
			{
				chBuffer = fgetc(inFp);
				//printf("chBuffer: %c ,\n",chBuffer);
				if(ferror(inFp) != 0) //Check I/O errors
					return ERROR_CODE;

				else if(feof(inFp) != 0) //Check EOF
					return EOF;

				if(chBuffer == '\n') //End of commented line reached
					break;
			}
		else if( chBuffer != ':'
			 && chBuffer != ' '
			 && chBuffer != '\n'
			 && chBuffer != '\t')
		{
			fseek(inFp,ftell(inFp)-1,SEEK_SET);
			return 0;
		}
	}
}


void replaceStrVARS(char *inStr , char *outStr , unsigned outStrMaxSize , systemVar_t* systemVars , unsigned vars_n)
{

	int i,j,a;
	unsigned cvar;
	char *tmpStr = outStr;
	outStrMaxSize--; // end char is reserved for '\0'

	systemVars--; // substract 1 because the index goes from vars_n initial value to 1
	              //Offset by one so the processor only has to check "if zero"

	i = 0;
	while(  *(inStr+i)
		&& outStrMaxSize)
	{
		if( *(inStr+i) == '$')
		{
			//Search var string
			cvar = vars_n;
			while(cvar)
			{
				j = i;
				for(j++ , a = 0 ; //init
				    *(inStr+j) == *((systemVars+cvar)->name+a) && *(inStr+j) ; //while
				    j++ , a++) //increment
				{
					;
				}
				 //if a variable is found , end the search
				if( a == strlen( (systemVars+cvar)->name) )
				break;

				cvar--;
			}
			if(cvar)
			{
				//copy var str value to output
				i = j-1;
				for( a = 0 ; //init
				    *((systemVars+cvar)->value+a) != '\0' && outStrMaxSize; //while
				    *(tmpStr++) = *((systemVars+cvar)->value+a), //increment
				    a++ , outStrMaxSize--);// increment
				
			}
			else
			{
				*(tmpStr++) = *(inStr+i);
				outStrMaxSize--;
			}
		}
		else
		{
			*(tmpStr++) = *(inStr+i);
			outStrMaxSize--;
		}

		i++;
	}
	*tmpStr = '\0';

}

//Returns the index of the VAR (on *systemVars) found in Str
//return > 0 if not VAR found
int searchStrVAR(char *inStr , systemVar_t* systemVars , unsigned vars_n)
{
	int i , j;
	systemVars--; // substract 1 because the index goes from vars_n initial value to 1
		      //Offset by one so the processor only has to check "if zero"

	while(vars_n)
	{
		i = 0;
		while( !*(inStr+i) )
		{
			if( *(inStr+i) == '$')
			{
				for(i++ , j = 0 ; //init
				    *(inStr+i) == *((systemVars+vars_n)->name+j) && *(inStr+j) ; //while
				    i++ , j++) //increment
				if( j == strlen( (systemVars+vars_n)->name) )
				return vars_n-1;
			}
			i++;
		}
		vars_n--;
	}
}
