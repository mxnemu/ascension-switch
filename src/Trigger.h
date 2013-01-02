/*
 * Trigger.h
 *
 *  Created on: 02.01.2013
 *      Author: nehmulos
 */
#pragma once

#include "Constants.h"

CLASS(Trigger) {
	SDL_Rect bounds;
	lua_CFunction luaCallback;
};

