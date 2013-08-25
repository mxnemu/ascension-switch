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
#include "Input.h"

typedef struct GameEngine {
	bool windowIsRunning;
	SDL_Window* screen;
	SDL_Renderer* renderer;
	Input* input[PLAYER_COUNT];
	lua_State* l;
	GameModule* module;
	GameModule* nextModule;
	TextureCache* textureCache;
	SDL_Surface* icon;
} GameEngine;

GameEngine* GameEngine_create(void);
void GameEngine_initSubsystems(void);
void GameEngine_setActiveModule(GameEngine* this, GameModule* module);
int GameEngine_run(GameEngine* this);
