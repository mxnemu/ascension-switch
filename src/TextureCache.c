/*
 * TextureCache.c
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#include "TextureCache.h"

TextureCache* TextureCache_create(SDL_Renderer* renderer) {
	TextureCache* this = malloc(sizeof(TextureCache));
	this->images = Vector_Create();
	this->renderer = renderer;
	return this;
}

void TextureCache_destroy(TextureCache* this) {
	CANCEL_IF_FALSE(this);
	for (int i=0; i < this->images->usedElements; ++i) {
		TextureEntry* it = this->images->elements[i];
		if (it) {
			TextureEntry_destroy(it);
			this->images->elements[i] = NULL;
		}
	}
	Vector_Destroy(this->images);
	free(this);
}

SDL_Texture* TextureCache_get(TextureCache* this, const char* path) {
	TextureEntry* entry = TextureCache_getExistingEntry(this, path);
	if (entry) {
		return entry->image;
	}
	SDL_Texture* tex = TextureCache_loadImage(this, path, true);
	if (!tex && strcmp(path, SAFE_TEXTURE_PATH) != 0) {
		tex = TextureCache_get(this, SAFE_TEXTURE_PATH);
	}
	return tex;
}
TextureEntry* TextureCache_getExistingEntry(TextureCache* this, const char* path) {
	for (int i=0; i < this->images->usedElements; ++i) {
		TextureEntry* it = this->images->elements[i];
		if (it) {
			if ((it->key && 0 == strcmp(it->key, path)) ||
				(it->ownedKey && 0 == strcmp(it->ownedKey, path))) {
				return it;
			}
		}
	}
	return NULL;
}

SDL_Texture* TextureCache_getForUnconstantPath(TextureCache* this, char* path) {
	TextureEntry* entry = TextureCache_getExistingEntry(this, path);
	if (entry) {
		return entry->image;
	}
	SDL_Texture* tex = TextureCache_loadImage(this, path, true);
	if (!tex && strcmp(path, SAFE_TEXTURE_PATH) != 0) {
		tex = TextureCache_get(this, SAFE_TEXTURE_PATH);
	} else {
		entry = TextureCache_getExistingEntry(this, path);
		if (!entry) {
			printf("ÈRROR RECENTLY INSERTED TEXTURE NOT CACHED see %s:%d", __FILE__, __LINE__);
		}
		entry->ownedKey = malloc((strlen(entry->key)+1) * sizeof(char));
		strcpy(entry->ownedKey, entry->key);
		entry->key = NULL;
	}
	return tex;
}

SDL_Texture* TextureCache_loadImage(TextureCache* this, const char* path, bool store) {
	SDL_Texture* originalImage = IMG_LoadTexture(this->renderer, path);
	if (originalImage) {
		//SDL_Texture* optimizedImage = SDL_DisplayFormatAlpha(originalImage);
		//SDL_FreeSurface(originalImage);
		if (this && store) {
			Vector_InsertInFirstFreeSpace(this->images, TextureEntry_create(path, originalImage , true));
		}
		return originalImage;
	}
	printf("Could not load image: %s\n", path);
	return NULL;
}

TextureEntry* TextureEntry_create(const char* key, SDL_Texture* image, bool ownsImage) {
	TextureEntry* this = malloc(sizeof(TextureEntry));
	this->key = key;
	this->image = image;
	this->ownsImage = ownsImage;
	this->ownedKey = NULL;
	return this;
}

void TextureEntry_destroy(TextureEntry* this) {
	CANCEL_IF_FALSE(this);
	if (this->ownsImage) {
		SDL_DestroyTexture(this->image);
	}
	if (this->ownedKey) {
		free(this->ownedKey);
	}
	free(this);
}
