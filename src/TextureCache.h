/*
 * TextureCache.h
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#pragma once

#include "Constants.h"
#include "Vector.h"


#define SAFE_TEXTURE_PATH "images/noImg.png"

typedef struct TextureCache {
	Vector* images;
	SDL_Renderer* renderer;
} TextureCache;

typedef struct TextureEntry {
	const char* key;
	char* ownedKey; // alt if key is NULL
	SDL_Texture* image;
	bool ownsImage;
} TextureEntry;

TextureCache* TextureCache_create(SDL_Renderer* renderer);
void TextureCache_destroy(TextureCache* this);
SDL_Texture* TextureCache_get(TextureCache* this, const char* path);
SDL_Texture* TextureCache_loadImage(TextureCache* this, const char* path, bool store);

TextureEntry* TextureEntry_create(const char* key, SDL_Texture* image, bool ownsImage);
void TextureEntry_destroy(TextureEntry* this);

SDL_Texture* TextureCache_getForUnconstantPath(TextureCache* this, char* path);
TextureEntry* TextureCache_getExistingEntry(TextureCache* this, const char* path);

