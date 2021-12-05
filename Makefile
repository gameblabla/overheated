PRGNAME     = game.elf
CC			= gcc
RESOURCE_FOLDER = data/

SRCDIR		= .
VPATH		= $(SRCDIR)
SRC_C		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJ_C		= $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJS		= $(OBJ_C)

CFLAGS		= -O3 -g3 -flto -I.

CXXFLAGS = $(CFLAGS)

LDFLAGS     = -flto -lc -lSDL -lm -lSDL_mixer


# Rules to make executable
$(PRGNAME): $(OBJS)  
	$(CC) $(CFLAGS) -o $(PRGNAME) $^ $(LDFLAGS)

$(OBJ_C) : %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(PRGNAME) *.o
