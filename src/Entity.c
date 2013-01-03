#include "Entity.h"
#include "Hitbox.h"

Entity* Entity_create(Scene* scene, void* context, Sprite* sprite) {
	Entity* this = malloc(sizeof(Entity));
	this->sprite = sprite;
	this->destroyed = false;
	this->dx = this->dy = 0;
	this->update = emptyUpdate;
	this->draw = emptyDraw;
	this->scene = scene;
	this->team = TEAM_NONE;
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
