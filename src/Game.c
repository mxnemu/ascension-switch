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
	Player_destroy(this->leftPlayer);
	Player_destroy(this->rightPlayer);
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

	Game_setupPlayer(this, 0, this->leftScene);
	Game_setupPlayer(this, 1, this->rightScene);

	this->leftPlayer->opponent = this->rightPlayer;
	this->rightPlayer->opponent = this->leftPlayer;

	Player_switchMode(this->leftPlayer);

	this->hourglass = Hourglass_create(this->engine);
	this->hourglass->sprite->bounds.x = SCENE_WIDTH;
	this->hourglass->sprite->bounds.y = (SCENE_HEIGHT/2) - (this->hourglass->sprite->bounds.h/2);
	this->hourglass->data = this;
	this->hourglass->onSpin = Game_onHourglassSpin;
}

void Game_setupPlayer(Game* this, int i, Scene* scene) {

	if (i == 0) {
		scene->colorPrefix = "red";
		this->leftPlayer = Player_create(scene, this->engine->input);

	} else if (i == 1) {
		scene->colorPrefix = "blue";
		scene->camera->translation.x = SCENE_WIDTH + SCENE_SPACER_WIDTH;
		scene->camera->translation.y = 0;
		scene->mirrorTiles = true;

		this->rightPlayer = Player_create(scene, this->engine->input);
	}
	Scene_setBounds(scene, 0, 0, SCENE_WIDTH, SCENE_HEIGHT);
}

void Game_onStartButton(UiNode* button) {
	printf("OH NO SOMEBODY REALLY WANT'S TO PLAY THIS SHIT");
}

void Game_update(void* context, RawTime dt) {
	Game* this = context;
	Scene_update(this->leftScene, dt);
	Scene_update(this->rightScene, dt);
	Player_update(this->leftPlayer, dt);
	Player_update(this->rightPlayer, dt);
	Hourglass_update(this->hourglass, dt);
}

void Game_draw(void* context, SDL_Renderer* renderer) {
	Game* this = context;
	Scene_draw(this->leftScene, renderer);
	Scene_draw(this->rightScene, renderer);
	Player_draw(this->leftPlayer, renderer);
	Player_draw(this->rightPlayer, renderer);
	Hourglass_draw(this->hourglass, renderer);
}

void Game_onHourglassSpin(void* context) {
	Game* this = context;
	Player_switchMode(this->leftPlayer);
	Player_switchMode(this->rightPlayer);
}
