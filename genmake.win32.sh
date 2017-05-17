#!/bin/bash

TOP_COMMENT="makefile for mingw32, building for Windows 32 bits" \
P="OverHeated" \
CFLAGS="-g  -O3 -I./win/SDL/include" \
OUTFILE="-o overheated.exe" \
LDLIBS="-L./win/SDL/lib -L./win/SDL_mixer/lib/x86/ -lmingw32 -lSDLmain -lSDL -lSDL_mixer -lm" \
DEFINES="-D PC" \
CC="i686-w64-mingw32-gcc" \
./genmake.sh > make.win32
