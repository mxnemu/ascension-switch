#include "EntityEditor.h"


EntityEditor* EntityEditor_create(GameEngine* engine) {
	EntityEditor* this = malloc(sizeof(EntityEditor));
	this->engine = engine;
	this->spritemap = TextureCache_get(this->engine->textureCache, "images/person.png");
	this->module = GameModule_create(this);
	this->module->draw = EntityEditor_draw;
	return this;
}

void EntityEditor_destroy(EntityEditor* this) {
	free(this);
}


void EntityEditor_draw(void* context, SDL_Surface* surface) {
	EntityEditor* this = context;
	SDL_Rect spritemapRect = {.x=0, .y=0, .w= this->spritemap->w, .h= this->spritemap->h};
	SDL_FillRect(surface, &spritemapRect, 0xFF00FF);
	SDL_BlitSurface(this->spritemap, &spritemapRect, surface, NULL);
}
