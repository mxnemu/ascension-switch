/*
 * GameEngine.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameEngine.h"

GameEngine* GameEngine_create(void) {

	GameEngine_initSubsystems();

	GameEngine* this = malloc(sizeof(GameEngine));
	this->windowIsRunning = true;
	this->screen = SDL_SetVideoMode(800,480,24,SDL_SWSURFACE|SDL_DOUBLEBUF);
	this->l = luaL_newstate();
	this->module = GameModule_create();
	return this;
}

void GameEngine_destroy(GameEngine* this) {
	CANCEL_IF_FALSE(this);
	lua_close(this->l);
	GameModule_destroy(this->module);
	SDL_Quit();
	free(this);
}

void GameEngine_initSubsystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
}

int GameEngine_run(GameEngine* this) {

	SDL_Event event;
	while(this->windowIsRunning) {

        while (SDL_PollEvent(&event))
        {
        	if (event.type == SDL_QUIT) {
        		this->windowIsRunning = false;
        	}
        }

        this->module->update(1); // TODO pass update time
        this->module->draw(this->screen);

		SDL_Flip(this->screen);
	}
	GameEngine_destroy(this); // destroy yourself
	puts("Closed fine without errors");
	return 0;
}

