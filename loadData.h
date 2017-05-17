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
    loadData.h
*********************************************/

#ifdef D_CHARSET_H
	int load_ChrSetArray( char *filePath , charSetArray *charSets);
	void free_ChrSetArray(charSetArray *charSets);
#endif

#ifdef D_BACKGROUND_H
	int loadBackground( char *filePath , background *bg);
#endif

#ifdef D_STAGE_H
	int loadStage( char *filePath , GMstage *stage);
	void freeStage( GMstage *stage);
#endif

#ifdef D_BM_FONT_H
	int loadBmFont(char *filePath , bmFont *font);
	void freeBmFont( bmFont *font);
#endif

#ifdef D_INPUT_H
	int loadInputConf( char *filePath , inputConf *controlsMap);
#endif

    long loadTime(FILE *inFp);
#ifdef D_GAME_CONFIG_H
    int loadGameCfgFile(char *filePath , gameCfgStruct *gameConfiguration);
#endif 
    
