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

	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(this->engine->textureCache, "images/person.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0,0, 64, 128, 1000));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 64, 128, 1000));
	List_pushBack(idleAnimation->frames, Frame_create(0,128, 64, 128, 1000));
	List_pushBack(idleAnimation->frames, Frame_create(64,128, 64, 128, 1000));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	memset(this->players, 0, sizeof(this->players));
	this->players[0] = Player_create(this->scene, sprite, this->engine->input);
	this->players[0]->entity->physics.bounds.y = 320 * PHYSICS_SCALE;


	Scene_addEntity(this->scene, this->players[0]->entity);
	this->scene->camera->trackedEntity = this->players[0]->entity;
}

void Game_onStartButton(UiNode* button) {
	printf("OH NO SOMEBODY REALLY WANT'S TO PLAY THIS SHIT");
}

void Game_update(void* context, RawTime dt) {
	Game* this = context;
	Scene_update(this->scene, dt);
}

void Game_draw(void* context, SDL_Renderer* renderer) {
	Game* this = context;
	Scene_draw(this->scene, renderer);
}
