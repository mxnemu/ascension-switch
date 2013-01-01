/*
 * Intro.c
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#include "Intro.h"
#include "MainMenu.h"

#define INTRO_TIME 1000

#define INTRO_FADEIN_START 0
#define INTRO_FADEIN_END (INTRO_TIME/3)
#define INTRO_FADEIN_TIME (INTRO_FADEIN_END - INTRO_FADEIN_START)

#define INTRO_REST_START INTRO_FADEIN_END
#define INTRO_REST_END (INTRO_TIME/3*2)
#define INTRO_REST_TIME (INTRO_REST_END - INTRO_REST_START)

#define INTRO_FADEOUT_START INTRO_REST_END
#define INTRO_FADEOUT_END INTRO_TIME
#define INTRO_FADEOUT_TIME (INTRO_FADEOUT_END - INTRO_FADEOUT_START)


Intro* Intro_create(GameEngine* engine) {
	Intro* this = malloc(sizeof(Intro));
	this->engine = engine;
	this->module = GameModule_create(this);
	this->module->init = Intro_init;
	this->module->update = Intro_update;
	this->module->draw = Intro_draw;
	this->module->destroy = Intro_destroy;
	this->module->resize = Intro_resize;
	return this;
}

void Intro_init(void *context) {
	Intro* this = context;

	SDL_Color fontColor =  {.r=255, .g=255, .b=255};
	SDL_Color fontShadeColor = {.r=0, .g=0, .b=0};

	SDL_Surface* vanillaLogo = IMG_Load("images/agdg-logo.png");
	SDL_Surface* transformedLogo = rotozoomSurface(vanillaLogo,0, 1, SMOOTHING_ON);
	SDL_FreeSurface(vanillaLogo);
	this->logo = Sprite_create(transformedLogo);

	this->font = TTF_OpenFont("fonts/Black-Chancery.ttf", 30);
	this->title = Sprite_create(TTF_RenderUTF8_Shaded(this->font, "Amateur Game Dev. Games", fontColor, fontShadeColor));
	this->slogan = Sprite_create(TTF_RenderUTF8_Shaded(this->font, " - just like make game -",  fontColor, fontShadeColor));
	this->music = Mix_LoadMUS("music/TheLoomingBattle.ogg");
	this->timePassed = 0;
	this->blendAlpha = 255;
	this->blendSurface = NULL;

	this->logo->rect.y = 20;
	this->title->rect.y = this->logo->rect.y + this->logo->rect.h + 15;
	this->slogan->rect.y = this->title->rect.y + this->title->rect.h + 15;

	Mix_PlayMusic(this->music, -1);
}

void Intro_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	Intro* this = context;
	TTF_CloseFont(this->font);
	Sprite_destroyWithImage(this->logo);
	Sprite_destroyWithImage(this->title);
	Sprite_destroyWithImage(this->slogan);
	SDL_FreeSurface(this->blendSurface);
	//Mix_FreeMusic(this->music);
	free(this);
}

void Intro_resize(void* context, SDL_Surface* surface) {
	Intro* this = context;
	this->logo->rect.x = (surface->w - this->logo->rect.w)/2;
	this->title->rect.x = (surface->w - this->title->rect.w)/2;
	this->slogan->rect.x = (surface->w - this->slogan->rect.w)/2;

	if (this->blendSurface) {
		SDL_FreeSurface(this->blendSurface);
	}
	SDL_Rect fillRect = {.x=0, .y=0, .w=surface->w, .h=surface->h};
	this->blendSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, DEFAULT_BPP, 0, 0, 0, 0);
	SDL_FillRect(this->blendSurface, &fillRect, 0x000000);
	SDL_SetAlpha(this->blendSurface, SDL_SRCALPHA, this->blendAlpha);
}

void Intro_update(void* context, RawTime dt) {
	Intro* this = context;
	this->timePassed += dt;

	if (inRangeOrEqual(this->timePassed, INTRO_FADEIN_START, INTRO_FADEIN_END)) {
		this->blendAlpha = 255-((float)(this->timePassed-INTRO_FADEIN_START)/(float)INTRO_FADEIN_TIME)*255;
	} else if (inRangeOrEqual(this->timePassed, INTRO_REST_START, INTRO_REST_END)) {
		this->blendAlpha = 0;
	} else if (inRangeOrEqual(this->timePassed, INTRO_FADEOUT_START, INTRO_FADEOUT_END)) {
		this->blendAlpha = ((float)(this->timePassed-INTRO_FADEOUT_START)/(float)INTRO_FADEOUT_TIME)*255;
	} else {
		this->engine->nextModule = MainMenu_create(this->engine)->module;
	}
	SDL_SetAlpha(this->blendSurface, SDL_SRCALPHA, this->blendAlpha);
}

void Intro_draw(void* context, SDL_Surface* surface) {
	Intro* this = context;

	Sprite_draw(this->logo, surface);
	Sprite_draw(this->title, surface);
	Sprite_draw(this->slogan, surface);
	SDL_BlitSurface(this->blendSurface, NULL, surface, NULL);
}
