#pragma once

#include "Constants.h"
#include "AnimatedSprite.h"
#include "TextureCache.h"

#define HEALTHBAR_FULL_TRANSITION 2000

CLASS(Healthbar) {
	AnimatedSprite* liquid;
	Sprite* background;
	int fullHeight;
	int transitionTarget;
	float transitionProgress;
	float transitionStep;
};

Healthbar* Healthbar_create(TextureCache* tc);
void Healthbar_destroy(Healthbar* this);
void Healthbar_draw(Healthbar* this, SDL_Renderer* renderer);

void Healthbar_set(Healthbar* this, const float health, const float maxHealth);
void Healthbar_update(Healthbar* this, RawTime dt);
