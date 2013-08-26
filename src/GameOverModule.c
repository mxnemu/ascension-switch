#include "GameOverModule.h"
#include "ui/UiMenuList.h"
#include "Game.h"

GameOverModule* GameOverModule_create(GameEngine* engine) {
	GameOverModule* this = malloc(sizeof(GameOverModule));
	this->engine = engine;
	this->module = GameModule_create(this);
	this->module->destroy = GameOverModule_destroy;
	this->module->init = GameOverModule_init;
	this->module->update = GameOverModule_update;
	this->module->draw = GameOverModule_draw;
	this->module->handleEvent = GameOverModule_handleEvent;
	this->moveMultiplier = -1;
	this->winnerName = "nobody";
	return this;
}

void GameOverModule_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	GameOverModule* this = context;
	UiNode_destroy(this->ui);
	free(this);
}

void GameOverModule_init(void* context) {
	GameOverModule* this = context;

	this->scene = Scene_create(this->engine, "scenes/mainMenu.lua", NULL);

	this->ui = UiNode_create(NULL, NULL);
	SDL_RenderGetLogicalSize(this->engine->renderer, &this->ui->bounds.w, &this->ui->bounds.h);
	UiMenuList* menu = UiMenuList_create(this->ui);

	SDL_Renderer* r = this->engine->renderer;
	char winnerString[128];
	sprintf(winnerString, "%s wins!", this->winnerName);
	UiMenuElement_create(menu, r, "Thanks for playing", NULL, NULL, this);
	UiMenuElement_create(menu, r, winnerString, NULL, NULL, this);
	UiMenuElement_create(menu, r, "Play", NULL, GameOverModule_onStartButton, this);
	//UiMenuElement_create(menu, r, "Options", NULL, NULL, this);
	//UiMenuElement_create(menu, r, "Credits", NULL, NULL, this);
	//UiMenuElement_create(menu, r, "Editor", NULL, GameOverModule_onEditorButton, this);
	UiMenuElement_create(menu, r, "Exit", NULL, GameOverModule_onExitButton, this);
	UiMenuElement_create(menu, r, "Created by Nehmulos", NULL, NULL, this);
	UiMenuElement_create(menu, r, "For LD27", NULL, NULL, this);

	UiNode_moveTo(menu->node,
				this->ui->bounds.w - menu->node->bounds.w - (this->ui->bounds.w/15),
				(this->ui->bounds.h - menu->node->bounds.h) / 2);

	//UiMenuList_layout(menu);

	// create stupid playersprite that
}

void GameOverModule_onStartButton(UiNode* button, void* callbackContext) {
	GameOverModule* this = callbackContext;
	this->engine->nextModule = Game_create(this->engine)->module;
}

void GameOverModule_onEditorButton(UiNode* button, void* callbackContext) {
//	GameOverModule* this = callbackContext;
//	this->engine->nextModule = EntityEditor_create(this->engine)->module;
}

void GameOverModule_onExitButton(UiNode* button, void* callbackContext) {
	GameOverModule* this = callbackContext;
	this->engine->windowIsRunning = false;
}

void GameOverModule_update(void* context, RawTime dt) {
	GameOverModule* this = context;
	Scene_update(this->scene, dt);

	if (!Camera_moveBy(this->scene->camera, 3*this->moveMultiplier, 0)) {
		this->moveMultiplier = -this->moveMultiplier;
	}
}

void GameOverModule_draw(void* context, SDL_Renderer* renderer) {
	GameOverModule* this = context;
	Scene_draw(this->scene, renderer);
	UiNode_draw(this->ui, renderer);
}

void GameOverModule_handleEvent(void* context, SDL_Event* event) {
	GameOverModule* this = context;
	UiNode_handleEvent(this->ui, event);
}
