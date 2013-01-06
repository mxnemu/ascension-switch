/*
 * Game.c
 *
 *  Created on: 06.01.2013
 *      Author: nehmulos
 */

#include "Game.h"


Game* Game_create(GameEngine* engine) {
	Game* this = malloc(sizeof(Game));
	this->engine = engine;
	this->module = GameModule_create(this);
	this->module->destroy = Game_destroy;
	this->module->init = Game_init;
	this->module->update = Game_update;
	this->module->draw = Game_draw;
	return this;
}

void Game_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	Game* this = context;
	free(this);
}

void Game_init(void* context) {
	Game* this = context;

	this->scene = Scene_create(this->engine, "scenes/stage1.lua");
}

void Game_onStartButton(UiNode* button) {
	printf("OH NO SOMEBODY REALLY WANT'S TO PLAY THIS SHIT");
}

void Game_update(void* context, RawTime dt) {
	Game* this = context;
	Scene_update(this->scene, dt);
}

void Game_draw(void* context, SDL_Surface* surface) {
	Game* this = context;
	Scene_draw(this->scene, surface);
}
