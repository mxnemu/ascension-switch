/*
 * Constants.h
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>

#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>   /* utils*/
#include <lua5.2/lualib.h>    /* libs */

typedef Uint32 RawTime; // in ms (1s == 1000)

#define PHYSICS_SCALE 6

#define PLAYER_COUNT 2

#define YOM_WINDOW_W 800
#define YOM_WINDOW_H 480

#define SCENE_WIDTH 384
#define SCENE_HEIGHT 480
#define SCENE_SPACER_WIDTH 32
#define SCENE_SPACER_HEIGHT 480

#define TILE_W 32
#define TILE_H 32
#define SCENE_TILES_X 12
#define SCENE_TILES_Y 15

#define DEFAULT_BPP 24
#define DEFAULT_WINDOW_FLAGS SDL_HWSURFACE|SDL_DOUBLEBUF


#define CANCEL_IF_FALSE(value) if (!value) { return; }
#define CLASS(name) typedef struct name name; struct name

// Thanks to Lindydancer for providing prepocess var to string converter code
// http://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Thanks to David Titarenco for providing this typesafe min/max
// http://stackoverflow.com/a/3437484
#ifndef MAX
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#ifndef MIN
#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif

#define inRange(a, min, max) (a > min && a < max)
#define inRangeOrEqual(a, min, max) (a >= min && a <= max)

