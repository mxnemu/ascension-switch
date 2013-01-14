#pragma once
#include "Constants.h"
#include "GameModule.h"
#include "GameEngine.h"

#include "Scene.h"
#include "Entity.h"

#include "ui/UiNode.h"

CLASS(MainMenu) {

	UiNode* ui;
	Scene* scene;
	int moveMultiplier;

	GameModule* module;
	GameEngine* engine;
};

MainMenu* MainMenu_create(GameEngine* engine);
void MainMenu_init(void* context);
void MainMenu_destroy(void* context);
void MainMenu_update(void* context, RawTime dt);
void MainMenu_draw(void* context, SDL_Surface* surface);
void MainMenu_handleEvent(void* context, SDL_Event* event);

// UiCallbacks
void MainMenu_onStartButton(UiNode* button, void* callbackContext);
void MainMenu_onEditorButton(UiNode* button, void* callbackContext);
void MainMenu_onExitButton(UiNode* button, void* callbackContext);
