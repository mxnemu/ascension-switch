#pragma once

#include "Constants.h"
#include "Entity.h"
#include "Vector.h"

typedef enum CollisionState {
	started, presolve, solved, canceled
}CollisionState;

typedef struct Scene Scene;
CLASS(Collision) {
	Entity* a;
	Entity* b;
	CollisionState state;
	Scene* s;
};

CLASS(CollisionHandler) {
	Vector* collisions;
};

CollisionHandler* CollisionHandler_create();
void CollisionHandler_destroy(CollisionHandler* this);

void CollisionHandler_update(CollisionHandler* this, Vector* entities);
void CollisionHandler_handle(CollisionHandler* this, Entity* a, Entity* b);
void CollisionHandler_removeCollisionsWithEntity(CollisionHandler* this, Entity* entity);
void CollisionHandler_removeCollisionByIndex(CollisionHandler* this, int index);

Collision* Collision_create(Entity* a, Entity* b);
void Collision_destroy(Collision*);

bool Collision_matches(Collision* this, Entity* a,Entity* b);
bool Collision_resolve(Collision* this);
