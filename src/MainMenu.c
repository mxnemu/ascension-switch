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

	this->scene = Scene_create(this->engine, "scenes/mainMenu.lua");

	this->ui = UiNode_create(NULL, NULL);
	this->ui->bounds.w = this->engine->screen->w;
	this->ui->bounds.h = this->engine->screen->h;
	UiMenuList* menu = UiMenuList_create(this->ui);

	UiMenuElement_create(menu, "Play", NULL, MainMenu_onStartButton, this);
	UiMenuElement_create(menu, "Options", NULL, NULL, this);
	UiMenuElement_create(menu, "Credits", NULL, NULL, this);
	UiMenuElement_create(menu, "Exit", NULL, NULL, this);

	UiNode_moveTo(menu->node,
				this->ui->bounds.w - menu->node->bounds.w - (this->ui->bounds.w/15),
				(this->ui->bounds.h - menu->node->bounds.h) / 2);

	//UiMenuList_layout(menu);

	// create stupid playersprite that
}

void MainMenu_onStartButton(UiNode* button, void* callbackContext) {
	MainMenu* this = callbackContext;
	printf("OH NO SOMEBODY REALLY WANT'S TO PLAY THIS SHIT");
	this->engine->nextModule = Game_create(this->engine)->module;
}

void MainMenu_update(void* context, RawTime dt) {
	MainMenu* this = context;
	Scene_update(this->scene, dt);

	if (!Camera_moveBy(this->scene->camera, 3*this->moveMultiplier, 0)) {
		this->moveMultiplier = -this->moveMultiplier;
	}
}

void MainMenu_draw(void* context, SDL_Surface* surface) {
	MainMenu* this = context;
	Scene_draw(this->scene, surface);
	UiNode_draw(this->ui, surface);
}

void MainMenu_handleEvent(void* context, SDL_Event* event) {
	MainMenu* this = context;
	UiNode_handleEvent(this->ui, event);
}
