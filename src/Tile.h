#pragma once

#include "Constants.h"
#include "Sprite.h"
#include "TextureCache.h"

CLASS(Tile) {
	Sprite* sprite;
	int type;
	bool blocks;
};

Tile* Tile_create(int type, TextureCache* tc, const char* colorPrefix);
void Tile_destroy(Tile* tile);
bool Tile_initByType(Tile* this, TextureCache* tc, const char* colorPrefix);
