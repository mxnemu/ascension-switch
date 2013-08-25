#include "Tile.h"
#include "List.h"

Tile* Tile_create(const int type, TextureCache* tc, const char* colorPrefix) {
	Tile* this = malloc(sizeof(Tile));
	this->type = type;
	this->blocks = true;
	SDL_RectEmpty(&this->physics.bounds);
	if (!Tile_initByType(this, tc, colorPrefix)) {
		free(this);
		return NULL;
	}
	return this;
}

void Tile_destroy(Tile* this) {
	Sprite_destroy(this->sprite);
	free(this);
}

bool Tile_initByType(Tile* this, TextureCache* tc, const char* colorPrefix) {
	if (this->type == 1) {
		char* path = malloc(128 * sizeof(char));
		sprintf(path, "images/%s/brick.png", colorPrefix);
		this->sprite = Sprite_create(TextureCache_getForUnconstantPath(tc, path));
		free(path);
	} else if (this->type == 2) {
		this->sprite = Sprite_create(TextureCache_get(tc, "images/ladder.png"));
		this->blocks = false;
	} else if (this->type >= TILE_DOOR_4 && this->type <= TILE_DOOR_6) {
		this->sprite = Sprite_create(TextureCache_get(tc, "images/door.png"));
	} else {
		return false;
	}
	return true;
}

void Tile_setPosition(Tile* this, SDL_Point p) {
	this->sprite->bounds.x = p.x;
	this->sprite->bounds.y = p.y;

	this->physics.bounds.x = p.x * PHYSICS_SCALE;
	this->physics.bounds.y = p.y * PHYSICS_SCALE;
	this->physics.bounds.w = this->sprite->bounds.w * PHYSICS_SCALE; // TODO set directly when setting the sprite
	this->physics.bounds.h = this->sprite->bounds.h * PHYSICS_SCALE;
}
