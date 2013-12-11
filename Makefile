CC=gcc
LIBS=-llua5.2 -lSDL2 -lSDL2_mixer -lSDL2_net -lSDL2_ttf -lSDL2_image
LIB_DIRS:=
INCLUDE_DIRS:=
CFLAGS=-std=c99 -Wall -pedantic -g
C_HEADERS:=$(wildcard src/*.h) $(wildcard src/ui/*.h)
C_FILES:=$(wildcard src/*.c) $(wildcard src/ui/*.c)
C_ALL:=$(C_HEADERS) $(C_FILES)

MKDIR=mkdir -p

BIN_DIR=bin

all: main
main: prepareDir
	$(CC) $(CFLAGS) $(C_FILES) $(LIBS) $(LIB_DIRS) $(INCLUDE_DIRS) -o $(BIN_DIR)/$@

#test:
#	 $(CC) $(CFLAGS) List.c StringReplace.c tests/StringReplaceTest.c $(LIBS) -o $(BIN_DIR)/$@

prepareDir:
	$(MKDIR) $(BIN_DIR)
