#pragma once
#include "Constants.h"
#include "Input.h"
#include "Vector.h"

typedef struct Combo {
	const char* name;
	ActionId action; // id of the key being pressed
	Vector* hitboxes;
	Vector* followups; // next combos
	RawTime timeUntilReady; // action will not be accepted before this time passed
	RawTime timeUntilCancel; // reset the combo tree when this time passed
	bool cancelOnWrongAction; // cancel if unrelated action button is pressed

	int minSpeedX;
	int minSpeedY;
	int maxSpeedX;
	int maxSpeedY;
} Combo;

Combo* Combo_create();
void Combo_destroy(Combo* this);
