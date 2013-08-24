#include "Tile.h"
#include "List.h"

Tile* Tile_create(const int type, TextureCache* tc, const char* colorPrefix) {
	Tile* this = malloc(sizeof(Tile));
	this->type = type;
	this->blocks = true;
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
	} else {
		return false;
	}
	return true;
}
