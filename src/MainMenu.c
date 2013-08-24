#include "MainMenu.h"
#include "ui/UiMenuList.h"
#include "Game.h"

MainMenu* MainMenu_create(GameEngine* engine) {
	MainMenu* this = malloc(sizeof(MainMenu));
	this->engine = engine;
	this->module = GameModule_create(this);
	this->module->destroy = MainMenu_destroy;
	this->module->init = MainMenu_init;
	this->module->update = MainMenu_update;
	this->module->draw = MainMenu_draw;
	this->module->handleEvent = MainMenu_handleEvent;
	this->moveMultiplier = -1;
	return this;
}

void MainMenu_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	MainMenu* this = context;
	UiNode_destroy(this->ui);
	free(this);
}

void MainMenu_init(void* context) {
	MainMenu* this = context;

	this->scene = Scene_create(this->engine, "scenes/mainMenu.lua", NULL);

	this->ui = UiNode_create(NULL, NULL);
	SDL_RenderGetLogicalSize(this->engine->renderer, &this->ui->bounds.w, &this->ui->bounds.h);
	UiMenuList* menu = UiMenuList_create(this->ui);

	SDL_Renderer* r = this->engine->renderer;
	UiMenuElement_create(menu, r, "Play", NULL, MainMenu_onStartButton, this);
	//UiMenuElement_create(menu, r, "Options", NULL, NULL, this);
	//UiMenuElement_create(menu, r, "Credits", NULL, NULL, this);
	//UiMenuElement_create(menu, r, "Editor", NULL, MainMenu_onEditorButton, this);
	UiMenuElement_create(menu, r, "Exit", NULL, MainMenu_onExitButton, this);

	UiNode_moveTo(menu->node,
				this->ui->bounds.w - menu->node->bounds.w - (this->ui->bounds.w/15),
				(this->ui->bounds.h - menu->node->bounds.h) / 2);

	//UiMenuList_layout(menu);

	// create stupid playersprite that
}

void MainMenu_onStartButton(UiNode* button, void* callbackContext) {
	MainMenu* this = callbackContext;
	this->engine->nextModule = Game_create(this->engine)->module;
}

void MainMenu_onEditorButton(UiNode* button, void* callbackContext) {
//	MainMenu* this = callbackContext;
//	this->engine->nextModule = EntityEditor_create(this->engine)->module;
}

void MainMenu_onExitButton(UiNode* button, void* callbackContext) {
	MainMenu* this = callbackContext;
	this->engine->windowIsRunning = false;
}

void MainMenu_update(void* context, RawTime dt) {
	MainMenu* this = context;
	Scene_update(this->scene, dt);

	if (!Camera_moveBy(this->scene->camera, 3*this->moveMultiplier, 0)) {
		this->moveMultiplier = -this->moveMultiplier;
	}
}

void MainMenu_draw(void* context, SDL_Renderer* renderer) {
	MainMenu* this = context;
	Scene_draw(this->scene, renderer);
	UiNode_draw(this->ui, renderer);
}

void MainMenu_handleEvent(void* context, SDL_Event* event) {
	MainMenu* this = context;
	UiNode_handleEvent(this->ui, event);
}
