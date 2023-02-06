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
    enemies.h
********************************************/

void spitze( GameObject *object);
void newSpitze( GameObject  *object , int type);
void spawnSpitze(SDL_Rect pos);
void spawnVtSpitzeR( SDL_Rect pos);
void spawnVtSpitzeL( SDL_Rect pos);

void archer( GameObject *object);
void newArcher( GameObject  *object , int type);
void spawnArcher(SDL_Rect pos);
void spawnR_archer( SDL_Rect pos);
void spawnL_archer( SDL_Rect pos);

void gaucer( GameObject *object);
void newGaucer( GameObject  *object);
void spawnGaucer(SDL_Rect pos);

void lovac( GameObject *object);
void newLovac( GameObject  *object);
void spawnLovac(SDL_Rect pos);

void zorgen( GameObject *object);
void newZorgen( GameObject  *object);
void spawnZorgen(SDL_Rect pos);

void litec( GameObject *object);
void newLitec( GameObject  *object);
void spawnLitec(SDL_Rect pos);

void farben( GameObject *object);
void newFarben( GameObject  *object , int type);
void spawnFarbenL(SDL_Rect pos);
void spawnFarbenR(SDL_Rect pos);

void petrus( GameObject *object);
void newPetrus( GameObject  *object);
void spawnPetrus(SDL_Rect pos);

void enemyMissile( GameObject *object);
void newEnemyMissile( GameObject  *object);
void spawnEnemyMissile(SDL_Rect pos);

void jumpa(GameObject *object);
void spawnJumpaLD( SDL_Rect pos);
void spawnJumpaRD( SDL_Rect pos);
void spawnJumpaLU( SDL_Rect pos);
void spawnJumpaRU( SDL_Rect pos);
//
void stom(GameObject *object);
void newStom( GameObject *object , int type);
void spawnStom(SDL_Rect pos , int type);

void spawnStomLD(SDL_Rect pos);
void spawnStomRD(SDL_Rect pos);
void spawnStomLU(SDL_Rect pos);
void spawnStomRU(SDL_Rect pos);
//

void laroc( GameObject *object);
void newLaroc( GameObject *object);
void spawnLaroc(SDL_Rect pos);
//
void spawnNacreeb(SDL_Rect pos);

//
void anancitesST( GameObject *object);
void newAnancitesST( GameObject *object);
void spawnAnancitesST(SDL_Rect pos);
//

void godrill(GameObject *object);
void newGodrill( GameObject *object);
void spawnGodrill(SDL_Rect pos);
//
void starView(GameObject *object);
void newStarView( GameObject *object);
void spawnStarView(SDL_Rect pos);

//
void laserRing( GameObject *object);
void newLaserRing( GameObject  *object);
void spawnLaserRing(SDL_Rect pos);
//
void spawnTirtasR( SDL_Rect pos);
void spawnTirtasL( SDL_Rect pos);
//
void enemyHMissile( GameObject *object);
void newEnemyHMissile( GameObject  *object , int pitch);
void spawnEnemyHMissile(SDL_Rect pos);
//
void spawnTekkerz(SDL_Rect pos);
//
void maraac( GameObject *object);
void newMaraac( GameObject *object);
void spawnMaraac(SDL_Rect pos);

//
void spawnMidBossEntrance(SDL_Rect pos);
//
void spawnNakser(SDL_Rect pos);
void destroyNakserTailPart(void *objData/*GameObject *nakserObj*/);
void destroyNakserTail(GameObject *nakserObj);
//
#ifdef D_PROGRAM_TYPES_H
LList_t makeNakserTail(SDL_Rect pos);
#endif
//
void bKnight( GameObject *object);
void newBKnight( GameObject  *object , int type);
void makeBKnight(SDL_Rect pos , int type);
void spawnBKnight_L(SDL_Rect pos);
void spawnBKnight_R(SDL_Rect pos);
//


