/*
 * TextureCache.c
 *
 *  Created on: 25.12.2012
 *      Author: nehmulos
 */

#include "TextureCache.h"

TextureCache* TextureCache_create(void) {
	TextureCache* this = malloc(sizeof(TextureCache));
	this->images = Vector_Create();
	return this;
}

void TextureCache_destroy(TextureCache* this) {
	CANCEL_IF_FALSE(this);
	for (int i=0; i < this->images->usedElements; ++i) {
		TextureEntry* it = this->images->elements[i];
		if (it) {
			TextureEntry_destroy(it);
		}
	}
	Vector_Destroy(this->images);
	free(this);
}

SDL_Surface* TextureCache_get(TextureCache* this, const char* path) {

	for (int i=0; i < this->images->usedElements; ++i) {
		TextureEntry* it = this->images->elements[i];
		if (it && 0 == strcmp(it->key, path)) {
			return it->image;
		}
	}
	return TextureCache_loadImage(this, path, true);
}

SDL_Surface* TextureCache_loadImage(TextureCache* this, const char* path, bool store) {
	SDL_Surface* originalImage = IMG_Load(path);
	SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(originalImage);
	SDL_FreeSurface(originalImage);
	if (this && store) {
		Vector_AddElement(this->images, TextureEntry_create(path, optimizedImage , true));
	}
	return optimizedImage;
}

TextureEntry* TextureEntry_create(const char* key, SDL_Surface* image, bool ownsImage) {
	TextureEntry* this = malloc(sizeof(TextureEntry));
	this->key = key;
	this->image = image;
	this->ownsImage = ownsImage;
	return this;
}

void TextureEntry_destroy(TextureEntry* this) {
	CANCEL_IF_FALSE(this);
	if (this->ownsImage) {
		SDL_FreeSurface(this->image);
	}
	free(this);
}
