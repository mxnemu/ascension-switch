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
	Sprite* background;
	Vector* entities;
	Vector* triggers;
	Vector* tiles;
	Camera* camera;
	CollisionHandler* collisionHandler;
	GameEngine* engine;
	int gravity;
	const char* sceneAbove;
	const char* colorPrefix;
	bool mirrorTiles;
};

Scene* Scene_create(GameEngine* engine, const char* filePath, Scene* lastScene);
int Scene_luaDestroy(lua_State* l);

void Scene_update(Scene* this, RawTime dt);
void Scene_draw(Scene* this, SDL_Renderer* renderer);

void Scene_addBackground(Scene* this, const char* background);
void Scene_addEntity(Scene* this, Entity* entity);

void Scene_setBounds(Scene* this, int x, int y, int w, int h);

SDL_Point Scene_positionForTileId(Scene* this, const int tileId);

void Scene_openDoors(Scene* this, int type);
void Scene_spawnCollectable(Scene* this, int tileId, int type);

// Lua stuff
Scene* Scene_checkfromLua (lua_State *L, int idx);

void Scene_luaExport(lua_State *l);
int Scene_luaCreate(lua_State* l);
int Scene_luaSetBackground(lua_State *l);
void Scene_luaAddEntity(lua_State *l);
void Scene_luaPopFrontBackgroundAndTranslateBack(lua_State);
int Scene_luaAddTrigger(lua_State* l);
int Scene_luaSetTile(lua_State* l);
int Scene_luaSetSceneAbove(lua_State* l);
int Scene_luaSetColorPrefix(lua_State* l);
int Scene_luaSetMirrorTile(lua_State* l);
int Scene_luaSpawnCollectable(lua_State* l);
