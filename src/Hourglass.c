#include "Hourglass.h"


Hourglass* Hourglass_create(GameEngine* engine) {
	Hourglass* this = malloc(sizeof(Hourglass));
	this->sprite = Sprite_create(TextureCache_get(engine->textureCache, "images/hourglass.png"));
	this->physicalAngle = 0;
	this->angleTarget = 0;
	this->angleDeltaStep = 0;
	this->timeUntilNextRound = 12;
	this->data = NULL;
	this->onSpin = NULL;
	return this;
}

void Hourglass_destroy(Hourglass* this) {
	Sprite_destroy(this->sprite);
	free(this);
}

void Hourglass_draw(Hourglass* this, SDL_Renderer* renderer) {
	Sprite_draw(this->sprite, renderer);
}

void Hourglass_update(Hourglass* this, RawTime dt) {
	this->timeUntilNextRound -= dt;
	if (this->timeUntilNextRound <= 0) {
		this->timeUntilNextRound = ROUND_TIME + TRANSITION_TIME;

		if (this->angleTarget >= 180) {
			this->angleTarget = 0;
		} else {
			this->angleTarget = 180;
		}
		this->angleDeltaStep = this->angleTarget - this->sprite->angle;
		this->physicalAngle = this->sprite->angle;
	}
	if (this->sprite->angle != this->angleTarget) {
		float step = ((float)this->angleDeltaStep) * ((float)dt / TRANSITION_TIME);
		this->physicalAngle += step;
		this->sprite->angle = this->physicalAngle;

		if (abs(this->sprite->angle - this->angleTarget) <= abs(step)) {
			this->sprite->angle = this->angleTarget;
			while (this->sprite->angle >= 360) {
				this->sprite->angle -= 360;
			}
			while (this->sprite->angle < 0) {
				this->sprite->angle += 360;
			}
			this->angleTarget = this->sprite->angle;
			if (this->onSpin) {
				this->onSpin(this->data);
			}
		}
	}
}
