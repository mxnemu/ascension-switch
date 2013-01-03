#include "Scene.h"
#include "Utils.h"
#include "Entity.h"

/*
Scene* Scene_create() {
	Scene* this = malloc(sizeof(Scene));
	this->backgrounds = List_create();
	this->camera = Camera_create(engine->screen->w, engine->screen->h);
	this->luaFile = luaFile;
	this->engine = engine;
	this->leftBackground = NULL;
	this->rightBackground = NULL;
	this->entities = Vector_Create();
	return this;
}
*/

int Scene_luaCreate(lua_State* l) {
	Scene* this = lua_newuserdata(l, sizeof(Scene));
	luaL_getmetatable(l, SCENE_LUA_MTABLE);
	lua_setmetatable(l, -2);
	//lua_pushvalue(l, -1);

	this->backgrounds = List_create();
	this->leftBackground = NULL;
	this->rightBackground = NULL;
	this->entities = Vector_Create();

	return 1;
}

Scene* Scene_create(GameEngine* engine, const char* filePath) {
	if (luaL_dofile(engine->l, filePath)) {
		printf("Could not execute Lua scene file: %s\n", filePath);
		return NULL;
	}
	stackDump(engine->l);

	lua_getglobal(engine->l, "scene");
	Scene* scene = Scene_checkfromLua(engine->l);
	scene->camera = Camera_create(engine->screen->w, engine->screen->h);
	return scene;
}

void Scene_destroy(Scene* this) {

	for (int i=0; i < this->entities->allocatedElements; ++i) {
		Entity* it = this->entities->elements[i];
		if (it != NULL) {
			Entity_destroy(it);
		}
	}

	ListNode* it = this->backgrounds->first;
	while (it) {
		Sprite_destroy(it->data);
		it = it->next;
	}

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

	for (int i=0; i < this->entities->allocatedElements; ++i) {
		Entity* it = this->entities->elements[i];
		if (it != NULL) {
			it->update(it->context, dt);
		}
	}
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


// Lua

void Scene_luaExport(lua_State *l) {
	static struct luaL_Reg methods[] = {
		{"addBackground", Scene_luaAddBackground},
		{NULL, NULL}
	};

	static struct luaL_Reg functions[] = {
		{"create", Scene_luaCreate},
		{NULL, NULL}
	};

	lua_createLib(l, SCENE_LUA_MTABLE, "Scene", functions, methods);
}

Scene* Scene_checkfromLua (lua_State *L) {
	void *userData = luaL_checkudata(L, 1, SCENE_LUA_MTABLE);
	luaL_argcheck(L, (userData != NULL), 1, "`Scene' expected");
	return (Scene*) userData;
}

int Scene_luaAddBackground(lua_State* l) {
	stackDump(l);
	Scene* this = Scene_checkfromLua(l);
	const char* background = luaL_checkstring(l, 2);
	Scene_addBackground(this, background);
	lua_pop(l,2);
	return 0;
}
