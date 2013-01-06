/*
 * GameEngine.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameEngine.h"
#include "Intro.h"
#include "LuaInit.h"

GameEngine* GameEngine_create(void) {

	GameEngine_initSubsystems();

	SDL_Surface* icon = SDL_LoadBMP("images/icon.bmp");
	SDL_WM_SetCaption("Year Of The Maya", NULL);
	SDL_WM_SetIcon(icon, NULL);

	GameEngine* this = malloc(sizeof(GameEngine));
	this->windowIsRunning = true;
	this->screen = SDL_SetVideoMode(800,480,DEFAULT_BPP,DEFAULT_WINDOW_FLAGS);
	this->l = luaL_newstate();
	luaL_openlibs(this->l);
	LuaInit_initCustomTypes(this->l);
	this->icon = icon;
	this->module = NULL;
	this->nextModule = NULL;
	GameEngine_setActiveModule(this, Intro_create(this)->module);
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

void GameEngine_setActiveModule(GameEngine* this, GameModule* module) {
	if (this->module) {
		GameModule_destroy(this->module);
	}
	module->init(module->context);
	module->resize(module->context, this->screen);
	this->module = module;
}

#define UPDATES_PER_SECOND 60
#define UPDATE_TIME 1000/UPDATES_PER_SECOND
#define MAX_UPDATE_TIME UPDATE_TIME*UPDATES_PER_SECOND*5
int GameEngine_run(GameEngine* this) {

	SDL_Rect screenRect = {.w=this->screen->w, .h=this->screen->h};
	SDL_Event event;
	RawTime lastUpdate = SDL_GetTicks();
	while(this->windowIsRunning) {

		if (this->nextModule) {
			GameEngine_setActiveModule(this, this->nextModule);
			this->nextModule = NULL;
		}

		while (SDL_PollEvent(&event)) {
        	if (event.type == SDL_QUIT) {
        		this->windowIsRunning = false;
        	} else if (event.type == SDL_VIDEORESIZE) {
        		SDL_FreeSurface(this->screen);
        		this->screen = SDL_SetVideoMode(event.resize.w,event.resize.h, DEFAULT_BPP, DEFAULT_WINDOW_FLAGS);
        		this->module->resize(this->module->context, this->screen);
        	} else {
        		this->module->handleEvent(this->module->context, &event);
        	}
        }


        RawTime now = SDL_GetTicks();
        while (now - lastUpdate > UPDATE_TIME) {
        	this->module->update(this->module->context, UPDATE_TIME);
        	lastUpdate += UPDATE_TIME;
        }

        SDL_FillRect(this->screen, &screenRect, 0x000000); // clear screen
        this->module->draw(this->module->context, this->screen);

		SDL_Flip(this->screen);
	}
	GameEngine_destroy(this); // destroy yourself
	puts("Closed fine without errors");
	return 0;
}

