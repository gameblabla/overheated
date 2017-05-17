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
   video.c
   Functions for strings manipulation
*********************************************/


/*Functions for strings manipulation*/

#ifndef STRING_UTILS_HEADER
#define STRING_UTILS_HEADER

#define FETCH_NEXT_STRING\
	fseek(inFp,ftell(inFp)-1,SEEK_SET);\
	fetchNextString(inFp);

#define READ_NEXT_STRING\
	readString(inFp , finStrBuffer , STR_BUFFER_MAXSIZE , ":#");\
	replaceStrVARS( finStrBuffer , StrBuffer , STR_BUFFER_MAXSIZE , systemVars , SYSTEM_VARS_NUMBER);

void strTxtDimension( char *inStr , int *x , int *y);

int readString(FILE *inFp , char *outStr , unsigned outStrMaxSize , char *delimiters);
int readComment(FILE *inFp , char *delimiter);

//Search one string in the file
//Return values:
//  0 : String found
//  EOF
//  Other value : Error

int searchString(FILE *inFp , char *target , char delimiter);

int fetchNextString(FILE *inFp);

#ifdef SYSTEM_VAR_TYPE_H

void replaceStrVARS(char *inStr , char *outStr , unsigned outStrMaxSize , systemVar_t* systemVars , unsigned vars_n);
int searchStrVAR(char *inStr , systemVar_t* systemVars , unsigned vars_n);

#endif


#endif
