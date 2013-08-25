#include "Healthbar.h"

Healthbar* Healthbar_create(TextureCache* tc) {
	Healthbar* this = malloc(sizeof(Healthbar));
	this->background = Sprite_create(TextureCache_get(tc, "images/healthbarBg.png"));


	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(tc, "images/healthbar.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0, 0, 32, 128, 3000));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 128, 3000));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 128, 3000));
	List_pushBack(idleAnimation->frames, Frame_create(96,0, 32, 128, 3000));
	List_pushBack(idleAnimation->frames, Frame_create(128,0, 32, 128, 3000));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;
	this->liquid = sprite;
	this->fullHeight = this->liquid->sprite->bounds.h;
	Healthbar_set(this, 100, 100);
	return this;
}

void Healthbar_destroy(Healthbar* this) {
	Sprite_destroy(this->background);
	AnimatedSprite_destroy(this->liquid);
	free(this);
}
void Healthbar_draw(Healthbar* this, SDL_Renderer* renderer) {
	Sprite_draw(this->background, renderer);
	this->liquid->sprite->bounds.h = this->transitionTarget;
	this->liquid->sprite->bounds.y = this->background->bounds.y + this->background->bounds.h - this->liquid->sprite->bounds.h;
	Sprite_draw(this->liquid->sprite, renderer);
}

void Healthbar_set(Healthbar* this, const float health, const float maxHealth) {
	this->transitionTarget = ((float)this->fullHeight) * (health / maxHealth);
	this->transitionTarget = MAX(this->transitionTarget, 0);
	//this->transitionStep;
}

void Healthbar_update(Healthbar* this, RawTime dt) {
	AnimationProgress_update(&this->liquid->progress, dt);
//	this->liquid->
}
