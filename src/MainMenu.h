#pragma once
#include "Constants.h"
#include "GameModule.h"
#include "GameEngine.h"

#include "ui/UiNode.h"

typedef struct MainMenu {

	UiNode* ui;

	GameModule* module;
	GameEngine* engine;
} MainMenu;

MainMenu* MainMenu_create();
void MainMenu_init(void* context);
void MainMenu_destroy(void* context);
