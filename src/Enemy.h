#pragma once

#include "Constants.h"
#include "Entity.h"

CLASS(Enemy) {
	Entity* entity;
};

Enemy* Enemy_createWhiteKnight(struct Scene* scene, SDL_Point tilePos);
void Enemy_draw(void* context, SDL_Renderer* renderer, Camera* camera);
void Enemy_update(void* context, RawTime dt);
void Enemy_update(void* context, RawTime dt);
void Enemy_destroy(void* context);
