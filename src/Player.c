#include "Player.h"

Player* Player_create(Scene* scene, AnimatedSprite* sprite, Input* input) {
	Player* this = malloc(sizeof(Player));
	this->entity = Entity_create(this, scene, sprite);
	this->entity->draw = Player_draw;
	this->entity->update = Player_update;
	this->input = input;

	return this;
}

void Player_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	Player* this = context;
	free(this);
}

void Player_update(void* context, RawTime dt) {
	Player* this = context;
	Player_processInput(this);
}

void Player_processInput(Player* this) {
	int x = Input_getAxis(this->input, horizontal);
	int y = Input_getAxis(this->input, vertical);
	if (x != 0 || 0 != y) {
		this->entity->physics.dx += (x*5 * PHYSICS_SCALE) / AXIS_MAX;
		this->entity->physics.dy += (y*5 * PHYSICS_SCALE) / AXIS_MAX;
		if (x < 0) {
			this->entity->animatedSprite->sprite->flip = true;
		} else  if (x > 0) {
			this->entity->animatedSprite->sprite->flip = false;
		}
	}

	if (Input_isDown(this->input, attackSword)) {
		// TODO attack
	}
}

void Player_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}
