#include "MainMenu.h"

MainMenu* MainMenu_create(GameEngine* engine) {
	MainMenu* this = malloc(sizeof(this));
	this->engine = engine;
	this->module = GameModule_create(this);
	this->module->destroy = MainMenu_destroy;
	this->module->init = MainMenu_init;
	return this;
}

void MainMenu_init(void* context) {
	MainMenu* this = context;

	this->ui = UiNode_create(NULL, NULL);
}

void MainMenu_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	MainMenu* this = context;
	free(this);
}
