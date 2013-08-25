#include "PlayerSidebarUi.h"
#include "Scene.h"
#include "Player.h"

PlayerSidebarUi* PlayerSidebarUi_create(void* player, SDL_Point pos) {
	Player* p = player;
	PlayerSidebarUi* this = malloc(sizeof(PlayerSidebarUi));
	this->player = player;
	this->coin = Sprite_create(TextureCache_get(p->scene->engine->textureCache, "images/uiCoin.png"));
	this->font = TTF_OpenFont("fonts/Black-Chancery.ttf", 16);
	this->color.r = 255;
	this->color.g = 255;
	this->color.b = 255;
	this->color.a = 255;
	this->money = NULL;
	this->position = pos;

	char path[128];
	sprintf(path, "images/%s/playerIcon.png", p->scene->colorPrefix);
	this->playerIcon = Sprite_create(TextureCache_getForUnconstantPath(p->scene->engine->textureCache, path));

	this->playerIcon->bounds.x = this->position.x;
	this->playerIcon->bounds.y = this->position.y;

	PlayerSidebarUi_setMoney(this, p->money);
	return this;
}

void PlayerSidebarUi_destroy(PlayerSidebarUi* this) {
	Sprite_destroy(this->playerIcon);
	Sprite_destroy(this->coin);
	Sprite_destroy(this->money);
	free(this);
}

void PlayerSidebarUi_draw(PlayerSidebarUi* this, SDL_Renderer* renderer) {
	CANCEL_IF_FALSE(this);
	Sprite_draw(this->playerIcon, renderer);
	Sprite_draw(this->coin, renderer);
	Sprite_draw(this->money, renderer);
}

void PlayerSidebarUi_setMoney(PlayerSidebarUi* this, const int money) {

	char moneyString[24];
	sprintf(moneyString, "%d", money);
	if (this->money) {
		Sprite_destroy(this->money);
	}
	this->money = Sprite_create(SDL_CreateTextureFromSurface(
			((Player*)this->player)->scene->engine->renderer,
			TTF_RenderUTF8_Blended(this->font, moneyString, this->color)
		)
	);

	this->money->bounds.x = this->position.x;
	this->money->bounds.y = this->position.y + this->playerIcon->bounds.h + 2;
	this->coin->bounds.x = this->money->bounds.x + this->money->bounds.w;
	this->coin->bounds.y = this->money->bounds.y + (this->money->bounds.h/2) - (this->coin->bounds.h/2);
}
