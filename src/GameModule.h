/*
 * GameModule.h
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"

typedef struct GameModule {
	void (*init)(void*context);
	void (*destroy)(void* context);
	void (*update)(void* context, RawTime dt);
	void (*draw)(void* context, SDL_Renderer* renderer);
	void (*resize)(void* context, SDL_Point size);
	void (*handleEvent)(void* context, SDL_Event* event);
	void* context;
} GameModule;

GameModule* GameModule_create(void* context);
void GameModule_destroy(GameModule* this);

