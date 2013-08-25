#pragma once

#include "Constants.h"
#include "GameEngine.h"
#include "Sprite.h"

#define ROUND_TIME (10.f * 1000.f)
#define TRANSITION_TIME (0.4f * 1000.f)

CLASS(Hourglass) {
	int timeUntilNextRound;
	Sprite* sprite;
	float physicalAngle;
	int angleTarget;
	int angleDeltaStep;
	void* data;
	void (*onSpin)(void*);
};

Hourglass* Hourglass_create(GameEngine*);
void Hourglass_destroy(Hourglass* this);

void Hourglass_draw(Hourglass* this, SDL_Renderer* renderer);
void Hourglass_update(Hourglass* this, RawTime dt);
