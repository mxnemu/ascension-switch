#pragma once

#include "Scene.h"

#define TEAM_NONE 0
#define TEAM_PLAYER 1
#define TEAM_EVILPLAYER 2
#define TEAM_ENEMY 3

CLASS(Entity) {
	Sprite* sprite;
	bool destroyed;
	int dx, dy;
	void (*destroy)(void* context);
	void (*update)(void* context, RawTime dt);
	void (*draw)(void* context, SDL_Surface*);
	Scene* scene;
	int team;
	void* context;
	List* hitboxes;
};

Entity* Entity_create(Scene* scene, void* context, Sprite* sprite);
void Entity_destroy(Entity* this);

