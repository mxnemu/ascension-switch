/*
 * GameModule.h
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"

typedef struct GameModule {
	void (*update)(void* context, RawTime dt);
	void (*draw)(void* context, SDL_Surface* surface);
	void (*destroy)(void* context);
	void* context;
} GameModule;

GameModule* GameModule_create(void* context);
void GameModule_destroy(GameModule* this);

void emptyUpdate(void* context, RawTime dt);
void emptyDraw(void* context, SDL_Surface* surface);
