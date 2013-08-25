#pragma once

#include "Constants.h"
#include "Entity.h"

#define COLLECTABLE_COIN 1
#define COLLECTABLE_COIN_BIG 2
#define COLLECTABLE_KEY_4 4
#define COLLECTABLE_KEY_5 5
#define COLLECTABLE_KEY_6 6
#define COLLECTABLE_POTION 7

typedef struct Player Player;
CLASS(Collectable) {
	Entity* entity;
	void (*onCollect)(Collectable*, Player*);
	int value;
};

Collectable* Collectable_create(Scene* scene, AnimatedSprite* sprite, void (*onCollect)(Collectable*, Player*));
void Collectable_destroy(void* context);
void Collectable_draw(void* context, SDL_Renderer* renderer, Camera* camera);
bool Collectable_onCollision(void* context, Entity* other);

Collectable* Collectable_createCoin(Scene* scene, SDL_Point pos, int value);
Collectable* Collectable_createKey(Scene* scene, SDL_Point point, int value);
Collectable* Collectable_createPotion(Scene* scene, SDL_Point point, int value);

void Collectable_onCollectCoin(Collectable* this, Player* player);
void Collectable_onCollectKey(Collectable* this, Player* player);
void Collectable_onCollectPotion(Collectable* this, Player* player);
