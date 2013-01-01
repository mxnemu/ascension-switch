#pragma once
#include "Constants.h"

typedef struct Sprite {
	SDL_Surface* image;
	SDL_Rect rect;
	SDL_Rect frame;
} Sprite;

Sprite* Sprite_create();
void Sprite_destroy(Sprite* this);
void Sprite_destroyWithImage(Sprite* this);
void Sprite_draw(Sprite* this, SDL_Surface* surface);

void Sprite_setFrameSize(Sprite* this, int w, int h);
