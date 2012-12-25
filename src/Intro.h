/*
 * Intro.h
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"
#include "GameModule.h"

#define INTRO_TIME 3000

typedef struct Intro {
	SDL_Surface* logo;
	TTF_Font* font;
	SDL_Surface* title;
	SDL_Surface* slogan;
	GameModule* module;
	RawTime timePassed;

	// TODO create a Sprite class for this bullshit
	SDL_Rect logoRect;
	SDL_Rect titleRect;
	SDL_Rect sloganRect;
} Intro;

Intro* Intro_create();
void Intro_destroy(void* context);

void Intro_update(void* context, RawTime dt);
void Intro_draw(void* context, SDL_Surface* surface);
