#pragma once
#include "Constants.h"
#include "Camera.h"
#include "List.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Vector.h"
#include "CollisionHandler.h"

#define SCENE_MTABLE "yotm.Scene"

CLASS(Scene) {
	SDL_Rect walkableBounds;
	Sprite* leftBackground;
	Sprite* rightBackground;
	List* backgrounds;
	Vector* entities;
	Vector* triggers;
	Camera* camera;
	CollisionHandler* collisionHandler;
	GameEngine* engine;
};

Scene* Scene_create(GameEngine* engine, const char* filePath);
int Scene_luaDestroy(lua_State* l);

void Scene_update(Scene* this, RawTime dt);
void Scene_draw(Scene* this, SDL_Surface* screen);

void Scene_addBackground(Scene* this, const char* background);
void Scene_addEntity(Scene* this, Entity* entity);

// Lua stuff
Scene* Scene_checkfromLua (lua_State *L, int idx);

void Scene_luaExport(lua_State *l);
int Scene_luaCreate(lua_State* l);
int Scene_luaAddBackground(lua_State *l);
void Scene_luaAddEntity(lua_State *l);
void Scene_luaPopFrontBackgroundAndTranslateBack(lua_State);
int Scene_luaAddTrigger(lua_State* l);
