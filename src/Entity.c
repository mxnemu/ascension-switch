#include "Entity.h"
#include "Hitbox.h"
#include "Utils.h"

Entity* Entity_create(void* context, Sprite* sprite) {
	Entity* this = malloc(sizeof(Entity));
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
	this->dx = 0;
	this->dy = 0;
	this->groupMask = INT_MAX;
	this->groups = COLLISION_GROUP_DEFAULT;
}

void EntityPhysics_destroy(EntityPhysics* this) {
	free(this);
}

void Entity_emptyDraw(void* context, SDL_Surface* surface, Camera* camera) {}
