#include "Scene.h"

Scene* Scene_create(GameEngine* engine, const char* luaFile) {
	Scene* this = malloc(sizeof(Scene));
	this->backgrounds = List_create();
	this->camera = Camera_create(engine->screen->w, engine->screen->h);
	this->luaFile = luaFile;
	this->engine = engine;
	return this;
}

void Scene_init(Scene* this) {
	luaL_dofile(this->engine->l, this->luaFile);
}

void Scene_destroy(Scene* this) {
	List_destroy(this->backgrounds); // TODO delete content
	free(this);
}

void Scene_update(Scene* this, RawTime dt) {
	// swap backgrounds
	if (this->leftBackground->rect.x + this->leftBackground->rect.w < this->camera->viewport.x) {
		ListNode* node = List_getNodeForData(this->backgrounds, this->rightBackground);
		node = node->previous ? node->previous : this->backgrounds->first;
		this->leftBackground = this->rightBackground;
		this->rightBackground = node->data;
	}
	if (this->rightBackground->rect.x > this->camera->viewport.x + this->camera->viewport.w) {
		ListNode* node = List_getNodeForData(this->backgrounds, this->rightBackground);
		node = node->next ? node->next : this->backgrounds->last;
		this->rightBackground = this->leftBackground;
		this->leftBackground = node->data;
	}


}

void Scene_draw(Scene* this, SDL_Surface* surface) {
	Sprite_drawOnCamera(this->leftBackground, surface, this->camera);
	Sprite_drawOnCamera(this->rightBackground, surface, this->camera);
}
