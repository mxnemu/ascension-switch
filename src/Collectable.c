#include "Collectable.h"
#include "TextureCache.h"
#include "Scene.h"
#include "Player.h"


Collectable* Collectable_create(Scene* scene, AnimatedSprite* sprite, void (*onCollect)(Collectable*, Player*)) {
	Collectable* this = malloc(sizeof(Collectable));
	this->entity = Entity_create(this, scene, sprite);
	this->entity->draw = Collectable_draw;
	this->entity->physics.belongsToGroups = COLLISION_GROUP_COLLECTABLE;
	this->entity->physics.collidesWithGroupMask = COLLISION_GROUP_TERRAIN;
	this->entity->destroy = Collectable_destroy;
	this->entity->onCollision = Collectable_onCollision;
	this->onCollect = onCollect;
	this->value = 0;
	return this;
}

void Collectable_destroy(void* context) {
	Collectable* this = context;
	free(this);
}

void Collectable_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	Collectable* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}

bool Collectable_onCollision(void* context, Entity* other) {
	Collectable* this = context;
	if (other->physics.belongsToGroups & COLLISION_GROUP_PLAYER) {
		ControlledEntity* controlled = other->context;
		this->onCollect(this, controlled->player);
		this->entity->destroyed = true;
	}
	return false;
}

Collectable* Collectable_createCoin(Scene* scene, SDL_Point pos, int value) {
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(scene->engine->textureCache, "images/coin.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0, 0, 32, 32, 65));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 65));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 65));
	List_pushBack(idleAnimation->frames, Frame_create(96,0, 32, 32, 65));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 65));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 65));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	Collectable* this = Collectable_create(scene, sprite, Collectable_onCollectCoin);
	this->value = 5;
	this->entity->physics.bounds.x = pos.x * TILE_W * PHYSICS_SCALE;
	this->entity->physics.bounds.y = pos.y * TILE_H * PHYSICS_SCALE;
	return this;
}

Collectable* Collectable_createKey(Scene* scene, SDL_Point pos, int value) {
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(scene->engine->textureCache, "images/key.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0, 0, 32, 32, 2400));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 100));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 120));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 100));
	List_pushBack(idleAnimation->frames, Frame_create(0, 0, 32, 32, 2500));
	List_pushBack(idleAnimation->frames, Frame_create(96, 0, 32, 32, 100));
	List_pushBack(idleAnimation->frames, Frame_create(128, 0, 32, 32, 120));
	List_pushBack(idleAnimation->frames, Frame_create(96, 0, 32, 32, 100));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	Collectable* this = Collectable_create(scene, sprite, Collectable_onCollectKey);
	this->value = value;
	this->entity->physics.bounds.x = pos.x * TILE_W * PHYSICS_SCALE;
	this->entity->physics.bounds.y = pos.y * TILE_H * PHYSICS_SCALE;
	return this;
}

Collectable* Collectable_createPotion(Scene* scene, SDL_Point pos, int value) {
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(scene->engine->textureCache, "images/potion.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0, 0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 200));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	Collectable* this = Collectable_create(scene, sprite, Collectable_onCollectPotion);
	this->value = value;
	this->entity->physics.bounds.x = pos.x * TILE_W * PHYSICS_SCALE;
	this->entity->physics.bounds.y = pos.y * TILE_H * PHYSICS_SCALE;
	return this;
}


void Collectable_onCollectCoin(Collectable* this, Player* player) {
	Player_earnMoney(player, this->value);
}

void Collectable_onCollectKey(Collectable* this, Player* player) {
	Scene_openDoors(player->entity->scene, this->value);
}

void Collectable_onCollectPotion(Collectable* this, Player* player) {
	Entity_heal(player->entity, this->value);
}

