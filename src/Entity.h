#pragma once

#include "Constants.h"
#include "Sprite.h"
#include "List.h"

#define COLLISION_GROUP_DEFAULT 1
#define COLLISION_GROUP_PLAYER 1 << 2

CLASS(EntityPhysics) {
	int dx, dy;
	SDL_Rect bounds;
	int weight;

	int groups;
	int groupMask;
};

CLASS(Entity) {
	Sprite* sprite;
	bool destroyed;
	EntityPhysics physics;

	void (*destroy)(void* context);
	void (*update)(void* context, RawTime dt);
	void (*draw)(void* context, SDL_Surface*);
	void* context;
	List* hitboxes;
};

Entity* Entity_create(void* context, Sprite* sprite);
void Entity_destroy(Entity* this);

bool Entity_collides(Entity* this, Entity* other);

void EntityPhysics_init(EntityPhysics* this);
