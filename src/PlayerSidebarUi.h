#pragma once

#include "Constants.h"
#include "Sprite.h"
#include "Healthbar.h"

CLASS(PlayerSidebarUi) {
	void* player;
	Sprite* playerIcon;
	Sprite* money;
	Sprite* coin;
	TTF_Font* font;
	SDL_Color color;
	SDL_Point position;
	Healthbar* healthbar;
};

PlayerSidebarUi* PlayerSidebarUi_create(void* player, SDL_Point pos);
void PlayerSidebarUi_destroy(PlayerSidebarUi* this);

void PlayerSidebarUi_draw(PlayerSidebarUi* this, SDL_Renderer* renderer);
void PlayerSidebarUi_update(PlayerSidebarUi* this, RawTime dt);

void PlayerSidebarUi_setMoney(PlayerSidebarUi* this, const int money);
