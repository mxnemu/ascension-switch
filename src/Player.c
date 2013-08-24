#include "Player.h"

Player* Player_create(Scene* scene, AnimatedSprite* sprite, Input* input) {
	Player* this = malloc(sizeof(Player));
	this->entity = Entity_create(this, scene, sprite);
	this->entity->draw = Player_draw;
	this->entity->update = Player_update;
	this->input = input;
	this->remainingJumps = 10;
	this->timeSinceGrounded = 0;

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

	if (this->entity->physics.groundedStatus == grounded) {
		this->timeSinceGrounded += dt;
	} else {
		this->timeSinceGrounded = 0;
	}
}

void Player_processInput(Player* this) {
	int x = Input_getAxis(this->input, horizontal);
	if (x != 0) {
		this->entity->physics.dx += (x*this->entity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
		if (x < 0) {
			this->entity->animatedSprite->sprite->flip = true;
		} else  if (x > 0) {
			this->entity->animatedSprite->sprite->flip = false;
		}
	}

	int y = Input_getAxis(this->input, vertical);
	if (y != 0 && this->entity->inFrontOfLadder) {
		this->entity->physics.groundedStatus = onLadder;
		this->entity->physics.dy += (y*this->entity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
	}

	if (Input_isDown(this->input, attackSword)) {
		// TODO attack
	}

	if (Input_isDown(this->input, jump)) {
		if (this->entity->physics.groundedStatus == grounded && this->timeSinceGrounded > 120) {
			this->remainingJumps = 10;
		}
		if (this->remainingJumps > 0) {
			this->entity->physics.dy -= (32*10) / PHYSICS_SCALE;
			this->remainingJumps--;
		}
	}
}

void Player_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}
