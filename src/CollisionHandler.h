#pragma once

#include "Constants.h"
#include "Entity.h"
#include "Vector.h"

typedef enum CollisionState {
	started, presolve, solved, canceled
}CollisionState;

CLASS(Collision) {
	struct Entity* a;
	struct Entity* b;
	CollisionState state;
	struct Scene* s;
};

CLASS(CollisionHandler) {
	Vector* collisions;
};

CollisionHandler* CollisionHandler_create();
void CollisionHandler_destroy(CollisionHandler* this);

void CollisionHandler_update(CollisionHandler* this, Vector* entities);
void CollisionHandler_handle(CollisionHandler* this, struct Entity* a, struct Entity* b);
void CollisionHandler_removeCollisionsWithEntity(CollisionHandler* this, struct Entity* entity);
void CollisionHandler_removeCollisionByIndex(CollisionHandler* this, int index);

Collision* Collision_create(struct Entity* a, struct Entity* b);
void Collision_destroy(Collision*);

bool Collision_matches(Collision* this, struct Entity* a, struct Entity* b);
bool Collision_resolve(Collision* this);
