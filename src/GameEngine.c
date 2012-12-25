/*
 * GameEngine.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameEngine.h"
#include "Intro.h"

GameEngine* GameEngine_create(void) {

	GameEngine_initSubsystems();

	SDL_Surface* icon = SDL_LoadBMP("images/icon.bmp");
	SDL_WM_SetCaption("Year Of The Maya", NULL);
	SDL_WM_SetIcon(icon,NULL);

	GameEngine* this = malloc(sizeof(GameEngine));
	this->windowIsRunning = true;
	this->screen = SDL_SetVideoMode(800,480,24,SDL_SWSURFACE|SDL_DOUBLEBUF);
	this->l = luaL_newstate();
	this->icon = icon;
//	this->module = GameModule_create(NULL);
	this->module = Intro_create()->module;
	this->textureCache = TextureCache_create();
	return this;
}

void GameEngine_destroy(GameEngine* this) {
	CANCEL_IF_FALSE(this);
	lua_close(this->l);
	GameModule_destroy(this->module);
	TextureCache_destroy(this->textureCache);
	SDL_FreeSurface(this->icon);
	SDL_Quit();
	IMG_Quit();
	SDLNet_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	free(this);
}

void GameEngine_initSubsystems() {
	// TODO error handling
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	SDLNet_Init();
	TTF_Init();
	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
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

        this->module->update(this->module->context, 1); // TODO pass update time
        this->module->draw(this->module->context, this->screen);

		SDL_Flip(this->screen);
	}
	GameEngine_destroy(this); // destroy yourself
	puts("Closed fine without errors");
	return 0;
}

