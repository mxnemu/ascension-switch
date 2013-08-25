#include "AnimatedSprite.h"

AnimatedSprite* AnimatedSprite_create(Sprite* sprite) {
	AnimatedSprite* this = malloc(sizeof(AnimatedSprite));
	this->sprite = sprite;
	this->animations = List_create();
	AnimationProgress_init(&this->progress, this, NULL);
	this->context = NULL;
	this->onAnimationEnd = AnimatedSprite_emptyOnAnimationEnd;
	return this;
}
void AnimatedSprite_destroy(AnimatedSprite* this) {
	Sprite_destroy(this->sprite);
}

void AnimatedSprite_setFrame(AnimatedSprite* this, SDL_Rect frame) {
	this->sprite->frame = frame;
	Sprite_setFrameSize(this->sprite, frame.w, frame.h);
}

void AnimatedSprite_setAnimation(AnimatedSprite* this, const char* name) {
	ListNode* it = this->animations->first;
	while (it) {
		Animation* animation = it->data;
		if (animation && strcmp(name, animation->name) == 0) {
			this->progress.animation = animation;
			AnimationProgress_resetFrameLoop(&this->progress);
			return;
		}
		it = it->next;
	}
	printf("ERROR no animation named %s", name);
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
			if (this->animation->loop) {
				AnimationProgress_resetFrameLoop(this);
			} else {
				this->owner->onAnimationEnd(this->owner->context);
			}
		}
	}
}

void AnimationProgress_resetFrameLoop(AnimationProgress* this) {
	this->progress = 0;
	this->frameNode = this->animation->frames->first;
	CANCEL_IF_FALSE(this->frameNode);
	this->frame = this->frameNode->data;
	AnimatedSprite_setFrame(this->owner, this->frame->rect);
}

Animation* Animation_create(const char* name, bool loop) {
	Animation* this = malloc(sizeof(Animation));
	this->name = name;
	this->frames = List_create();
	this->loop = loop;
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

void AnimatedSprite_emptyOnAnimationEnd(void* context) {}
