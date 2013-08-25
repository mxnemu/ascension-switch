#include "Scene.h"
#include "Utils.h"
#include "Entity.h"
#include "Trigger.h"
#include "Tile.h"
#include "Collectable.h"

int Scene_luaCreate(lua_State* l) {
	Scene* this = lua_newuserdata(l, sizeof(Scene));
	luaL_getmetatable(l, SCENE_MTABLE);
	lua_setmetatable(l, -2);
	GameEngine* engine = *((GameEngine**)lua_checklightuserdata(l, 1));
	luaL_argcheck(l, (engine != NULL), 1, "please set the global 'gameEngine' as 1st argument of Scene.create");
	Scene* lastScene = *((Scene**)lua_checklightuserdata(l, 2));

	this->engine = engine;
	SDL_Point screenSize;
	SDL_RenderGetLogicalSize(engine->renderer, &screenSize.x, &screenSize.y);
	this->camera = Camera_create(screenSize.x, screenSize.y);
	this->walkableBounds.x = 0;
	this->walkableBounds.w = 0;
	this->walkableBounds.y = 295 * PHYSICS_SCALE;
	this->walkableBounds.h = 185 * PHYSICS_SCALE;

	//lua_pushvalue(l, -1);
	this->entities = Vector_Create();
	this->triggers = Vector_Create();
	this->tiles = Vector_Create();
	this->collisionHandler = CollisionHandler_create();

	if (lastScene) {
		this->mirrorTiles = lastScene->mirrorTiles;
		this->colorPrefix = lastScene->colorPrefix;
	} else {
		this->mirrorTiles = false;
		this->colorPrefix = "red";
	}
	char* path = malloc(128 * sizeof(char));
	sprintf(path, "images/%s/bg.png", this->colorPrefix);
	this->background = Sprite_create(TextureCache_getForUnconstantPath(engine->textureCache, path));
	free(path);

	//_Vector_RecreateWithoutSizeCheck(this->tiles, SCENE_TILES_X*SCENE_TILES_Y);
	for (int i=0; i < SCENE_TILES_X*SCENE_TILES_Y; ++i) {
		Vector_AddElement(this->tiles, NULL);
	}

	this->gravity = (32*5) / PHYSICS_SCALE; // flall 5 tiles per second

	return 1;
}

Scene* Scene_create(GameEngine* engine, const char* filePath, Scene* lastScene) {
	lua_pushlightuserdata(engine->l, &engine);
	lua_setglobal(engine->l, "gameEngine");
	lua_pushlightuserdata(engine->l, &lastScene);
	lua_setglobal(engine->l, "lastScene");
//	lua_pushnil(engine->l);
//	lua_setglobal(engine->l, "scene");
	if (luaL_dofile(engine->l, filePath)) {
		printf("Could not execute Lua scene file: %s\n", filePath);
		printf("%s\n", lua_tostring(engine->l, -1));
		return NULL;
	}
	stackDump(engine->l);

	lua_getglobal(engine->l, "scene");
	Scene* scene = Scene_checkfromLua(engine->l, -1);

	if (scene) {
		lua_gc(engine->l, LUA_GCSTEP,0);
	}
	return scene;
}

int Scene_luaDestroy(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	FREE_VECTOR_WITH_ELMENTS(this->entities, Entity_destroy);
	// lua probably deals whit everything.
	//FREE_VECTOR_WITH_ELMENTS(this->triggers, Trigger_destroy);
	Vector_Destroy(this->triggers);

	Sprite_destroy(this->background);
//	free(this); // lua does this now
	printf("destroyed scene via gc\n");
	return 0;
}

void Scene_update(Scene* this, RawTime dt) {
	//CollisionHandler_update(this->collisionHandler, this->entities);

	for (int i=0; i < this->entities->usedElements; ++i) {
		Entity* it = this->entities->elements[i];
		if (it != NULL) {
//			it->update(it->context, dt);
			Entity_update(it, dt);
			if (it->destroyed) {
				Entity_destroy(it);
				Vector_Set(this->entities, i, NULL);
			} else if (it->physics.groundedStatus == grounded || it->physics.groundedStatus == inAir) {
				it->physics.dy += this->gravity;
			}

		}
	}
	Camera_update(this->camera);
}

void Scene_draw(Scene* this, SDL_Renderer* renderer) {
	Sprite_drawOnCamera(this->background, renderer, this->camera);

	for (int i=0; i < this->tiles->usedElements; ++i) {
		Tile* it = this->tiles->elements[i];
		if (it != NULL) {
			Sprite_drawOnCamera(it->sprite, renderer, this->camera);
		}
	}

	for (int i=0; i < this->entities->usedElements; ++i) {
		Entity* it = this->entities->elements[i];
		if (it != NULL) {
			it->draw(it->context, renderer, this->camera);
		}
	}
}

void Scene_addEntity(Scene* this, Entity* entity) {
	Vector_InsertInFirstFreeSpace(this->entities, entity);
}

void Scene_setBounds(Scene* this, int x, int y, int w, int h) {
	this->walkableBounds.x = x * PHYSICS_SCALE;
	this->walkableBounds.y = y * PHYSICS_SCALE;
	this->walkableBounds.w = w * PHYSICS_SCALE;
	this->walkableBounds.h = h * PHYSICS_SCALE;
	this->camera->bounds.x = x;
	this->camera->bounds.y = y;
	this->camera->bounds.w = w;
	this->camera->bounds.h = h;
}

void Scene_setTile(Scene* this, const int tileId, const int tileType) {
	Tile* oldTile = Vector_Get(this->tiles, tileId);
	if (oldTile) {
		Tile_destroy(oldTile);
	}
	Tile* t = Tile_create(tileType, this->engine->textureCache, this->colorPrefix);

	if (t) {
		SDL_Point p = Scene_positionForTileId(this, tileId);
		p.x *= TILE_W;
		p.y *= TILE_H;
		Tile_setPosition(t, p);
		Vector_Set(this->tiles, tileId, t);
	}

	printf("%d,", tileType);
}

SDL_Point Scene_positionForTileId(Scene* this, const int tileId) {
	SDL_Point p;
	if (tileId != 0) {
		p.x = tileId % SCENE_TILES_X;
		p.y = tileId / SCENE_TILES_X;
	} else {
		p.x = 0;
		p.y = 0;
	}
	if (this->mirrorTiles) {
		p.x = (SCENE_TILES_X-1) - p.x;
	}
	return p;
}

void Scene_openDoors(Scene* this, int type) {
	for (int i=0; i < this->tiles->usedElements; ++i) {
		Tile* tile = this->tiles->elements[i];
		if (tile != NULL && tile->type == type) {
			Scene_setTile(this, i, TILE_NONE);
		}
	}
}

void Scene_spawnCollectable(Scene* this, int tileId, int type) {
	Collectable* c = NULL;
	if (type == COLLECTABLE_COIN) {
		c = Collectable_createCoin(this, Scene_positionForTileId(this, tileId), 5);
	} else if (type == COLLECTABLE_COIN_BIG) {
		c = Collectable_createCoin(this, Scene_positionForTileId(this, tileId), 25);
	} else if (type >= COLLECTABLE_KEY_4 && type <= COLLECTABLE_KEY_6) {
		c = Collectable_createKey(this, Scene_positionForTileId(this, tileId), type);
	} else if (type == COLLECTABLE_COIN) {
		c = Collectable_createPotion(this, Scene_positionForTileId(this, tileId), 25);
	}

	if (c) {
		Scene_addEntity(this, c->entity);
	}
}

// Lua

void Scene_luaExport(lua_State *l) {
	static struct luaL_Reg methods[] = {
		{"setBackground", Scene_luaSetBackground},
		{"addTrigger", Scene_luaAddTrigger},
		{"setTile", Scene_luaSetTile},
		{"setSceneAbove", Scene_luaSetSceneAbove},
		{"setColorPrefix", Scene_luaSetColorPrefix},
		{"setMirrorTiles", Scene_luaSetMirrorTile},
		{"spawnCollectable", Scene_luaSpawnCollectable},
		//{"__gc", Scene_luaDestroy},
		{NULL, NULL}
	};

	static struct luaL_Reg functions[] = {
		{"create", Scene_luaCreate},
		{NULL, NULL}
	};

	lua_createLib(l, SCENE_MTABLE, "Scene", functions, methods, Scene_luaDestroy);
}

Scene* Scene_checkfromLua (lua_State *L, int idx) {
	void *userData = luaL_checkudata(L, idx, SCENE_MTABLE);
	luaL_argcheck(L, (userData != NULL), idx, "`Scene' expected");
	return (Scene*) userData;
}

int Scene_luaSetBackground(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	const char* background = luaL_checkstring(l, 2);
	if (!this->background) {
		Sprite_destroy(this->background);
	}
	this->background = Sprite_create(TextureCache_get(this->engine->textureCache, background));
	lua_pop(l,2);
	return 0;
}

int Scene_luaAddTrigger(lua_State* l) {
	stackDump(l);
	Scene* this = Scene_checkfromLua(l, 1);
	Trigger* trigger = Trigger_checkFromLua(l, 2);
	Trigger_setScene(trigger, this);
	Vector_InsertInFirstFreeSpace(this->triggers, trigger);
	return 0;
}

int Scene_luaSetTile(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	const int tileId = luaL_checkint(l, 2);
	const int tileType = luaL_checkint(l, 3);
	Scene_setTile(this, tileId-1, tileType);
	lua_pop(l,3);
	return 0;
}

int Scene_luaSetSceneAbove(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	const char* sceneAbove = luaL_checkstring(l, 2);
	this->sceneAbove = sceneAbove;
	lua_pop(l,2);
	return 0;
}

int Scene_luaSetColorPrefix(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	const char* colorPrefix = luaL_checkstring(l, 2);
	this->colorPrefix = colorPrefix;
	lua_pop(l,2);
	return 0;
}

int Scene_luaSetMirrorTile(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	const int mirror= luaL_checknumber(l, 2);
	this->mirrorTiles = mirror == 1;
	lua_pop(l,2);
	return 0;
}

int Scene_luaSpawnCollectable(lua_State* l) {
	Scene* this = Scene_checkfromLua(l, 1);
	const int tileId = luaL_checkint(l, 2);
	const int collectableType = luaL_checkint(l, 3);
	Scene_spawnCollectable(this, tileId-1, collectableType);
	lua_pop(l,3);
	return 0;
}
