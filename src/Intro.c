/*
 * Intro.c
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#include "Intro.h"

Intro* Intro_create() {

	SDL_Color fontColor =  {.r=255, .g=255, .b=255};
	SDL_Color fontShadeColor = {.r=0, .g=0, .b=0};

	Intro* this = malloc(sizeof(Intro));
	this->font = TTF_OpenFont("fonts/Black-Chancery.ttf", 30);
	this->logo = IMG_Load("images/agdg-logo.png");
	this->title = TTF_RenderUTF8_Shaded(this->font, "Amateur Game Dev. Games", fontColor, fontShadeColor);
	this->slogan = TTF_RenderUTF8_Shaded(this->font, " - just like make game -",  fontColor, fontShadeColor);
	this->timePassed = 0;

	this->module = GameModule_create(this);
	this->module->update = Intro_update;
	this->module->draw = Intro_draw;
	this->module->destroy = Intro_destroy;

	SDL_Rect r = {.x=0, .y=0, .w=0, .h=0};
	this->logoRect = r;
	this->titleRect = r;
	this->sloganRect = r;

	this->logoRect.y = 10;
	this->titleRect.y = this->logoRect.y + this->logo->h + 25;
	this->sloganRect.y = this->titleRect.y + this->title->h + 35;
	return this;
}

void Intro_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	Intro* this = context;
	TTF_CloseFont(this->font);
	SDL_FreeSurface(this->logo);
	SDL_FreeSurface(this->title);
	SDL_FreeSurface(this->slogan);
}

void Intro_update(void* context, RawTime dt) {
	Intro* this = context;
	this->timePassed += dt;
	if (this->timePassed >= INTRO_TIME) {

	}
}

void Intro_draw(void* context, SDL_Surface* surface) {
	Intro* this = context;

	this->logoRect.x = (surface->w - this->logo->w)/2;
	this->titleRect.x = (surface->w - this->title->w)/2;
	this->sloganRect.x = (surface->w - this->slogan->w)/2;

	SDL_BlitSurface(this->logo, NULL, surface, &this->logoRect);
	SDL_BlitSurface(this->title, NULL, surface, &this->titleRect);
	SDL_BlitSurface(this->slogan, NULL, surface, &this->sloganRect);
}
