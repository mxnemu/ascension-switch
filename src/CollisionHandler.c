#include "CollisionHandler.h"

void CollisionHandler_update(CollisionHandler* this, Vector* entities) {
	for (int i=0; i < entities->usedElements; ++i) {
		Entity* it = entities->elements[i];
		if (it != NULL) {

			for (int j=0; j < entities->usedElements; ++j) {
				Entity* jt = entities->elements[j];
				if (jt != NULL && jt != it && Entity_collides(it, jt)) {
					CollisionHandler_handle(this, it, jt);
				}
			}
		}
	}
}

void CollisionHandler_handle(CollisionHandler* this, Entity* a, Entity* b) {
	for (int i=0; i < this->collisions->usedElements; ++i) {
		Collision* it = this->collisions->elements[i];
		if (it != NULL && Collision_matches(it, a, b)) {
			Collision_resolve(it); // TODO check if it has been resolved and remove it;
			return;
		}
	}
	Collision* newCollision = Collision_create(a,b);
	if (!Collision_resolve(newCollision)) {
		Vector_InsertInFirstFreeSpace(this->collisions, newCollision);
	}

}

void CollisionHandler_removeCollisionsWithEntity(CollisionHandler* this, Entity* entity) {
	for (int i=0; i < this->collisions->usedElements; ++i) {
		Collision* it = this->collisions->elements[i];
		if (it != NULL) {
			if (it->a == entity) {
				CollisionHandler_removeCollisionByIndex(this, i);
			} else if (it->b == entity) {
				CollisionHandler_removeCollisionByIndex(this, i);
			}
			Collision_resolve(it);
		}
	}
}

void CollisionHandler_removeCollisionByIndex(CollisionHandler* this, int index) {
	Collision* c = Vector_Get(this->collisions, index);
	Collision_destroy(c);
	Vector_Set(this->collisions, index, NULL);
}

Collision* Collision_create(Entity* a, Entity* b) {
	Collision* this = malloc(sizeof(Collision));
	return this;
}

void Collision_destroy(Collision* this) {
	free(this);
}

bool Collision_resolve(Collision* this) {
	static const int repulsion = 1;
	const bool aIsleft = this->a->physics.bounds.x < this->b->physics.bounds.x;
//	const bool aIstop  = this->a->sprite->rect.y < this->b->sprite->rect.y;

	if (aIsleft) {
		this->a->physics.dx -= repulsion;
		this->b->physics.dx += repulsion;
	} else {
		this->a->physics.dx += repulsion;
		this->b->physics.dx += repulsion;
	}

	return false;
}

bool Collision_matches(Collision* this, Entity* a,Entity* b) {
	return (this->a == a && this->b == b) || (this->b == a && this->b == a);
}
