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
	this->rect.x = 0;
	this->rect.y = 0;
	this->frame.x = 0;
	this->frame.y = 0;
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
	this->rect.w = w;
	this->rect.h = h;
	this->frame.w = w;
	this->frame.h = h;
}

void Sprite_draw(Sprite* this, SDL_Surface* surface) {
	SDL_BlitSurface(this->image, &this->frame, surface, &this->rect);
}
