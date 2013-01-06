#include "Player.h"

Player* Player_create(Sprite* sprite) {
	Player* this = malloc(sizeof(Player));
	this->entity = Entity_create(this, sprite);
	this->entity->draw = Player_draw;
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
//	if (Input_keyDown(INPUT_P1_LEFT)) {
//		this->entity->sprite->bounds.x -= 1;
//	}
//	if (Input_keyDown(INPUT_P1_RIGHT)) {
//		this->entity->sprite->bounds.x += 1;
//	}
//	if (Input_keyDown(INPUT_P1_UP)) {
//		this->entity->sprite->bounds.y += 1;
//	}
//	if (Input_keyDown(INPUT_P1_DOWN)) {
//		this->entity->sprite->bounds.y -= 1;
//	}
}

void Player_draw(void* context, SDL_Surface* screen, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->sprite, screen, camera);
}
