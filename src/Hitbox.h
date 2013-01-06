#pragma once
#include "Constants.h"

typedef enum HitboxState {
	passive, aggressive, defensive
} HitboxState;

CLASS(Hitbox) {
	int xForce, yForce;
	SDL_Rect bounds;
	bool isCircular;
	HitboxState state;
};

Hitbox* Hitbox_create();
void Hitbox_destroy(Hitbox* this);

bool Hitbox_collides(Hitbox* this, Hitbox* other);
