/*
 * Constants.h
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#pragma once

#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_joystick.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_mixer.h>

#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>   /* utils*/
#include <lua5.2/lualib.h>    /* libs */

typedef Uint32 RawTime;

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

