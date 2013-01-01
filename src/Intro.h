/*
 * Intro.h
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"
#include "GameModule.h"
#include "Sprite.h"
#include "GameEngine.h"

typedef struct Intro {
	Sprite* logo;
	Sprite* title;
	Sprite* slogan;
	SDL_Surface* blendSurface;
	Uint8 blendAlpha;
	TTF_Font* font;
	GameModule* module;
	RawTime timePassed;
	Mix_Music* music;
	GameEngine* engine;
} Intro;

Intro* Intro_create(GameEngine* engine);
void Intro_init(void* context);
void Intro_destroy(void* context);

void Intro_resize(void* context, SDL_Surface* surface);
void Intro_update(void* context, RawTime dt);
void Intro_draw(void* context, SDL_Surface* surface);
