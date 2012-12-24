/*
 * GameModule.h
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"

typedef struct GameModule {
	void (*update)(RawTime dt);
	void (*draw)(SDL_Surface* surface);
} GameModule;

GameModule* GameModule_create(void);
void GameModule_destroy(GameModule* this);

void emptyUpdate(RawTime dt);
void emptyDraw(SDL_Surface* surface);
