#include "CollisionHandler.h"

void CollisionHandler_update(CollisionHandler* this, Vector* entities) {
	for (int i=0; i < entities->allocatedElements; ++i) {
		Entity* it = entities->elements[i];
		if (it != NULL) {

			for (int j=0; j < entities->allocatedElements; ++j) {
				Entity* jt = entities->elements[j];
				if (jt != NULL && jt != it && Entity_collides(it, jt)) {
					CollisionHandler_handle(this, it, jt);
				}
			}
		}
	}
}

void CollisionHandler_handle(CollisionHandler* this, Entity* a, Entity* b) {
	for (int i=0; i < this->collisions->allocatedElements; ++i) {
		Collision* it = this->collisions->elements[i];
		if (it != NULL && Collison_matches(it, a, b)) {
			Collision_handle(it);
		}
	}
}

void CollisionHandler_removeCollisionsWithEntity(CollisionHandler* this, Entity* entity) {
	for (int i=0; i < this->collisions->allocatedElements; ++i) {
		Collision* it = this->collisions->elements[i];
		if (it != NULL) {
			if (it->a == entity) {
				CollisionHandler_removeCollisionByIndex(this, i);
			} else if (it->b == entity) {
				CollisionHandler_removeCollisionByIndex(this, i);
			}
			Collision_handle(it);
		}
	}
}

CollisionHandler_removeCollisionByIndex(this, i) {
	Collision* c = Vector_Get(this->collisions, index);
	Collision_destroy(c);
	Vector_Set(this->collisions, index, NULL);
}

bool Collision_matches(Collision* this, Entity* a,Entity* b) {
	return (this->a == a && this->b == b) || (this->b == a && this->b == a);
}
