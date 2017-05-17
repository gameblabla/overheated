#!/bin/bash
#Generate makefile

: ${TOP_COMMENT:="Auto generated makefile for UNIX-Like OSes"}
echo "#${TOP_COMMENT}"

: ${P:=main}
: ${CFLAGS:=-g -O3}
: ${OUTFILE:=-o overHeated.x86}
: ${LDLIBS:=-lSDL -lSDL_mixer -lm}
: ${DEFINES:=-D PC}
: ${CC:=gcc}

echo "P= ${P}" 
echo "CFLAGS= ${CFLAGS}" 
echo "OUTFILE= ${OUTFILE}" 
echo "LDLIBS= ${LDLIBS}" 
echo "DEFINES= ${DEFINES}" 
echo "CC= ${CC}" 

printf "OBJECTS= " 
for file in *.c
do
    printf "`basename $file .c`.o " 
done

echo -e "\n" 
echo -e "\$(P): \$(OBJECTS)" 
echo -e	"\t\$(CC) \$(CFLAGS) \$(OBJECTS) \$(DEFINES) \$(LDLIBS) \$(OUTFILE)\n\n" 

for file in *.c 
do
    gcc -MM $file 
    echo -e "\t\$(CC) $file -c \$(DEFINES) \$(CFLAGS)" 
    echo "" 
done

echo -e "clean:\n\trm *.o\n"
