#pragma once
#include "Constants.h"
#include "Camera.h"
#include "List.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Vector.h"

CLASS(Scene) {
	const char* luaFile;
	Sprite* leftBackground;
	Sprite* rightBackground;
	List* backgrounds;
	Vector* entities;
	Camera* camera;
	GameEngine* engine;
};

Scene* Scene_create(GameEngine* engine, const char* luafile);
void Scene_init(Scene* this);
void Scene_destroy(Scene* this);

void Scene_update(Scene* this, RawTime dt);
void Scene_draw(Scene* this, SDL_Surface* screen);

void Scene_addBackground(Scene* this, const char* background);


// Lua stuff
int Scene_luaAddBackground(lua_State *l);
