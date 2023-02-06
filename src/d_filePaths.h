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
   d_filePaths.h
   File paths literal strings
*********************************************/

#ifndef D_FILEPATH_H
#define D_FILEPATH_H

#include "globalDefines.h"

#define TWO_MINMODE_BGM_FILE DREAMCAST_CD_PATH"data/bgm/2min.ogg"
#define FIVE_MINMODE_BGM_FILE DREAMCAST_CD_PATH"data/bgm/5min.ogg"
/**/
#define SFX_FILE_PATH DREAMCAST_CD_PATH"data/sfx/audio.sfx"
/**/
#ifdef DREAMCAST
#define TWO_MINMODE_REPLAY_FILE "2.rep"
#define FIVE_MINMODE_REPLAY_FILE "5.rep"
#define TWO_MINMODE_REPLAY_FILE_READ_ONLY "data/replay/twoMinMode.replay"
#define FIVE_MINMODE_REPLAY_FILE_READ_ONLY "data/replay/fiveMinMode.replay"
#else
#define TWO_MINMODE_REPLAY_FILE "data/replay/twoMinMode.replay"
#define FIVE_MINMODE_REPLAY_FILE "data/replay/fiveMinMode.replay"
#endif
/**/
#define SCORE_FONT_FILE DREAMCAST_CD_PATH"data/gfx/fonts/cell_phone.font"
#define ADVERTS_FONT_FILE DREAMCAST_CD_PATH"data/gfx/fonts/announcements.font"

#endif
