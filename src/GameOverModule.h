#pragma once
#include "Constants.h"
#include "GameModule.h"
#include "GameEngine.h"

#include "Scene.h"
#include "Entity.h"

#include "ui/UiNode.h"

CLASS(GameOverModule) {

	UiNode* ui;
	Scene* scene;
	int moveMultiplier;
	const char* winnerName;

	GameModule* module;
	GameEngine* engine;
};

GameOverModule* GameOverModule_create(GameEngine* engine);
void GameOverModule_init(void* context);
void GameOverModule_destroy(void* context);
void GameOverModule_update(void* context, RawTime dt);
void GameOverModule_draw(void* context, SDL_Renderer* renderer);
void GameOverModule_handleEvent(void* context, SDL_Event* event);

// UiCallbacks
void GameOverModule_onStartButton(UiNode* button, void* callbackContext);
void GameOverModule_onEditorButton(UiNode* button, void* callbackContext);
void GameOverModule_onExitButton(UiNode* button, void* callbackContext);
