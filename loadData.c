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
   loadData.c
*********************************************/

//
#include <stdio.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "d_input.h"
#include "system_var_type.h"
#include "str_utils.h"
#include "d_GameObject.h"
#include "d_Charset.h"
#include "charset.h"
#include "d_spawn.h"
#include "d_background.h"
#include "l_bgIns.h"
#include "d_stage.h"
#include "d_bmFont.h"
#include "globalVars.h"
#include "time.h"
#include "enemies.h"
#include "d_gameConfig.h"
#include "loadData.h"
#include "SDL_utils.h"
#include "l_objects.h"
//
#define TERMINAL_ERROR_MESSAGES
#define TERMINAL_STATUS_MESSAGES

#define SYSTEM_VARS_NUMBER 0 //See str_utils.h
#define STR_BUFFER_MAXSIZE 100 //See str_utils.h

#define ERROR_HANDLING exit(-1);

#define DEBUG
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
int load_ChrSetArray( char *filePath , charSetArray *charSets)
{
#ifdef DEBUG
	printf("[DEBUG] Loading \"%s\" charset file\n",filePath);
#endif
	systemVar_t *systemVars = NULL;

	//Open Charset file
	FILE *inFp;
	if( ( inFp = fopen( filePath , "rb")) == NULL)
	{
		fprintf( stderr , "\t[Error] while opening %s\n" , filePath);
		exit(-1);
	}

	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

	//Count the number of charsets
	charSets->size = 0;
	while( searchString(inFp,"charset",':') == 0)
		charSets->size++;

	fseek(inFp , 0 , SEEK_SET);

	//Allocate memory for the charset data
	charSets->array = (charSet *)malloc( sizeof(charSet) * charSets->size );

	int i;
	for(i= 0 ; i < charSets->size ; i++)
	{
		//Load charset gfx path
		if( searchString(inFp,"gfx",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf( stderr , "\t[Error] charset %d gfx missing\n"
				, i , filePath);
#endif
			ERROR_HANDLING

		}
		FETCH_NEXT_STRING
		READ_NEXT_STRING

		//Load the gfx bitmap file
		SDL_Surface *chrSetGfx  = SDL_LoadBMP(StrBuffer);
		if(!chrSetGfx)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf( stderr , "\t[Error] loading \"%s\"\n"
				, StrBuffer);
#endif
			ERROR_HANDLING
		}
		//Convert to the screen display format
		(charSets->array + i)->gfx = SDL_optimizeSurface(chrSetGfx);
		SDL_FreeSurface(chrSetGfx);
		//Set transparency color
		SDL_SetColorKey( (charSets->array + i)->gfx
				, SDL_SRCCOLORKEY
				, SDL_MapRGB( (charSets->array + i)->gfx ->format, 255,0,255));
        SDL_SetAlpha( (charSets->array + i)->gfx ,SDL_RLEACCEL|SDL_SRCALPHA,255);

		//Get charset tiles Width
		if( searchString(inFp,"tilesW",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf( stderr , "[Error] charset %d undefined in \"%s\"\n"
				, i , filePath);
#endif
			ERROR_HANDLING

		}

		FETCH_NEXT_STRING
		READ_NEXT_STRING

		(charSets->array + i)->tileW = atoi(StrBuffer);

		//Calculate charset tiles per line
		(charSets->array + i)->tilesPerLine =   (charSets->array + i)->gfx->w
							/(charSets->array + i)->tileW;

		//Get charset tiles Height
		if( searchString(inFp,"tilesH",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf( stderr , "[Error] charset %d undefined in \"%s\"\n"
				, i , filePath);
#endif
			ERROR_HANDLING

		}

		FETCH_NEXT_STRING
		READ_NEXT_STRING

		(charSets->array + i)->tileH = atoi(StrBuffer);
#ifdef DEBUG
		printf("\tCharset %03d , bpp: %02d tilesPerLine: %03d , W: %03d , H: %03d\n" ,
		 i ,(charSets->array + i)->gfx->format->BytesPerPixel
		,(charSets->array + i)->tilesPerLine , (charSets->array + i)->tileW , (charSets->array + i)->tileH);
#endif
	}

	fclose( inFp);
	return 0;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//

void free_ChrSetArray(charSetArray *charSets)
{
	int i = 0;
	for( i = charSets->size - 1 ; i >= 0 ; i--)
		SDL_FreeSurface( ( charSets->array + i)->gfx );

	free( charSets->array);
}

//---------------------------------------------------------------------//
//Load Bitmap font
//---------------------------------------------------------------------//
int loadBmFont(char *filePath , bmFont *font)
{
	FILE *inFp;
	if( (inFp = fopen( filePath , "rb")) == NULL)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] opening the file: \"%s\"\n",filePath);
#endif
		ERROR_HANDLING
	}

	systemVar_t *systemVars = NULL;
	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

	//Load gfx path 
	if( searchString(inFp,"gfx",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"gfx\" not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	SDL_Surface *fontGfx  = SDL_LoadBMP(StrBuffer);
	if(!fontGfx)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf( stderr , "[Error] loading \"%s\"\n"
			, StrBuffer);
#endif
		ERROR_HANDLING
	}

	font->gfx = SDL_optimizeSurface(fontGfx);
	SDL_FreeSurface(fontGfx);
	//Set transparency color
	SDL_SetColorKey( font->gfx
			, SDL_SRCCOLORKEY
			, SDL_MapRGB( font->gfx->format, 255,0,255));

	if( searchString(inFp,"charsPerLine",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"charsPerLine\" not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	font->chrPerLine = atoi( StrBuffer);

	if( searchString(inFp,"charW",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"charW(\"(char width) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	font->size.w = atoi( StrBuffer);

	if( searchString(inFp,"charH",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"charH(\"(char height) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	font->size.h = atoi( StrBuffer);

	if( searchString(inFp,"spaceW",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"spaceW(\"(space width) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	font->wSpacing = atoi( StrBuffer);

	if( searchString(inFp,"spaceH",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"spaceH(\"(space height) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	font->hSpacing = atoi( StrBuffer);

	fclose(inFp);
	return 0;
}

//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void freeBmFont( bmFont *font)
{
    if( font->gfx != NULL)
    {
	    SDL_FreeSurface( font->gfx);
        font->gfx = NULL;
    }
	return;
}

//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
Uint16 *loadTilemap( char *filePath , int tilemapW , int tilemapH)
{
	Uint16 *tilemap;

	//open tilemap file
	FILE *inFp;
	if( (inFp = fopen( filePath , "rb")) == NULL)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] opening the tilemap file: \"%s\"\n",filePath);
#endif
		ERROR_HANDLING
	}
	int tilemapSize = tilemapW * tilemapH;
	//Allocate memory for the tilemap
	tilemap = (Uint16 *)malloc( sizeof(Uint16) * tilemapSize);
	if(tilemap == NULL)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] allocating memory for the tilemap (\"%s\")\n",filePath);
#endif
		ERROR_HANDLING
	}

	//read tilemap from file
	fread( tilemap , sizeof(Uint16) , tilemapSize , inFp);
	if(ferror(inFp))
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] reading \"%s\"\n",filePath);
#endif
		fclose(inFp);
		free(tilemap);
		ERROR_HANDLING
	}

	fclose(inFp);

	return tilemap;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
int loadBackgroundScript(FILE *inFp , background *bg)
{

#ifdef DEBUG
	printf("[DEBUG] Loading background script:\n");
#endif
	systemVar_t *systemVars = NULL;
	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

	long fpMark;

	bg->scriptSize = 0;
	bg->script = NULL;
	//Look for the script
	if( searchString(inFp,"script",':') != 0)
		return 0;

	fpMark = ftell(inFp);

	//Calculate background script size
	while( searchString(inFp,"instructions",':') == 0)
		bg->scriptSize++;

#ifdef DEBUG
	printf("\tScript size: %d intructions\n",bg->scriptSize);
#endif
	if(bg->scriptSize == 0)
    {//The script is empty
        bg->script == NULL;	
        return 0;
    }

	//Return to the begining of the script
	fseek(inFp, fpMark ,SEEK_SET);

	//allocate space for the script
	bg->script = malloc( sizeof(bgScript) * bg->scriptSize);
	if(!bg->script)
	{
		ERROR_HANDLING
	}

	int insCode;
	int insOperandsNum;
	register i,j;
	for(i = 0 ; i < bg->scriptSize ; i++)
	{
		//Load instructions number
		if( searchString(inFp,"instructions",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[ERROR] \"instructions\" tag number %d missing!\n",i);
#endif
			ERROR_HANDLING

		}
		FETCH_NEXT_STRING
		READ_NEXT_STRING
		(bg->script+i)->insNum = atoi( StrBuffer);
		(bg->script+i)->ins = malloc( sizeof(bgInstruction)
					      * (bg->script+i)->insNum );

		if(!(bg->script+i)->ins)
		{
			ERROR_HANDLING
		}

		//Load instructions time
		if( searchString(inFp,"time",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[ERROR] \"time\" tag missing!\n");
#endif
			ERROR_HANDLING

		}
		(bg->script+i)->frame = time_mstf(loadTime(inFp));

		//load script instructions
		int a;
		j = (bg->script+i)->insNum - 1;
		for( ; j >= 0 ; j--)
		{
			FETCH_NEXT_STRING
			READ_NEXT_STRING
			insCode = getBgInsCode(StrBuffer);
			if( insCode < 0)
			{
#ifdef TERMINAL_ERROR_MESSAGES
				fprintf(stderr,"[ERROR]"
				" background instruction \"%s\" does not exist!\n"
				,StrBuffer);
#endif
				ERROR_HANDLING
			}
			((bg->script+i)->ins+j)->func = getBgInsFunc(insCode);
			insOperandsNum = getBgInsOperandsNum( insCode );
			//Allocade memory for the instruction operands
			((bg->script+i)->ins+j)->operands = malloc( sizeof(int) * insOperandsNum);


#ifdef DEBUG
			printf("\tframe: %5d, \"%s\" ",(bg->script+i)->frame , StrBuffer);
#endif

			//Read instructions operands
			for( a = 0 ; a < insOperandsNum ; a++)
			{
				FETCH_NEXT_STRING
				READ_NEXT_STRING
				*(((bg->script+i)->ins+j)->operands+a) = atoi(StrBuffer);
#ifdef DEBUG
				printf("%d ",*(((bg->script+i)->ins+j)->operands+a));
#endif
			}
#ifdef DEBUG
			putchar('\n');
#endif
		}
	}

	return 0;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void freeBackgroundScript(background *bg)
{

	register i,j;
	for(i = bg->scriptSize-1 ; i >= 0 ; i--)
	{
		for( j = (bg->script+i)->insNum-1 ; j >= 0 ; j--)
		{
			free( ((bg->script+i)->ins+j)->operands );
		}
		free( (bg->script+i)->ins );
	}
	free( bg->script);
	return;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
int loadBackground( char *filePath , background *bg)
{

#ifdef DEBUG
	printf("[DEBUG]---- Loading background \"%s\" ----\n",filePath);
#endif
	systemVar_t *systemVars = NULL;
	FILE *inFp;
	if( (inFp = fopen( filePath , "rb")) == NULL)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] opening \"%s\"\n",filePath);
#endif
		ERROR_HANDLING
	}

	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

	//Load stage tileset charset
	if( searchString(inFp,"charset",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , stage \"charset\" not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	charSetArray tmpChrSetArray;
	tmpChrSetArray.size = 1;
	load_ChrSetArray( StrBuffer , &tmpChrSetArray );
	bg->tileset = tmpChrSetArray.array;
	//chrSet_SetColorKey( bg->tileset , 0 , 0); //Disable transparency

    if( searchString(inFp,"interactive",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"interactive\" tag not found!\n");
#endif
		ERROR_HANDLING

	}

    FETCH_NEXT_STRING
	READ_NEXT_STRING

	bg->interactive = atoi(StrBuffer);


	if( searchString(inFp,"size",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , Size tag not found!\n");
#endif
		ERROR_HANDLING

	}
	if( searchString(inFp,"w",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , size \"w\"(width) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	bg->tilemap.w = atoi(StrBuffer);

	if( searchString(inFp,"h",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , size \"h\"(height) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	bg->tilemap.h = atoi(StrBuffer);

	//Load stage tilemap
	if( searchString(inFp,"tilemap",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , stage \"tilemap\" not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	bg->tilemap.data = loadTilemap( StrBuffer , bg->tilemap.w , bg->tilemap.h );
    bg->tilemap.tileObj = NULL;

	//Load scroll data
	//get stage scroll increment
	if( searchString(inFp,"increment",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , scroll \"inc\"(increment) not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	bg->scroll.inc = atoi(StrBuffer);

	//get stage scroll delay
	if( searchString(inFp,"delay",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , scroll \"delay\" not found!\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	bg->scroll.framesDelay = atoi(StrBuffer);

#ifdef DEBUG
	printf("\tWidth: %5d (tiles) , Height: %5d (tiles)\n"
		"\tScroll:\n\t\tincrement:%d , delay: %d\n"
		, bg->tilemap.w , bg->tilemap.h
		, bg->scroll.inc , bg->scroll.framesDelay);
#endif

	//Load background script from file
	loadBackgroundScript(inFp,bg);

	//Set initial values
	bg->pos.x = 0;
	//set initial Y position
	bg->pos.y = bg->tilemap.h * bg->tileset->tileH 
		        - (GM_SCREEN_H_TILES+1) * GM_SCREEN_TILE_SIZE;
	if(bg->pos.y < 0)
		bg->pos.y = 0;
	else
		bg->pos.y /= bg->tileset->tileH;

	bg->currentScript = 0;
	//Set loop to default values
	bg->scroll.loopStart = bg->tilemap.h-1;
	bg->scroll.loopEnd = 0;
	bg->scroll.loopRepeat = 1;

	bg->scroll.rowOffset= 0;
	bg->scroll.tileScroll.x = 0;
	bg->scroll.tileScroll.y = -bg->tileset->tileH+1;
	//bg->scroll.inc =  loaded from file
	//bg->scroll.framesDelay = loaded from file
	bg->scroll.framesCounter = 0;

    //Allocate and initialize the tilemap buffer
    
    bg->tilemapBuffer.w = GM_SCREEN_W / bg->tileset->tileW;
    bg->tilemapBuffer.h = GM_SCREEN_H / bg->tileset->tileH + 1;
    bg->tilemapBuffer.data = (Uint16 *)malloc( bg->tilemapBuffer.w * bg->tilemapBuffer.h * sizeof(Uint16));
    if(!bg->tilemapBuffer.data)
    {
        ERROR_HANDLING
    }

    //initialize the tilemap buffer
    fillTilemapBuffer( bg , bg->pos.y);

    //Allocate and initialize the tilemap object array
    bg->tilemapBuffer.tilesNumber =  bg->tilemapBuffer.w * bg->tilemapBuffer.h ;

    bg->tilemapBuffer.tileObj = (GameObject *)calloc( bg->tilemapBuffer.tilesNumber , sizeof(GameObject));

    //initialize the tilemap buffer objects
    setTilemapObjects( bg );
    
	fclose(inFp);

#ifdef DEBUG
	printf("       ---- Background loaded succesfully! ----\n\n");
#endif
	return 0;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void freeBackground(background *bg)
{
    //free tile objects data
    register i;
    for(i = 0 ; i < bg->tilemapBuffer.tilesNumber ; i++)
    {
        freeObject(bg->tilemapBuffer.tileObj+i);
    } 

    free(bg->tilemap.data);
    free(bg->tilemapBuffer.tileObj);

	free(bg->tilemapBuffer.data);
	freeBackgroundScript(bg);
	//Ugly:
	charSetArray tmpChrSetArray;
	tmpChrSetArray.size = 1;
	tmpChrSetArray.array = bg->tileset;
	free_ChrSetArray(&tmpChrSetArray);

}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//

long loadTime(FILE *inFp)
{
	long time = 0;

	systemVar_t *systemVars = NULL;
	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

	//Read minutes
	FETCH_NEXT_STRING
	READ_NEXT_STRING
	time = atoi(StrBuffer)*1000*60;

	//Read seconds
	FETCH_NEXT_STRING
	READ_NEXT_STRING
	time += atoi(StrBuffer)*1000;

	//Read milliseconds * 10
	FETCH_NEXT_STRING
	READ_NEXT_STRING
	time += atoi(StrBuffer)*10;

	return time;
}

//---------------------------------------------------------------------//
//---------------------------------------------------------------------//


#define ABSOLUTE_MODE 0 //The time in the delay tag is an absolute value
#define RELATIVE_MODE 1 //The time in the delay tag is relative to the last object spawn

int loadSpawnTable( FILE *inFp , spawnTable *spawnTable , long endPos , int relative)
{
	int i = 0;
	systemVar_t *systemVars = NULL;
	long fMark = 0;
	long entryEnd = 0;
	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer
	int spawnFrame = 0;

	//load spawn tables
    i = 0;
	while(i < MAX_STAGE_SPAWNS-1)
	{
		if( searchString(inFp,"delay",':') != 0)
			break;

		if( endPos != EOF && endPos <= ftell(inFp) )
		break;

		if(!relative)
			spawnFrame = 0;

		spawnFrame += time_mstf(loadTime(inFp));
		(spawnTable+i)->line = spawnFrame;

		if( searchString(inFp,"number",':') != 0)
		{
#ifdef  TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[Error] , spawn table %d has \"number\" missing\n",i);
#endif
			ERROR_HANDLING
		}

		FETCH_NEXT_STRING
		READ_NEXT_STRING

		(spawnTable+i)->num = atoi(StrBuffer);
#ifdef DEBUG
		//printf("\tstage spawn table %d , line %d , number of objects : %d\n"
		//	,i, (spawnTable+i)->line , (spawnTable+i)->num );
#endif
        //This spawn table is empty , reuse this
        //this array entry for the next spawn table
        if( (spawnTable+i)->num == 0)
             continue;

		//Allocate memory for the object spawns
		(spawnTable+i)->spawns = (objectSpawn *)malloc( sizeof(objectSpawn) * (spawnTable+i)->num );

		//Load the object spawns
		int j = (spawnTable+i)->num-1;
		for(j ; j >= 0 ; j--)
		{
			//Load X position
			if( searchString(inFp,"x",':') != 0)
			{
#ifdef  TERMINAL_ERROR_MESSAGES
				fprintf(stderr,"[Error] , spawn table %d : object spawn %d"
					"has \"x\" missing\n",i,j);
#endif
				ERROR_HANDLING
			}

			FETCH_NEXT_STRING
			READ_NEXT_STRING

			((spawnTable+i)->spawns+j)->pos.x = atoi(StrBuffer);

			//Load Y position
			if( searchString(inFp,"y",':') != 0)
			{
#ifdef  TERMINAL_ERROR_MESSAGES
				fprintf(stderr,"[Error] , spawn table %d : object spawn %d"
					"has \"y\" missing\n",i,j);
#endif
				ERROR_HANDLING
			}

			FETCH_NEXT_STRING
			READ_NEXT_STRING

			((spawnTable+i)->spawns+j)->pos.y = atoi(StrBuffer);

			//Load Object spawn function
			if( searchString(inFp,"type",':') != 0)
			{
#ifdef  TERMINAL_ERROR_MESSAGES
				fprintf(stderr,"[Error] , spawn table %d : object spawn %d"
					"has \"type\" missing\n",i,j);
#endif
				ERROR_HANDLING
			}

			FETCH_NEXT_STRING
			READ_NEXT_STRING

			((spawnTable+i)->spawns+j)->func = getObjectSpawnFunc( StrBuffer );
			if(((spawnTable+i)->spawns+j)->func == NULL)
				fprintf(stderr , "[ERROR]\t\"%s\" , the object does not exist or can't be spawned\n" , StrBuffer);

#ifdef DEBUG
			/*printf("\tObject spawn %d , x: %d , y: %d , type: %s\n"
				,j ,((spawnTable+i)->spawns+j)->pos.x
				,((spawnTable+i)->spawns+j)->pos.y , StrBuffer);*/
#endif

		}
        i++;
	}

	//Set the null spawn
	(spawnTable+i)->spawns = NULL;

	return 0;
}

//---------------------------------------------------------------------//
//---------------------------------------------------------------------//

int loadStage( char *filePath , GMstage *stage)
{
	int i = 0;
	long fpMark = 0;
	long endMark = 0;
	systemVar_t *systemVars = NULL;
	//Open stage file
	FILE *inFp;
	if( ( inFp = fopen(filePath,"rb") ) == NULL)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf( stderr , "[Error] while opening %s\n" , filePath);
#endif
		ERROR_HANDLING
	}

	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

    //Init stage frame counter
    stage->frame = 0;

	//Load sprites charsets
	if( searchString(inFp,"fmtVersion",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"fmtVersion\" not found!\n");
#endif
		ERROR_HANDLING
	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

#ifdef DEBUG
    printf("[DEBUG] Map format version: %02d\n" , atoi(StrBuffer));
#endif

	//Load sprites charsets
	if( searchString(inFp,"charsets",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , \"charsets\" not found!\n");
#endif
		ERROR_HANDLING
	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	if( load_ChrSetArray(StrBuffer , &sprites_charSets ) == -1)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] Loading \"%s\"\n",StrBuffer);
#endif
		ERROR_HANDLING
	}

	//Load Backgrounds

	fpMark = ftell(inFp);
	stage->backgrounds.number = 0;

	while( !searchString(inFp,"background",':'))
		stage->backgrounds.number++;

	//Allocate memory for the backgrounds
	if(stage->backgrounds.number)
	{
		stage->backgrounds.bg = malloc( sizeof(background)
						*stage->backgrounds.number);

		if(stage->backgrounds.bg == NULL)
		{
			ERROR_HANDLING
		}
	}

	fseek(inFp , fpMark , SEEK_SET);

	for(i = 0; i < stage->backgrounds.number ; i++)
	{
		if( searchString(inFp,"background",':') != 0)
		{
#ifdef TERMINAL_ERROR_MESSAGES
			fprintf(stderr,"[Error] , \"background\" not found!\n");
#endif
			ERROR_HANDLING
		}

		FETCH_NEXT_STRING
		READ_NEXT_STRING

		loadBackground( StrBuffer,
				stage->backgrounds.bg+i);
	}

	//Load spawns

	stage->spawnCounter = 0;
	stage->espSpawnCount = 0;

	if( searchString(inFp,"spawnTable",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[Error] , stage spawns not found!\n");
#endif
		ERROR_HANDLING
	}

	fpMark = ftell(inFp);
	endMark = EOF;

	if( searchString(inFp,"SpecialspawnTable",':') == 0)
		endMark = ftell(inFp);

	fseek(inFp , fpMark , SEEK_SET);

	//Allocate memory for all the spawns
	stage->spawnTable = (spawnTable *)malloc( sizeof(spawnTable) * MAX_STAGE_SPAWNS );
	//load normal spawns

#ifdef DEBUG
	printf("[DEBUG] Loading normal spawns table:\n");
#endif
	loadSpawnTable( inFp , stage->spawnTable , endMark, RELATIVE_MODE);


	fseek(inFp , fpMark , SEEK_SET);
	if( searchString(inFp,"SpecialspawnTable",':') != 0)
		return 0;
#ifdef DEBUG
	printf("[DEBUG] Loading Special spawns table:\n");
#endif
	//Allocate memory for all the spawns
	stage-> specialSpawns = (spawnTable *)malloc( sizeof(spawnTable) * MAX_STAGE_SPAWNS );
	//load special spawns
	loadSpawnTable( inFp , stage->specialSpawns , EOF, ABSOLUTE_MODE);

	fclose(inFp);
	return 0;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void freeSpawnTable(spawnTable* spawnTable)
{
	register i = 0;
	if(spawnTable != NULL)
	{
		while((spawnTable+i)->spawns != NULL )
		{
            free( (spawnTable+i)->spawns );
			i++;
		}
		free(spawnTable);
	}
    return;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void freeStage( GMstage *stage)
{
	register i = 0;
	//Free normal object spawn tables
    freeSpawnTable(stage->spawnTable);
	//Free special object spawn tables
    freeSpawnTable(stage->specialSpawns);
	//Free backgrounds
	for(i = 0; i < stage->backgrounds.number ; i++)
		freeBackground(stage->backgrounds.bg+i);
	free(stage->backgrounds.bg);

	//Free ChrSetArray
	free_ChrSetArray(&sprites_charSets);
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//


//keyboard keys
#define KEYS_NAME 8
char *keysName[KEYS_NAME] = 
	{
		 "SET_KEY:UP"
		,"SET_KEY:LEFT"
		,"SET_KEY:RIGHT"
		,"SET_KEY:DOWN"
		,"SET_KEY:START"
        ,"SET_KEY:SELECT"
		,"SET_KEY:A"
        ,"SET_KEY:B"
	};

//joystick buttons
#define JBUTTONS_NAMES 8
char *jButtonsNames[JBUTTONS_NAMES] =
	{
		 "SET_BUTTON:UP"
		,"SET_BUTTON:LEFT"
		,"SET_BUTTON:RIGHT"
		,"SET_BUTTON:DOWN"
		,"SET_BUTTON:START"
        ,"SET_BUTTON:SELECT"
		,"SET_BUTTON:A"
        ,"SET_BUTTON:B"
	};

//joypad axis
#define JAXIS_NAMES 3
char *jaxisNames[JAXIS_NAMES] = 
	{
		 "SET_AXIS:X"
		,"SET_AXIS:Y"
		,"AXIS_THRESHOLD"
	};

int loadKeyList( FILE *inFp , char **names , int num , Uint16 **keys)
{
		systemVar_t *systemVars = NULL;
		char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
		char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

		register int i = 0;
		long int fpMark = ftell(inFp);
		for(i = 0 ; i < num ; i++)
		{
			fseek(inFp,fpMark,SEEK_SET);
			if( searchString(inFp,*(names+i),':') != 0)
			{
				fprintf(stderr,"[ERROR] can't find \"%s\"\n",
				       *(names+i));
				ERROR_HANDLING
			}
			FETCH_NEXT_STRING
			READ_NEXT_STRING
			**(keys+i) = (Uint16)atoi(StrBuffer);
#ifdef DEBUG
			printf("\t%s\tVALUE: %d\n",*(names+i),**(keys+i));
#endif
		}
		fseek(inFp,fpMark,SEEK_SET);
		return;
}

int loadInputConf( char *filePath , inputConf *controlsMap)
{

	Uint16 *keys[]=
	{
		 &(controlsMap->key.UP)
		,&(controlsMap->key.LEFT)
		,&(controlsMap->key.RIGHT)
		,&(controlsMap->key.DOWN)
		,&(controlsMap->key.START)
		,&(controlsMap->key.SELECT)
		,&(controlsMap->key.A)
		,&(controlsMap->key.B)
	};
	Uint16 *jbuttons[]=
	{
		 &(controlsMap->jbutton.UP)
		,&(controlsMap->jbutton.LEFT)
		,&(controlsMap->jbutton.RIGHT)
		,&(controlsMap->jbutton.DOWN)
        ,&(controlsMap->jbutton.START)
        ,&(controlsMap->jbutton.SELECT)
		,&(controlsMap->jbutton.A)
		,&(controlsMap->jbutton.B)
	};
	Uint32 *jaxis[]=
	{
		 &(controlsMap->jaxisX)
		,&(controlsMap->jaxisY)
		,&(controlsMap->axisThreshold)
	};
#ifdef DEBUG
	printf("[DEBUG] Loading \"%s\" input configuration file:\n",filePath);
#endif
	systemVar_t *systemVars = NULL;

	FILE *inFp;
	if( ( inFp = fopen(filePath,"rb")) == NULL)
	{
		fprintf(stderr,"[ERROR] can't load \"%s\""
			       " input configuration file\n",filePath);
		ERROR_HANDLING
	}

	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

	//Look for the keyboard tag
	if( searchString(inFp,"keyboard",':') != 0)
		controlsMap->keyboard = 0;
	else
	{
		FETCH_NEXT_STRING
		READ_NEXT_STRING
		controlsMap->keyboard = ( atoi(StrBuffer) == 0 ? 0 : 1);
	}

#ifdef DEBUG
	printf("\tkeyboard: %d\n",controlsMap->keyboard);
#endif
	if(controlsMap->keyboard)
		//load configuration from file
		loadKeyList(inFp , keysName , KEYS_NAME , keys);

	//Look for joystick tag
	if( searchString(inFp,"joypad",':') != 0)
		controlsMap->joypad = 0;
	else
	{
		FETCH_NEXT_STRING
		READ_NEXT_STRING
		controlsMap->joypad = ( atoi(StrBuffer) == 0 ? 0 : 1);
	}

#ifdef DEBUG
	printf("\tjoypad: %d\n",controlsMap->joypad);
#endif
	if(controlsMap->joypad)
	{//load configuration from file
		loadKeyList(inFp , jButtonsNames , JBUTTONS_NAMES , jbuttons);
		register int i;
		long int fpMark = ftell(inFp);
		for(i = 0 ; i < JAXIS_NAMES ; i++)
		{
			fseek(inFp,fpMark,SEEK_SET);
			if( searchString(inFp,jaxisNames[i],':') != 0)
			{
				fprintf(stderr,"[ERROR] can't find \"%s\"\n",
				        jaxisNames[i]);
				ERROR_HANDLING
			}
			FETCH_NEXT_STRING
			READ_NEXT_STRING
			**(jaxis+i) = (Uint16)atoi(StrBuffer);
#ifdef DEBUG
			printf("\t%s\tVALUE: %d\n",jaxisNames[i] , **(jaxis+i));
#endif
		}
	}


	fclose(inFp);

	return 0;
}



/********************************************
    loadGameCfgFile
    Load Game configuration file
*********************************************/

int loadGameCfgFile(char *filePath , gameCfgStruct *gameConfiguration)
{
	FILE *inFp;
	if( ( inFp = fopen(filePath,"rb")) == NULL)
	{
		fprintf(stderr,"[ERROR] can't load \"%s\""
			       " game configuration file\n",filePath);
		ERROR_HANDLING
	}

    printf("[INFO] Loading game configuration file \"%s\"\n" , filePath);

	systemVar_t *systemVars = NULL;
	char finStrBuffer[STR_BUFFER_MAXSIZE]; //File string input buffer
	char StrBuffer[STR_BUFFER_MAXSIZE]; //String buffer

    //Load Musicvolume
    fseek( inFp , 0 , SEEK_SET);
	if( searchString(inFp,"MUS_Volume",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] \"MUS_Volume\" field not found\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	gameConfiguration->MUS_Volume = atoi(StrBuffer);

    //Load sound effect volume
    fseek( inFp , 0 , SEEK_SET);
	if( searchString(inFp,"SFX_Volume",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] \"SFX_Volume\" field not found\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

	gameConfiguration->SFX_Volume = atoi(StrBuffer);

    //Load fullscreen option
    fseek( inFp , 0 , SEEK_SET);
	if( searchString(inFp,"FULLSCREEN",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] \"FULLSCREEN\" field not found\n");
#endif
		ERROR_HANDLING

	}

	FETCH_NEXT_STRING
	READ_NEXT_STRING

    int fullscreenFlag = atoi(StrBuffer);
	gameConfiguration->fullscreen = fullscreenFlag ? 1 : 0;

    //Resolution mode
    fseek( inFp , 0 , SEEK_SET);
	if( searchString(inFp,"RESOLUTION_MODE",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] , \"RESOLUTION_MODE\" field not found\n"
                       "        Using default mode\n"
                );
#endif
        gameConfiguration->resMode = DEFAULT_VIDEO_RES_MODE;
	}
    else
    {
	    FETCH_NEXT_STRING
	    READ_NEXT_STRING

        gameConfiguration->resMode = atoi(StrBuffer);
    }

    //Scanlines

    fseek( inFp , 0 , SEEK_SET);
	if( searchString(inFp,"SCANLINES",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] \"SCANLINES\" field not found\n"
                       "            Using default value\n"
                );
#endif
		gameConfiguration->scanlines = 0;

	}
    else
    {
	    FETCH_NEXT_STRING
	    READ_NEXT_STRING

        int scanlinesFlag = atoi(StrBuffer);
	    gameConfiguration->scanlines = scanlinesFlag ? 1 : 0;
    }

    //Scanlines Intensity value

    fseek( inFp , 0 , SEEK_SET);
	if( searchString(inFp,"SL_VALUE",':') != 0)
	{
#ifdef TERMINAL_ERROR_MESSAGES
		fprintf(stderr,"[ERROR] , \"SL_VALUE\" field not found\n"
                       "             Using default value\n"
                );
#endif
        gameConfiguration->slValue = 0;
	}
    else
    {
	    FETCH_NEXT_STRING
	    READ_NEXT_STRING

        gameConfiguration->slValue = atoi(StrBuffer);
    }

    fclose(inFp);
    
    return 0;
}

//-------------------------------------------
