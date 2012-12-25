/*
 * GameEngine.h
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"
#include "GameModule.h"
#include "TextureCache.h"

typedef struct GameEngine {
	bool windowIsRunning;
	SDL_Surface* screen;
	lua_State* l;
	GameModule* module;
	TextureCache* textureCache;
} GameEngine;

GameEngine* GameEngine_create(void);
void GameEngine_initSubsystems(void);
int GameEngine_run(GameEngine* this);
