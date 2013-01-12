#include "Combo.h"
#include "Utils.h"

Combo* Combo_create() {
	Combo* this = malloc(sizeof(Combo));
	this->name = "";
	this->action = none;
	this->followups = Vector_Create();
	this->timeUntilReady = 0;
	this->timeUntilCancel = 2000;
	this->cancelOnWrongAction = true;
	return this;
}

void Combo_destroy(Combo* this) {
	FREE_VECTOR_WITH_ELMENTS(this->followups, Combo_destroy);
	free(this);
}
