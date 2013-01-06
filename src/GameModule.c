/*
 * GameModule.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameModule.h"
#include "Utils.h"

GameModule* GameModule_create(void* context) {
	GameModule* this = malloc(sizeof(GameModule));
	this->init = emptyInit;
	this->draw = emptyDraw;
	this->resize = emptyDraw;
	this->update = emptyUpdate;
	this->destroy = emptyDestroy;
	this->handleEvent = emptyHandleEvent;
	this->context = context;
	return this;
}

void GameModule_destroy(GameModule* this) {
	CANCEL_IF_FALSE(this);
	this->destroy(this->context);
	free(this);
}
