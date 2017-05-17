#!/bin/bash

TOP_COMMENT="makefile gcc, building for UNIX-Like OSes running on PCs" \
P="Overheated" \
CFLAGS="-g -O3 \`pkg-config --cflags SDL_mixer sdl\`" \
OUTFILE="-o overheated.x86" \
LDLIBS="\`pkg-config --libs SDL_mixer sdl\` -lm" \
DEFINES="-D PC" \
CC="gcc" \
./genmake.sh > make.unixPC
