#include "Combo.h"
#include "Utils.h"
#include "Hitbox.h"

Combo* Combo_create() {
	Combo* this = malloc(sizeof(Combo));
	this->name = "";
	this->action = none;
	this->followups = Vector_Create();
	this->hitboxes = Vector_Create();
	this->timeUntilReady = 0;
	this->timeUntilCancel = 2000;
	this->cancelOnWrongAction = false;

	this->minSpeedX = 0;
	this->maxSpeedX = 0;
	this->minSpeedY = 0;
	this->maxSpeedY = 0;

	return this;
}

void Combo_destroy(Combo* this) {
	FREE_VECTOR_WITH_ELMENTS(this->followups, Combo_destroy);
	FREE_VECTOR_WITH_ELMENTS(this->hitboxes, Hitbox_destroy);
	free(this);
}
