#pragma once
#include "../Constants.h"
#include "../GameEngine.h"
#include "../GameModule.h"
#include "../Sprite.h"
#include "../Hitbox.h"
#include "../ui/UiMenuList.h"
#include "../Utils.h"

CLASS(EntityEditor) {
	SDL_Surface* spritemap;

	GameModule* module;
	GameEngine* engine;
};

EntityEditor* EntityEditor_create(GameEngine* engine);
void EntityEditor_destroy(EntityEditor* this);

void EntityEditor_draw(void* context, SDL_Surface* surface);
