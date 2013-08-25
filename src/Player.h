#pragma once
#include "Constants.h"
#include "Entity.h"
#include "Input.h"

CLASS(Player) {
	Input* input;
	Entity* entity;
	Entity* controlledEntity;
	Scene* scene;
	int money;
};

Player* Player_create(Scene* scene, Input* input);
void Player_destroy(void* context);
void Player_update(void* context, RawTime dt);
void Player_draw(void* context, SDL_Renderer* renderer, Camera* camera);
void Player_spawnPlayerEntity(Player* this);

void Player_onEntityDestroyed(void* context);

void Player_processInput(Player* this);
void Player_earnMoney(Player* this, int money);
