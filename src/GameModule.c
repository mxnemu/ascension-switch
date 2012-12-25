/*
 * GameModule.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameModule.h"

GameModule* GameModule_create(void* context) {
	GameModule* this = malloc(sizeof(GameModule));
	this->draw = emptyDraw;
	this->update = emptyUpdate;
	this->destroy = NULL;
	this->context = context;
	return this;
}

void GameModule_destroy(GameModule* this) {
	CANCEL_IF_FALSE(this);
	if (this->destroy) {
		this->destroy(this->context);
	}
	free(this);
}

void emptyUpdate(void* context, RawTime dt) {}
void emptyDraw(void* context, SDL_Surface* surface) {}
