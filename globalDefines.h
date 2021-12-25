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
   globalDefines.h
*********************************************/

#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#define PERRORS //Print errors

#define GM_SCREEN_W 320
#define GM_SCREEN_H 240
#define GM_SCREEN_X_CENTER (GM_SCREEN_W/2)
#define GM_SCREEN_Y_CENTER (GM_SCREEN_H/2)

#define DEFAULT_VIDEO_RES_MODE 1 //Default video resolution mode 640x480
#define GCW0_DEFAULT_RES_MODE 0  //320x240

#define GM_SCREEN_W_TILES 20
#define GM_SCREEN_H_TILES 15

#define GM_SCREEN_TILE_SIZE 16	//Normal background tile size

#define SCREEN_W 320
#define SCREEN_H 240

#ifdef FORCE_32BPP
#define SCREEN_BPP 32
#else
#define SCREEN_BPP 16
#endif

#define MAX_SC_VALUE SDL_ALPHA_OPAQUE //Max scanlines value
#define DEFAULT_ZOOM 1

#define FPS 60
#define MENU_FPS FPS
#define MSPF 16
#define GAME_FPS FPS

#define ENEMIES_OAM_SIZE 100
#define PL_BULLETS_OAM_SIZE 40
#define ENEMIES_BULLETS_OAM_SIZE 120
#define EFFECTS_OAM_SIZE 100
#define BONUS_OAM_SIZE 100
#define EVENTS_OAM_SIZE 20

#define MAX_STAGE_SPAWNS 2714

#define ENEMIES_16X16_CHARSET 0
#define PLAYER_CHARSET 1
#define PLAYER_BULLETS_CHARSET 2
#define EFFECTS_16X16_CHARSET 3
#define BULLETS_8X8_CHARSET 4
#define BONUS_16X16_CHARSET 5
#define ENEMIES_64X64_CHARSET 6
#define ENEMIES_32X32_CHARSET 7
#define ENEMIES_128X128_CHARSET 8
#define BULLETS_16X16_CHARSET 9
#define EFFECTS_32X32_CHARSET 10
#define EFFECTS_8X8_CHARSET 11
#define ENEMIES_80X80_CHARSET 12
#define EFFECTS_64X64_CHARSET 13
#define EMPTY_TILESET 14

#define ENEMIE_BULLET_LEFT 0

//
#define PL_MOVE_SPEED 5
#define PL_INIT_LIVES 3
//#define PL_SPAWN_MS 1000 //spawn milliseconds
#define PL_SPAWN_FRAMES 63
#define PL_SHIP_W  16    //Player ship width
#define PL_SHIP_H  16    //Player ship height


#define DEFAULT_ADV_FONT_W 8
#define DEFAULT_ADV_FONT_H 16

//GAME MODES/TYPES
#define TWO_MINUTES_MODE  1
#define FIVE_MINUTES_MODE 2
#define TWO_MINUTES_REPLAY 3
#define FIVE_MINUTES_REPLAY 4

//GAME RESOURCES

#define SMALL_FONT  "data/gfx/fonts/cell_phone.font"
#define ADVERT_FONT "data/gfx/fonts/announcements.font"

#endif
