/*
 * GameEngine.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "GameEngine.h"
#include "Intro.h"
#include "MainMenu.h"
#include "Game.h"
#include "LuaInit.h"

GameEngine* GameEngine_create(void) {

	GameEngine_initSubsystems();

	GameEngine* this = malloc(sizeof(GameEngine));
	this->windowIsRunning = true;
	this->screen = SDL_CreateWindow("LD 27", 0,0, YOM_WINDOW_W, YOM_WINDOW_H, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	this->renderer = SDL_CreateRenderer(this->screen, 0, SDL_RENDERER_ACCELERATED);
	SDL_Surface* icon = SDL_LoadBMP("images/icon.bmp");
	SDL_SetWindowIcon(this->screen, icon);
	SDL_RenderSetLogicalSize(this->renderer, YOM_WINDOW_W, YOM_WINDOW_H);

	this->l = luaL_newstate();
	luaL_openlibs(this->l);
	LuaInit_initCustomTypes(this->l);
	for (int i=0; i < PLAYER_COUNT; ++i) {
		char path[128];
		sprintf(path, "hotkeysPlayer%d.lua", i+1);
		this->input[i] = Input_create();
		Input_loadHotkeys(this->input[i], this->l, path);
	}
	this->icon = icon;
	this->module = NULL;
	this->nextModule = NULL;
	this->textureCache = TextureCache_create(this->renderer);
	//GameEngine_setActiveModule(this, Intro_create(this)->module);
	GameEngine_setActiveModule(this, MainMenu_create(this)->module);
	//GameEngine_setActiveModule(this, Game_create(this)->module);
	return this;
}

void GameEngine_destroy(GameEngine* this) {
	CANCEL_IF_FALSE(this);
	GameModule_destroy(this->module);
	lua_close(this->l);
	for (int i=0; i < PLAYER_COUNT; ++i) {
		Input_destroy(this->input[i]);
	}
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
	SDL_Point size;
	SDL_RenderGetLogicalSize(this->renderer, &size.x, &size.y);
	module->resize(module->context, size);
	this->module = module;
}

#define UPDATES_PER_SECOND 60
#define UPDATE_TIME 1000/UPDATES_PER_SECOND
#define MAX_UPDATE_TIME UPDATE_TIME*UPDATES_PER_SECOND*5
int GameEngine_run(GameEngine* this) {

	SDL_Rect screenRect = {.x=0, .y=0};
	SDL_RenderGetLogicalSize(this->renderer, &screenRect.w, &screenRect.h);
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
        	} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        		//SDL_RenderSetLogicalSize(this->renderer, event.window.data1, event.window.data2);
        		// TODO
        		//this->module->resize(this->module->context, this->screen);
        	} else {
        		this->module->handleEvent(this->module->context, &event);
        	}
        }
		for (int i=0; i < PLAYER_COUNT; ++i) {
			Input_update(this->input[i]);
		}

        RawTime now = SDL_GetTicks();
        while (now - lastUpdate > UPDATE_TIME) {
        	this->module->update(this->module->context, UPDATE_TIME);
        	lastUpdate += UPDATE_TIME;
        }
        SDL_RenderClear(this->renderer);
        this->module->draw(this->module->context, this->renderer);

        SDL_RenderPresent(this->renderer);
	}
	GameEngine_destroy(this); // destroy yourself
	puts("Closed fine without errors");
	return 0;
}

