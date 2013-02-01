#pragma once
#include "Constants.h"
#include "Sprite.h"
#include "List.h"

CLASS(Frame) {
	SDL_Rect rect;
	RawTime timeToNextFrame;
};

CLASS(Animation) {
	const char* name;
	List* frames;
};

typedef struct AnimatedSprite AnimatedSprite;
CLASS(AnimationProgress) {
	AnimatedSprite* owner;
	Animation* animation;
	Frame* frame;
	ListNode* frameNode;
	RawTime progress;
};

struct AnimatedSprite {
	Sprite* sprite;
	List* animations;
	AnimationProgress progress;
};

AnimatedSprite* AnimatedSprite_create(Sprite* sprite);
void AnimatedSprite_destroy(AnimatedSprite* this);
void AnimatedSprite_setFrame(AnimatedSprite* this, SDL_Rect frame);

void AnimationProgress_init(AnimationProgress* this, AnimatedSprite* owner, Animation* animation);
void AnimationProgress_resetFrameLoop(AnimationProgress* this);
void AnimationProgress_update(AnimationProgress* this, RawTime dt);

Animation* Animation_create(const char* name);
void Animation_destroy(Animation* this);

Frame* Frame_create(int x, int y, int w, int h, int timeToNextFrame);
void Frame_destroy(Frame* this);
