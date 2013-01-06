#include "Player.h"

Player* Player_create(Sprite* sprite, Input* input) {
	Player* this = malloc(sizeof(Player));
	this->entity = Entity_create(this, sprite);
	this->entity->draw = Player_draw;
	this->entity->update = Player_update;
	this->input = input;
	return this;
}

void Player_destroy(void* context) {
	Player* this = context;
	free(this);
}

void Player_update(void* context, RawTime dt) {
	Player* this = context;
	Player_processInput(this);
}

// TODO proper movement
void Player_processInput(Player* this) {
	if (Input_getAxis(this->input, left) != 0) {
		this->entity->sprite->bounds.x -= 1;
	}
	if (Input_getAxis(this->input, right) != 0) {
		this->entity->sprite->bounds.x += 1;
	}
	if (Input_getAxis(this->input, up) != 0) {
		this->entity->sprite->bounds.y -= 1;
	}
	if (Input_getAxis(this->input, down) != 0) {
		this->entity->sprite->bounds.y += 1;
	}
}

void Player_draw(void* context, SDL_Surface* screen, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->sprite, screen, camera);
}
