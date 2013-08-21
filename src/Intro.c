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

	SDL_Texture* vanillaLogo = IMG_LoadTexture(this->engine->renderer, "images/agdg-logo.png");
	this->logo = Sprite_create(vanillaLogo);

	this->font = TTF_OpenFont("fonts/Black-Chancery.ttf", 30);
//	this->title = Sprite_create(TTF_RenderUTF8_Blended(this->font, "Amateur Game Dev. Games", fontColor));
//	this->slogan = Sprite_create(TTF_RenderUTF8_Blended(this->font, " - just like make game -",  fontColor));
	this->title = Sprite_create(
		SDL_CreateTextureFromSurface(
			this->engine->renderer,
			TTF_RenderUTF8_Shaded(this->font, "Amateur Game Dev. Games", fontColor, fontShadeColor)
		)
	);
	this->slogan = Sprite_create(
		SDL_CreateTextureFromSurface(
			this->engine->renderer,
			TTF_RenderUTF8_Shaded(this->font, " - just like make game -",  fontColor, fontShadeColor)
		)
	);
	this->music = Mix_LoadMUS("music/TheLoomingBattle.ogg");
	this->timePassed = 0;
	this->blendAlpha = 255;
	this->blendSurface = NULL;

	this->logo->bounds.y = 20;
	this->title->bounds.y = this->logo->bounds.y + this->logo->bounds.h + 15;
	this->slogan->bounds.y = this->title->bounds.y + this->title->bounds.h + 15;

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

void Intro_resize(void* context, SDL_Point size) {
	Intro* this = context;
	this->logo->bounds.x = (size.x - this->logo->bounds.w)/2;
	this->title->bounds.x = (size.x - this->title->bounds.w)/2;
	this->slogan->bounds.x = (size.x - this->slogan->bounds.w)/2;

	if (this->blendSurface) {
		SDL_FreeSurface(this->blendSurface);
	}
	// TODO
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
}

void Intro_draw(void* context, SDL_Renderer* renderer) {
	Intro* this = context;

	SDL_Rect screenSize = {.x=0, .y=0};
	SDL_RenderGetLogicalSize(this->engine->renderer, &screenSize.w, &screenSize.h);

	Sprite_draw(this->logo, renderer);
	Sprite_draw(this->title, renderer);
	Sprite_draw(this->slogan, renderer);
	SDL_SetRenderDrawBlendMode(this->engine->renderer, SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawColor(this->engine->renderer, 0, 0, 0, this->blendAlpha);
	SDL_RenderDrawRect(this->engine->renderer, &screenSize); // TODO draw above
}
