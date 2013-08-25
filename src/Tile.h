#pragma once

#include "Constants.h"
#include "Sprite.h"
#include "TextureCache.h"

#define TILE_NONE 0
#define TILE_BRICK 1
#define TILE_LADDER 2

#define TILE_DOOR_4 4
#define TILE_DOOR_5 5
#define TILE_DOOR_6 6

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
