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
    SDL_utils.c
*********************************************/

//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
//
#include "globalDefines.h"
#include "SDL_utils.h"

//#define CHECK_PRECONDITIONS

//------------------------------------------------

int SDL_setpixel( SDL_Surface *dest , unsigned x , unsigned y , Uint32 color)
{
#ifdef CHECK_PRECONDITIONS
	if( dest == NULL)
		return -1;

	if(dest->format->BytesPerPixel != 2)
		return -1;

	//Check if the pixel is inside the surface  boundaries
	if( x >= dest->w
	   || y >= dest->h)
		return -1;
#endif
	//Lock surface
	SDL_LockSurface(dest);
	//Set pixel value
	*((Uint16 *)dest->pixels + (y * dest->w) + x) = color;
	//Unlock surface
	SDL_UnlockSurface(dest);

	return 0;
}

//------------------------------------------------

Uint32 SDL_getPixel( SDL_Surface *source , unsigned x , unsigned y)
{
#ifdef CHECK_PRECONDITIONS
	if( source == NULL)
		return -1;

	if(source->format->BytesPerPixel != 2)
		return -1;

	//Check if the pixel is inside the surface  boundaries
	if( x >= source->w
	   || y >= source->h)
		return -1;
#endif
	Uint32 color;
	//Lock surface
	SDL_LockSurface(source);
	//Get pixel value
	color = *((Uint16 *)source->pixels + (y * source->w) + x);
	//Unlock surface
	SDL_UnlockSurface(source);

	return color;
}

//------------------------------------------------

int SDL_zoom( SDL_Surface *source , SDL_Surface *dest 
             , SDL_Rect *destRect ,unsigned zoom)
{
#ifdef CHECK_PRECONDITIONS
	if( source == NULL
	   || dest == NULL)
		return -1;

	if(dest->format->BytesPerPixel != 2)
		return -1;

	//Check if the dest surface has the
	//correct size
	if( source->w * zoom > dest->w
	   || source->h * zoom > dest->h)
		return -1;
#endif

    SDL_Rect defaultRect = { 0 , 0 , 0 , 0};
    if( NULL == destRect)
        destRect = &defaultRect;

	SDL_LockSurface(dest);
	SDL_LockSurface(source);

	Uint32 *srcPixelRow = (Uint32 *)source->pixels;
	Uint32 *destPixelRow = (Uint32 *)dest->pixels + destRect->x;
            destPixelRow += dest->w * destRect->y;
	Uint32 *destPixel = NULL;
	Uint32 *destPixelp = NULL; //Destination pixel pointer
	register y,x,a,b;
	for( y = 0 ;
		 y < source->h;
		 y++ , destPixelRow += zoom*dest->w , srcPixelRow += source->w
       )
	{
		for( x = 0 , destPixel = destPixelRow;
			 x < source->w;
			 x++ , destPixel += zoom)
		{
			for( a = 0 , destPixelp = destPixel;
				 a < zoom;
				 a++)
			{
                *(destPixelp+a) = *(srcPixelRow+x);
			}
		}
        /*Duplicate lines*/
        for( a = 1 ; a < zoom ; a++)
        {
            memcpy (  destPixelRow + a*dest->w  , destPixelRow 
                    , sizeof(Uint32)* (dest->w-destRect->x));
        }
	}
	SDL_UnlockSurface(dest);
	SDL_UnlockSurface(source);

	return 0;
}

//------------------------------------------------

SDL_Surface *SDL_optimizeSurface(SDL_Surface *surface)
{
	return SDL_DisplayFormatAlpha(surface);
}

//------------------------------------------------
/********************************************
loadIMG
*********************************************/

SDL_Surface *loadIMG( char *path , Uint16 flags)
{
    SDL_Surface *imgp = SDL_LoadBMP(path);
	if(!imgp)
	{
		fprintf(stderr,"[ERROR] Can't load %s\n" , path );
		exit(1);
	}

    if( flags & OPTIMIZE_IMG)
    {
        SDL_Surface *tmpSurface = imgp;
        imgp = SDL_optimizeSurface(tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }

    return imgp;
}

/********************************************
    fade Surface:
    Apply shadow mask to surface
*********************************************/

SDL_Surface *SDL_fadeSurface( SDL_Surface *mask , int value , SDL_Surface *dest)
{
    SDL_SetAlpha( mask , SDL_RLEACCEL|SDL_SRCALPHA , value);
    SDL_BlitSurface( mask , NULL , dest , NULL);
    return dest;
}

//------------------------------------------------

/********************************************
    Check_SDL_Version:
    Compare the runtime version of SDL with the SDL version
    that you compiled with.
    Issues a warning if they don't match

    Returns:
        0 if the versions match
        otherwise returns a value DIFFERENT from 0

*********************************************/

int check_SDL_Version(void)
{
    const SDL_version* runtimeVer = SDL_Linked_Version();

    SDL_version compiledVer;
    SDL_VERSION( &compiledVer );

    putchar('\n');

    printf("[INFO] Running with SDL v%u.%u.%u\n"
           , runtimeVer->major
           , runtimeVer->minor
           , runtimeVer->patch);

    printf("[INFO] Compiled with SDL v%u.%u.%u\n"
           , compiledVer.major
           , compiledVer.minor
           , compiledVer.patch);

    putchar('\n');

    if(     runtimeVer->major != compiledVer.major
        ||  runtimeVer->minor != compiledVer.minor
        ||  runtimeVer->patch != compiledVer.patch )
    {
        fprintf( stderr , "[WARNING]The SDL version that this program was compiled with"
                          "don't match with the runtime version\n\n");
        return 1;
    }

    return 0;    
}

/********************************************
createScanLineMask
SDL_Rect maskRect = Mask dimensions
int value = Intensity value
*********************************************/

SDL_Surface *createScanLineMask( SDL_Rect maskRect , int value)
{
    SDL_Surface *mask;
    mask = SDL_CreateRGBSurface( SDL_SWSURFACE|SDL_RLEACCEL|SDL_SRCALPHA, 
					                 maskRect.w, maskRect.h , SCREEN_BPP, 0, 0, 0, 0);
    if(!mask)
        return NULL;

    SDL_FillRect(mask, 0 , SDL_MapRGB(mask->format, 255, 0, 255));
    SDL_SetColorKey( mask, SDL_SRCCOLORKEY, SDL_MapRGB( mask->format, 255,0,255));
    /**/
    Uint32 lineColor = SDL_MapRGB(mask->format, 0, 0, 0);
    SDL_Rect linePos = { 0 , 0 , mask->w , 1};

    /*Draw Scanlines*/
    for( linePos.y = 0 ; linePos.y < mask->h ; linePos.y+=2)
    {
        SDL_FillRect(mask, &linePos, lineColor);
    }
    SDL_SetAlpha( mask , SDL_RLEACCEL|SDL_SRCALPHA , value);
    /**/
    return mask;
}
