#include "DungeonMasterUi.h"

#include "Scene.h"
#include "Utils.h"
#include "Enemy.h"
#include "Tile.h"

DungeonMasterUi* DungeonMasterUi_create(Player* player) {
	DungeonMasterUi* this = malloc(sizeof(DungeonMasterUi));
	this->cards = List_create();
	SDL_Renderer* renderer = player->scene->engine->renderer;
	TextureCache* tc = player->scene->engine->textureCache;

	this->player = player;

	this->background.x = player->scene->camera->translation.x;
	this->background.y = player->scene->camera->translation.y;
	this->background.w = player->scene->background->bounds.w;
	this->background.h = player->scene->background->bounds.h;

	this->cardFont = TTF_OpenFont("fonts/Black-Chancery.ttf", 16);

	SDL_Point p = {.x = player->scene->camera->translation.x, .y=250};
	List_pushBack(this->cards, Card_create(p, renderer, tc, "images/uiWhiteKnight.png", CARD_SUMMON_WHITKNIGHT, 10, this->cardFont, "Summon Skeleton Knight"));
	p.x += ((Card*)this->cards->first->data)->background->bounds.w + 5;
	List_pushBack(this->cards, Card_create(p, renderer, tc, "images/blue/cursor.png", CARD_CONTROL_STRONGEST, 5, this->cardFont, "Control strongest enemy"));
	p.x += ((Card*)this->cards->first->data)->background->bounds.w + 5;
	//List_pushBack(this->cards, Card_create(renderer, tc, "images/uiWhiteKnight.png", CARD_SUMMON_WHITKNIGHT, 15, this->cardFont, "Summon Skeleton Knight"));

	SDL_Color white = {.r=255, .g=255, .b=255, .a=255};
	p.x = player->scene->camera->translation.x;

	char hintActionHotkey[256];
	char* hotkeyName = Input_keycodeToPrintable(Input_getHotkeyForAction(player->input, action));
	sprintf(hintActionHotkey, "Use: %s", hotkeyName);
	free(hotkeyName);
	this->hintSummonHotkey = Sprite_create(SDL_CreateTextureFromSurface(
			renderer,
			TTF_RenderUTF8_Blended(this->cardFont, hintActionHotkey, white)
		)
	);
	this->hintSummonHotkey->bounds.x = p.x;
	this->hintSummonHotkey->bounds.y = p.y += ((Card*)this->cards->first->data)->background->bounds.h + 5;

	char hintNextHotkey[256];
	hotkeyName = Input_keycodeToPrintable(Input_getHotkeyForAction(player->input, attackSword));
	sprintf(hintNextHotkey, "N. %s", hotkeyName);
	free(hotkeyName);
	this->hintNextHotkey = Sprite_create(SDL_CreateTextureFromSurface(
			renderer,
			TTF_RenderUTF8_Blended(this->cardFont, hintNextHotkey, white)
		)
	);
	this->hintNextHotkey->bounds.x = p.x;
	this->hintNextHotkey->bounds.y = this->hintSummonHotkey->bounds.y + this->hintSummonHotkey->bounds.h;

	char hintPreviousHotkey[256];
	hotkeyName = Input_keycodeToPrintable(Input_getHotkeyForAction(player->input, attackBow));
	sprintf(hintPreviousHotkey, "P. %s", hotkeyName);
	free(hotkeyName);
	this->hintPreviousHotkey = Sprite_create(SDL_CreateTextureFromSurface(
			renderer,
			TTF_RenderUTF8_Blended(this->cardFont, hintPreviousHotkey, white)
		)
	);
	this->hintPreviousHotkey->bounds.x = p.x;
	this->hintPreviousHotkey->bounds.y = this->hintNextHotkey->bounds.y + this->hintNextHotkey->bounds.h;

	char indicatorpath[128];
	sprintf(indicatorpath, "images/%s/indicator.png", player->scene->colorPrefix);
	this->indicator = Sprite_create(TextureCache_getForUnconstantPath(tc, indicatorpath));

	this->indicator->bounds.y = ((Card*)this->cards->first->data)->background->bounds.y - this->indicator->bounds.h -2;
	this->selected = this->cards->first;
	return this;
}

void DungeonMasterUi_destroy(DungeonMasterUi* this) {
	FREE_LIST_WITH_ELMENTS(this->cards, Card_destroy);
	TTF_CloseFont(this->cardFont);
}

void DungeonMasterUi_summonSelectedCard(DungeonMasterUi* this, Scene* scene, SDL_Point tilePosition) {
	Card* card = this->selected->data;
	if (card && this->player->money >= card->cost) {
		Player_earnMoney(this->player, -card->cost);
		if (card->type == CARD_SUMMON_WHITKNIGHT) {
			if (Scene_getTile(scene, tilePosition) == NULL) {
				Enemy* enemy = Enemy_createWhiteKnight(scene, tilePosition);
				Scene_addEntity(scene, enemy->entity);
			}
		} else if (card->type == CARD_CONTROL_STRONGEST) {
			Entity* strongest = NULL;
			for (int i=0; i < scene->entities->usedElements; ++i) {
				Entity* e = scene->entities->elements[i];
				if (e && (e->physics.belongsToGroups & COLLISION_GROUP_ENEMY)) {
					if (!strongest || e->attack1 > strongest->attack1) {
						strongest = e;
					}
				}
			}
			if (strongest) {
				ControlledEntity_set(&this->player->controlledEntity, strongest);
			}
		}
	}
}

void DungeonMasterUi_selectNext(DungeonMasterUi* this) {
	if (this->selected->next) {
		this->selected = this->selected->next;
	}
}

void DungeonMasterUi_selectPrevious(DungeonMasterUi* this) {
	if (this->selected->previous) {
		this->selected = this->selected->previous;
	}
}

void DungeonMasterUi_draw(DungeonMasterUi* this, SDL_Renderer* renderer) {

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 205);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &this->background);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	ListNode* it = this->cards->first;
	while (it) {
		Card* card = it->data;
		if (card) {
			Card_draw(card, renderer);
		}
		it = it->next;
	}
	Card* selected = this->selected->data;
	this->indicator->bounds.x = selected->background->bounds.x + (selected->background->bounds.w/2) - (this->indicator->bounds.w/2);

	Sprite_draw(this->indicator, renderer);
	Sprite_draw(this->hintNextHotkey, renderer);
	Sprite_draw(this->hintPreviousHotkey, renderer);
	Sprite_draw(this->hintSummonHotkey, renderer);
}

void DungeonMasterUi_update(DungeonMasterUi* this, RawTime dt) {

}

Card* Card_create(SDL_Point p, SDL_Renderer* renderer, TextureCache* tc, const char* picture, int type, int cost, TTF_Font* font, const char* description) {
	Card* this = malloc(sizeof(Card));
	this->background = Sprite_create(TextureCache_get(tc, "images/card.png"));
	this->coin = Sprite_create(TextureCache_get(tc, "images/uiCoin.png"));
	this->picture = Sprite_create(TextureCache_get(tc, picture));
	this->cost = cost;
	this->type = type;
	SDL_Color white = {.r=255, .g=255, .b=255, .a=255};
	this->description = Sprite_create(
		SDL_CreateTextureFromSurface(
			renderer,
			TTF_RenderUTF8_Blended(font, description, white)
		)
	);

	char costString[128];
	sprintf(costString, "%d", cost);
	this->costLabel = Sprite_create(
		SDL_CreateTextureFromSurface(
			renderer,
			TTF_RenderUTF8_Blended(font, costString, white)
		)
	);

	this->background->bounds.x = p.x;
	this->background->bounds.y = p.y;

	this->costLabel->bounds.x = p.x + 7;
	this->costLabel->bounds.y = p.y + 90;

	this->coin->bounds.x = p.x + 7 + this->costLabel->bounds.w;
	this->coin->bounds.y = p.y + 90;

	this->picture->bounds.x = p.x + 38;
	this->picture->bounds.y = p.y + 45;

	this->description->bounds.x = p.x + 7;
	this->description->bounds.y = p.y + 90 + this->costLabel->bounds.h + 3;

	return this;
}

void Card_draw(Card* this, SDL_Renderer* renderer) {
	Sprite_draw(this->background, renderer);
	Sprite_draw(this->picture, renderer);
	Sprite_draw(this->description, renderer);
	Sprite_draw(this->costLabel, renderer);
	Sprite_draw(this->coin, renderer);
}

void Card_destroy(Card* this) {
	Sprite_destroy(this->background);
	Sprite_destroy(this->picture);
	Sprite_destroy(this->description);
	Sprite_destroy(this->costLabel);
	Sprite_destroy(this->coin);
	free(this);
}
