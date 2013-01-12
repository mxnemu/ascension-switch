#pragma once

#include "Constants.h"
#include "Sprite.h"
#include "List.h"
#include "Camera.h"
#include "Combo.h"

#define COLLISION_GROUP_DEFAULT 1
#define COLLISION_GROUP_PLAYER 1 << 2

#define PHYSICS_SCALE 6

typedef struct Scene Scene;
CLASS(EntityPhysics) {
	Scene* scene;
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
	void (*draw)(void* context, SDL_Surface*, Camera* camera);
	Scene* scene;
	void* context;
	List* hitboxes;

	Vector* combos;
	Combo* currentCombo;
	RawTime timeSinceLastComboAction;
};

Entity* Entity_create(void* context, Scene* scene, Sprite* sprite);
void Entity_destroy(Entity* this);

bool Entity_collides(Entity* this, Entity* other);
bool Entity_wouldCollide(Entity* this, SDL_Rect *rect);
void Entity_performComboAction(Entity* this, ActionId action);

void EntityPhysics_init(EntityPhysics* this, Sprite* sprite);
void Entity_emptyDraw(void* context, SDL_Surface* screen, Camera* camera);
