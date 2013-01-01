#include "Scene.h"
#include "Utils.h"

Scene* Scene_create(GameEngine* engine, const char* luaFile) {
	Scene* this = malloc(sizeof(Scene));
	this->backgrounds = List_create();
	this->camera = Camera_create(engine->screen->w, engine->screen->h);
	this->luaFile = luaFile;
	this->engine = engine;
	this->leftBackground = NULL;
	this->rightBackground = NULL;
	return this;
}

void Scene_init(Scene* this) {
	lua_State* l = this->engine->l;
	if (luaL_dofile(this->engine->l, this->luaFile)) {
		printf("Could not execute Lua scene file: %s\n", this->luaFile);
		return;
	}

	lua_getglobal(l, "backgrounds");
	if (lua_istable(l, -1)) {
		int i = 1;
		lua_pushinteger(l, i);
		lua_gettable(l, -2);
		while(lua_isstring(l, -1)) {
			const char* background = lua_tostring(l, -1);
			printf("oh my %s\n", background);
			Scene_addBackground(this, background);

			++i;
			lua_pop(l, 1);
			lua_pushinteger(l, i);
			lua_gettable(l, -2);
		}
		lua_pop(l, 2);
	}
}

void Scene_destroy(Scene* this) {
	List_destroy(this->backgrounds); // TODO delete content
	free(this);
}

void Scene_update(Scene* this, RawTime dt) {
//	// load new right background when the left leaves the screen
//	if (this->leftBackground->rect.x + this->leftBackground->rect.w < this->camera->viewport.x) {
//		printf("left out\n");
//		ListNode* node = List_getNodeForData(this->backgrounds, this->rightBackground);
//		node = node->previous ? node->previous : this->backgrounds->first;
//		this->leftBackground = this->rightBackground;
//		this->rightBackground = node->data;
//	}
//	// load new left background when the right leaves the screen
//	if (this->rightBackground->rect.x > this->camera->viewport.x + this->camera->viewport.w) {
//		printf("right out\n");
//		ListNode* node = List_getNodeForData(this->backgrounds, this->leftBackground);
//		node = node->previous ? node->previous : this->backgrounds->last;
//		this->rightBackground = this->leftBackground;
//		this->leftBackground = node->data;
//	}
}

void Scene_draw(Scene* this, SDL_Surface* surface) {
//	Sprite_drawOnCamera(this->leftBackground, surface, this->camera);
//	Sprite_drawOnCamera(this->rightBackground, surface, this->camera);
	ListNode* it = this->backgrounds->first;
	while (it) {
		Sprite_drawOnCamera(it->data, surface, this->camera);
		it = it->next;
	}
}

void Scene_addBackground(Scene* this, const char* background) {
	Sprite* sprite = Sprite_create(TextureCache_get(this->engine->textureCache, background));
	if (this->backgrounds->last) {
		Sprite* lastSprite = this->backgrounds->last->data;
		sprite->rect.x = lastSprite->rect.x + lastSprite->rect.w;
	}

	List_pushBack(this->backgrounds, sprite);

	if (!this->leftBackground) {
		this->leftBackground = sprite;
	}
	if (!this->rightBackground || this->rightBackground == this->leftBackground) {
		this->rightBackground = sprite;
	}
	this->camera->bounds.w += sprite->rect.w;
}
