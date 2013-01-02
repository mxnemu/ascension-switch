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
	void (*draw)(void* context, SDL_Surface* surface);
	void (*resize)(void* context, SDL_Surface* surface);
	void* context;
} GameModule;

GameModule* GameModule_create(void* context);
void GameModule_destroy(GameModule* this);

void emptyInit(void* context);
void emptyUpdate(void* context, RawTime dt);
void emptyDraw(void* context, SDL_Surface* surface);
void emptyDestroy(void* context);
