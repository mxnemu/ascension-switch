/*
 * GameModule.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameModule.h"

GameModule* GameModule_create(void* context) {
	GameModule* this = malloc(sizeof(GameModule));
	this->init = emptyInit;
	this->draw = emptyDraw;
	this->resize = emptyDraw;
	this->update = emptyUpdate;
	this->destroy = emptyDestroy;
	this->context = context;
	return this;
}

void GameModule_destroy(GameModule* this) {
	CANCEL_IF_FALSE(this);
	this->destroy(this->context);
	free(this);
}
void emptyInit(void* context) {}
void emptyUpdate(void* context, RawTime dt) {}
void emptyDraw(void* context, SDL_Surface* surface) {}
void emptyDestroy(void* context) {}
