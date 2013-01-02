#include "Entity.h"

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

	return this;
}

void Entity_destroy(Entity* this) {
	this->destroy(this->context);
	free(this);
}
