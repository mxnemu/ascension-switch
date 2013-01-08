#include "Entity.h"
#include "Hitbox.h"
#include "Utils.h"
#include "Scene.h"

Entity* Entity_create(void* context, Scene* scene, Sprite* sprite) {
	Entity* this = malloc(sizeof(Entity));
	this->scene = scene;
	this->sprite = sprite;
	this->destroyed = false;
	EntityPhysics_init(&this->physics);
	this->update = emptyUpdate;
	this->draw = Entity_emptyDraw;
	this->destroy = emptyDestroy;
	this->context = context;
	this->hitboxes = List_create();

	return this;
}

void Entity_destroy(Entity* this) {
	this->destroy(this->context);
	ListNode* it = this->hitboxes->first;
	while(it) {
		Hitbox_destroy((Hitbox*)it->data);
	}
	List_destroy(this->hitboxes);
	free(this);
}

bool Entity_collides(Entity* this, Entity* other) {
	if (SDL_Rect_touches(&this->physics.bounds, &other->physics.bounds)) {
		bool hitboxCollision = false;
		ListNode* it = this->hitboxes->first;
		while (it) {
			ListNode* jt = other->hitboxes->first;
			while (jt) {
				Hitbox* a = it->data;
				Hitbox* b = jt->data;
				if (Hitbox_collides(a, b)) {
					hitboxCollision = true; // TODO apply attacks / blocks and stuff
				}
			}
		}
		return hitboxCollision;
	}
	return false;
}


void EntityPhysics_init(EntityPhysics* this) {
	SDL_Rect_init(&this->bounds);
	this->dx = 0;
	this->dy = 0;
	this->groupMask = INT_MAX;
	this->groups = COLLISION_GROUP_DEFAULT;
}

void Entity_update(Entity* this, RawTime dt) {
	this->update(this->context, dt);
	SDL_Rect newPosition = {
		.x = this->physics.bounds.x + this->physics.dx,
		.y = this->physics.bounds.y + this->physics.dy,
		.w = this->physics.bounds.w,
		.h = this->physics.bounds.h
	};

	if (!Entity_wouldCollide(this, &newPosition)) {
		this->physics.bounds = newPosition;
		this->physics.dx = this->physics.dy = 0;
	}
	if (this->sprite) {
		this->sprite->bounds.x = this->physics.bounds.x / PHYSICS_SCALE;
		this->sprite->bounds.y = this->physics.bounds.y / PHYSICS_SCALE;
	}
}

bool Entity_wouldCollide(Entity* this, SDL_Rect *rect) {
	// test entities
	for (int i=0; i < this->scene->entities->usedElements; ++i) {
		Entity* it = this->scene->entities->elements[i];
		if (it != NULL) {

			for (int j=0; j < this->scene->entities->usedElements; ++j) {
				Entity* jt = this->scene->entities->elements[j];
				if (jt != NULL && jt != it && Entity_collides(it, jt)) {
					return true;
				}
			}
		}
	}
	return false;
}

void EntityPhysics_destroy(EntityPhysics* this) {
	free(this);
}

void Entity_emptyDraw(void* context, SDL_Surface* surface, Camera* camera) {}
