#pragma once

#include "Constants.h"
#include "Entity.h"

CLASS(PlayerCursor) {
	Entity* entity;
	Entity* entityUnderCursor;
};

PlayerCursor* PlayerCursor_create(Scene* scene, const char* colorPrefix);
void PlayerCursor_destroy(void* context);

void PlayerCursor_draw(void* context, SDL_Renderer* renderer, Camera* camera);
