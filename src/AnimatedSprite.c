#include "AnimatedSprite.h"

AnimatedSprite* AnimatedSprite_create(Sprite* sprite) {
	AnimatedSprite* this = malloc(sizeof(AnimatedSprite));
	this->sprite = sprite;
	this->animations = List_create();
	AnimationProgress_init(&this->progress, this, NULL);
	return this;
}
void AnimatedSprite_destroy(AnimatedSprite* this) {
	Sprite_destroy(this->sprite);
}

void AnimatedSprite_setFrame(AnimatedSprite* this, SDL_Rect frame) {
	this->sprite->frame = frame;
	Sprite_setFrameSize(this->sprite, frame.w, frame.h);
}

void AnimationProgress_init(AnimationProgress* this, AnimatedSprite* owner, Animation* animation) {
	this->owner = owner;
	this->animation = animation;
	this->progress = 0;
	this->frameNode = animation ? animation->frames->first : NULL;
	this->frame = this->frameNode ? this->frameNode->data : NULL;
}

void AnimationProgress_update(AnimationProgress* this, RawTime dt) {
	CANCEL_IF_FALSE(this->animation);
	if (!this->frameNode) {
		AnimationProgress_resetFrameLoop(this);
	}
	this->progress += dt;

	if (this->progress >= this->frame->timeToNextFrame) {
		if (this->frameNode->next) {
			this->frameNode = this->frameNode->next;
			this->frame = this->frameNode->data;
			AnimatedSprite_setFrame(this->owner, this->frame->rect);
			this->progress = 0;
		} else {
			AnimationProgress_resetFrameLoop(this);
		}
	}
}

void AnimationProgress_resetFrameLoop(AnimationProgress* this) {
	this->frameNode = this->animation->frames->first;
	CANCEL_IF_FALSE(this->frameNode);
	this->frame = this->frameNode->data;
	this->progress = 0;
}

Animation* Animation_create(const char* name) {
	Animation* this = malloc(sizeof(Animation));
	this->frames = List_create();
	return this;
}

void Animation_destroy(Animation* this) {
	free(this);
}

Frame* Frame_create(int x, int y, int w, int h, int timeToNextFrame) {
	Frame* this = malloc(sizeof(Frame));
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	this->timeToNextFrame = timeToNextFrame;
	return this;
}

void Frame_destroy(Frame* this) {
	free(this);
}
