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
	Hourglass_destroy(this->hourglass);
	// lua propbaby kills everything
	/* wtf is wrong with this too much headache to fix
	for (int i=0; i < PLAYER_COUNT; ++i) {
		Player_destroy(this->players[i]);
	}
	free(this->players);
	*/
	free(this);
}

void Game_init(void* context) {
	Game* this = context;

	// fuck this lua setup it's just stupid
	// I screwed this up
	this->leftScene = Scene_create(this->engine, "scenes/initRed.lua", NULL);
	this->rightScene = Scene_create(this->engine, "scenes/initBlue.lua", NULL);
	this->leftScene = Scene_create(this->engine, "scenes/stage1.lua", this->leftScene);
	this->rightScene = Scene_create(this->engine, "scenes/stage1.lua", this->rightScene);

	memset(this->players, 0, PLAYER_COUNT * sizeof(Player*));
	Game_setupPlayer(this, 0, this->leftScene);
	Game_setupPlayer(this, 1, this->rightScene);

	this->hourglass = Hourglass_create(this->engine);
	this->hourglass->sprite->bounds.x = SCENE_WIDTH;
	this->hourglass->sprite->bounds.y = (SCENE_HEIGHT/2) - (this->hourglass->sprite->bounds.h/2);
}

void Game_setupPlayer(Game* this, int i, Scene* scene) {

	Player* player = this->players[i];

	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(this->engine->textureCache, "images/knightRed.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	player = Player_create(scene, sprite, this->engine->input);
	player->entity->physics.bounds.y = 320 * PHYSICS_SCALE;
	player->entity->physics.bounds.x = 20 * PHYSICS_SCALE;
	player->entity->physics.bounds.w = 15 * PHYSICS_SCALE; // Make sure Mr. Fatty get's though those slim trapdoors
	player->entity->offset.x = -10;
	player->entity->offset.y = 3;

	Scene_addEntity(scene, player->entity);
	scene->camera->trackedEntity = player->entity;
	Scene_setBounds(scene, 0, 0, SCENE_WIDTH, SCENE_HEIGHT);
	if (i == 0) {
		scene->colorPrefix = "red";
	} else if (i == 1) {
		scene->colorPrefix = "blue";
		scene->camera->translation.x = SCENE_WIDTH + SCENE_SPACER_WIDTH;
		scene->camera->translation.y = 0;
		scene->mirrorTiles = true;
	}
}

void Game_onStartButton(UiNode* button) {
	printf("OH NO SOMEBODY REALLY WANT'S TO PLAY THIS SHIT");
}

void Game_update(void* context, RawTime dt) {
	Game* this = context;
	Scene_update(this->leftScene, dt);
	Scene_update(this->rightScene, dt);
	Hourglass_update(this->hourglass, dt);
}

void Game_draw(void* context, SDL_Renderer* renderer) {
	Game* this = context;
	Scene_draw(this->leftScene, renderer);
	Scene_draw(this->rightScene, renderer);
	Hourglass_draw(this->hourglass, renderer);
}
