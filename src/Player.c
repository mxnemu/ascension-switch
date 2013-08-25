#include "Player.h"
#include "AnimatedSprite.h"
#include "TextureCache.h"
#include "Scene.h"

Player* Player_create(Scene* scene, Input* input) {
	Player* this = malloc(sizeof(Player));
	this->input = input;
	this->scene = scene;
	this->money = 0;

	Player_spawnPlayerEntity(this);
	return this;
}

void Player_onEntityDestroyed(void* context) {
	Player* this = context;
	this->money = -1;
	this->entity = NULL;
}

void Player_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	Player* this = context;
	free(this);
}

void Player_update(void* context, RawTime dt) {
	Player* this = context;
	Player_processInput(this);
}

void Player_spawnPlayerEntity(Player* this) {
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(this->scene->engine->textureCache, "images/knightRed.png")));
	Animation* idleAnimation = Animation_create("idle");
	List_pushBack(idleAnimation->frames, Frame_create(0,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(sprite->animations, idleAnimation);
	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	sprite->sprite->flip = this->scene->mirrorTiles;

	Entity* entity = Entity_create(this, this->scene, sprite);
	entity->draw = Player_draw;
	entity->update = Player_update;
	entity->destroy = Player_onEntityDestroyed;

	SDL_Point tilePos = Scene_positionForTileId(this->scene, 1 + (SCENE_TILES_X * (SCENE_TILES_Y-2)) );
	entity->physics.bounds.x = tilePos.x * TILE_W * PHYSICS_SCALE;
	entity->physics.bounds.y = tilePos.y * TILE_H * PHYSICS_SCALE;
	entity->physics.bounds.w = 15 * PHYSICS_SCALE; // Make sure Mr. Fatty get's though those slim trapdoors
	entity->physics.bounds.h = 15 * PHYSICS_SCALE;
	entity->physics.belongsToGroups = COLLISION_GROUP_PLAYER;
	entity->physics.collidesWithGroupMask = COLLISION_GROUP_TERRAIN | COLLISION_GROUP_COLLECTABLE | COLLISION_GROUP_ENEMY;
	entity->offset.x = -10;
	entity->offset.y = -17;

	Scene_addEntity(this->scene, entity);
	this->scene->camera->trackedEntity = entity;

	this->entity = entity;
	this->controlledEntity = this->entity;
}

void Player_processInput(Player* this) {
	int x = Input_getAxis(this->input, horizontal);
	if (x != 0) {
		this->controlledEntity->physics.dx += (x*this->controlledEntity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
		if (x < 0) {
			this->controlledEntity->animatedSprite->sprite->flip = true;
		} else  if (x > 0) {
			this->controlledEntity->animatedSprite->sprite->flip = false;
		}
	}

	if (Input_isDown(this->input, attackSword)) {
		// TODO attack
	}

	int y = Input_getAxis(this->input, vertical);
	bool didJump = false;
	if (Input_isDown(this->input, jump) || y < 0) {
		didJump = Entity_jump(this->controlledEntity);
	}

	if (!didJump && y != 0 && this->controlledEntity->inFrontOfLadder) {
		this->controlledEntity->physics.groundedStatus = onLadder;
		this->controlledEntity->physics.dy += (y*this->controlledEntity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
	}
}

void Player_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}

void Player_earnMoney(Player* this, int money) {
	this->money += money;
	printf("emone %d\n", money);
}
