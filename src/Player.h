#pragma once
#include "Constants.h"
#include "Entity.h"
#include "Input.h"

CLASS(Player) {
	Input* input;
	Entity* entity;
	int money;
};

Player* Player_create(Scene* scene, AnimatedSprite* sprite, Input* input);
void Player_destroy(void* context);
void Player_update(void* context, RawTime dt);
void Player_draw(void* context, SDL_Renderer* renderer, Camera* camera);

void Player_onEntityDestroyed(void* context);

void Player_processInput(Player* this);
void Player_earnMoney(Player* this, int money);
