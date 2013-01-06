/*
 * Sprite.c
 *
 *  Created on: 31.12.2012
 *      Author: nehmulos
 */

#include "Sprite.h"

Sprite* Sprite_create(SDL_Surface* image) {
	Sprite* this = malloc(sizeof(Sprite));
	this->image = image;
	this->bounds.x = 0;
	this->bounds.y = 0;
	this->frame.x = 0;
	this->frame.y = 0;
	this->scrollX = 1;
	this->scrollY = 1;
	if (image) {
		Sprite_setFrameSize(this, image->w, image->h);
	} else {
		Sprite_setFrameSize(this, 0,0);
	}
	return this;
}

void Sprite_destroy(Sprite* this) {
	free(this);
}

void Sprite_destroyWithImage(Sprite* this) {
	SDL_FreeSurface(this->image);
	Sprite_destroy(this);
}

void Sprite_setFrameSize(Sprite* this, int w, int h) {
	this->bounds.w = w;
	this->bounds.h = h;
	this->frame.w = w;
	this->frame.h = h;
}

void Sprite_draw(Sprite* this, SDL_Surface* surface) {
	SDL_BlitSurface(this->image, &this->frame, surface, &this->bounds);
}

void Sprite_drawRelative(Sprite* this, SDL_Surface* surface, SDL_Rect* container) {
	SDL_Rect translatedRect = {
		.x = this->bounds.x + container->x,
		.y = this->bounds.y + container->y,
		.w = this->bounds.w,
		.h = this->bounds.h
	};

	SDL_BlitSurface(this->image, &this->frame, surface, &translatedRect);
}

void Sprite_drawOnCamera(Sprite* this, SDL_Surface* surface, Camera* camera) {
	// TODO only draw the the subrect that is acutally on cam
	SDL_Rect translatedRect = {
		.x = (this->bounds.x - (camera->viewport.x * this->scrollX)),
		.y = (this->bounds.y - (camera->viewport.y * this->scrollY)),
		.w = this->bounds.w,
		.h = this->bounds.h
	};

	SDL_BlitSurface(this->image, &this->frame, surface, &translatedRect);
}
