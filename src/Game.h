#pragma once
#include "Constants.h"
#include "GameModule.h"
#include "GameEngine.h"

#include "Scene.h"
#include "Entity.h"

#include "ui/UiNode.h"

CLASS(Game) {
	Scene* scene;

	GameModule* module;
	GameEngine* engine;
};

Game* Game_create(GameEngine* engine);
void Game_init(void* context);
void Game_destroy(void* context);
void Game_update(void* context, RawTime dt);
void Game_draw(void* context, SDL_Surface* surface);

