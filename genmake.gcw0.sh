#!/bin/bash

TOP_COMMENT="makefile for mipsel-gcc, building for GCW-ZERO" \
P="Overheated" \
CFLAGS="-O3 -I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include/SDL" \
OUTFILE="-o overheated_gcw0" \
LDLIBS="-L/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib/ -lSDL -lSDL_mixer -lm" \
DEFINES=" " \
CC="/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc" \
./genmake.sh > make.gcw0
