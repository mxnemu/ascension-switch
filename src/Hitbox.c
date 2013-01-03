
#include "Hitbox.h"

Hitbox* Hitbox_create() {
	Hitbox* this = malloc(sizeof(Hitbox));
	this->xForce = 0;
	this->yForce = 0;
	this->isCircular = 0;
	this->state = passive;
	return this;
}

void Hitbox_destroy(Hitbox* this) {
	free(this);
}
