#pragma once

#include "Constants.h"
#include "Entity.h"

typedef enum CollisionState {
	started, presolve, solved, canceled
}CollisionState;

CLASS(Collision) {
	Entity* a, b;
	CollisionState state;
};

CLASS(CollisionHandler) {
	Vector collisions;
};

CollisionHandler* CollisionHandler_create();
void CollisionHandler_destroy();

void CollisionHandler_update(Vector* entities);
