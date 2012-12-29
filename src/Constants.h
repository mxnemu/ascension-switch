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

typedef float RawTime;

#define CANCEL_IF_FALSE(value) if (!value) { return; }
