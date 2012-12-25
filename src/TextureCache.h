/*
 * TextureCache.h
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"
#include "Vector.h"


typedef struct TextureCache {
	Vector* images;
} TextureCache;

typedef struct TextureEntry {
	const char* key;
	SDL_Surface* image;
	bool ownsImage;
} TextureEntry;

TextureCache* TextureCache_create(void);
void TextureCache_destroy(TextureCache* this);
SDL_Surface* TextureCache_get(TextureCache* this, const char* path);
SDL_Surface* TextureCache_loadImage(TextureCache* this, const char* path, bool store);

TextureEntry* TextureEntry_create(const char* key, SDL_Surface* image, bool ownsImage);
void TextureEntry_destroy(TextureEntry* this);

