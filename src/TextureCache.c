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
		}
	}
	Vector_Destroy(this->images);
	free(this);
}

SDL_Texture* TextureCache_get(TextureCache* this, const char* path) {

	for (int i=0; i < this->images->usedElements; ++i) {
		TextureEntry* it = this->images->elements[i];
		if (it && 0 == strcmp(it->key, path)) {
			return it->image;
		}
	}
	SDL_Texture* tex = TextureCache_loadImage(this, path, true);
	if (!tex && strcmp(path, SAFE_TEXTURE_PATH) != 0) {
		tex = TextureCache_get(this, SAFE_TEXTURE_PATH);
	}
	return tex;
}

SDL_Texture* TextureCache_loadImage(TextureCache* this, const char* path, bool store) {
	SDL_Texture* originalImage = IMG_LoadTexture(this->renderer, path);
	if (originalImage) {
		//SDL_Texture* optimizedImage = SDL_DisplayFormatAlpha(originalImage);
		//SDL_FreeSurface(originalImage);
		if (this && store) {
			Vector_AddElement(this->images, TextureEntry_create(path, originalImage , true));
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
	return this;
}

void TextureEntry_destroy(TextureEntry* this) {
	CANCEL_IF_FALSE(this);
	if (this->ownsImage) {
		SDL_DestroyTexture(this->image);
	}
	free(this);
}
