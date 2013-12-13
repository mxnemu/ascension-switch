CC=gcc
LIBS=-llua5.2 -lSDL2 -lSDL2_mixer -lSDL2_net -lSDL2_ttf -lSDL2_image
LIB_DIRS:=
INCLUDE_DIRS:=
CFLAGS=-std=c99 -Wall -pedantic -g
C_HEADERS:=$(wildcard src/*.h) $(wildcard src/ui/*.h)
C_FILES:=$(wildcard src/*.c) $(wildcard src/ui/*.c)
C_ALL:=$(C_HEADERS) $(C_FILES)

BIN_DIR=bin/

OBJECTS:=$(addsuffix .o, $(basename $(C_FILES)))

MKDIR=mkdir -p
RM=rm -r


all: main
main: prepareDir $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) $(LIB_DIRS) $(INCLUDE_DIRS) -o $(BIN_DIR)$@

$(OBJECTS): %.o: %.c

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#test:
#	 $(CC) $(CFLAGS) List.c StringReplace.c tests/StringReplaceTest.c $(LIBS) -o $(BIN_DIR)/$@

prepareDir:
	$(MKDIR) $(BIN_DIR)

clean:
	$(RM) $(OBJECTS) $(BIN_DIR)
