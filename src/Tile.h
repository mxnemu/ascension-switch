#pragma once

#include "Constants.h"
#include "Sprite.h"
#include "TextureCache.h"

#define TILE_LADDER 2

CLASS(TilePhysics) {
	SDL_Rect bounds;
};

CLASS(Tile) {
	Sprite* sprite;
	int type;
	bool blocks;
	TilePhysics physics;
};

Tile* Tile_create(int type, TextureCache* tc, const char* colorPrefix);
void Tile_destroy(Tile* tile);
bool Tile_initByType(Tile* this, TextureCache* tc, const char* colorPrefix);
void Tile_setPosition(Tile* this, SDL_Point);
