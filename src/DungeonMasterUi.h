#pragma once

#include "Player.h"

#define CARD_SUMMON_WHITKNIGHT 1
#define CARD_CONTROL_STRONGEST 2
#define CARD_SUMMON_BOULDER 3

typedef struct Player Player;
CLASS(Card) {
	Sprite* sprite;
	Sprite* costLabel;
	Sprite* coin;
	Sprite* background;
	Sprite* description;
	Sprite* picture;

	int type;
	int cost;
};

CLASS(DungeonMasterUi) {
	Player* player;
	ListNode* selected;
	List* cards;
	TTF_Font* cardFont;

	Sprite* hintSummonHotkey;
	Sprite* hintNextHotkey;
	Sprite* hintPreviousHotkey;
	Sprite* indicator;
	SDL_Rect background;
};

DungeonMasterUi* DungeonMasterUi_create(Player* player);
void DungeonMasterUi_destroy(DungeonMasterUi* this);

void DungeonMasterUi_draw(DungeonMasterUi* this, SDL_Renderer* renderer);
void DungeonMasterUi_update(DungeonMasterUi* this, RawTime dt);

void DungeonMasterUi_summonSelectedCard(DungeonMasterUi* this, Scene* scene, SDL_Point tilePosition);
void DungeonMasterUi_selectNext(DungeonMasterUi* this);
void DungeonMasterUi_selectPrevious(DungeonMasterUi* this);

Card* Card_create(SDL_Point p, SDL_Renderer* renderer, TextureCache* tc, const char* picture, int type, int cost, TTF_Font* font, const char* description);
void Card_draw(Card* this, SDL_Renderer* renderer);
void Card_destroy(Card* this);
