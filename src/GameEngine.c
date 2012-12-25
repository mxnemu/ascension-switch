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
	this->textureCache = TextureCache_create();
	return this;
}

void GameEngine_destroy(GameEngine* this) {
	CANCEL_IF_FALSE(this);
	lua_close(this->l);
	GameModule_destroy(this->module);
	TextureCache_destroy(this->textureCache);
	SDL_Quit();
	IMG_Quit();
	free(this);
}

void GameEngine_initSubsystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
}

int GameEngine_run(GameEngine* this) {

	SDL_Surface* person = TextureCache_get(this->textureCache, "images/person.png");
	SDL_Rect offset = {.x = 0, .y = 0, .w=0, .h=0};
	SDL_Rect rect = {.x = 0, .y = 0, .w=800, .h=480};

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

        SDL_FillRect(this->screen, &rect, 0xFF0000);
        SDL_BlitSurface(person, NULL, this->screen, &offset);

		SDL_Flip(this->screen);
	}
	GameEngine_destroy(this); // destroy yourself
	puts("Closed fine without errors");
	return 0;
}

