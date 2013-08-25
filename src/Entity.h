#pragma once

#include "Constants.h"
#include "AnimatedSprite.h"
#include "List.h"
#include "Camera.h"

#define COLLISION_GROUP_NONE (0)
#define COLLISION_GROUP_ALL (INT_MAX)
#define COLLISION_GROUP_TERRAIN (1)
#define COLLISION_GROUP_PLAYER (1 << 2)
#define COLLISION_GROUP_COLLECTABLE (1 << 3)
#define COLLISION_GROUP_ENEMY (1 << 4)
#define COLLISION_GROUP_CURSOR (1 << 5)

enum GroundedStatus {
	grounded, inAir, onLadder, immuneToGravity
};

typedef struct Scene Scene;
CLASS(EntityPhysics) {
	Scene* scene;
	int dx, dy;
	SDL_Rect bounds;
	int weight;

	int belongsToGroups;
	int collidesWithGroupMask;
	enum GroundedStatus groundedStatus;
};

CLASS(Entity) {
	AnimatedSprite* animatedSprite;
	bool destroyed;
	EntityPhysics physics;

	void (*destroy)(void* context);
	void (*update)(void* context, RawTime dt);
	void (*draw)(void* context, SDL_Renderer*, Camera* camera);
	bool (*onCollision)(void* context, Entity* otherEntity);
	Scene* scene;
	void* context;
	int speedMultiplier;
	bool inFrontOfLadder;
	SDL_Point offset;
	int health;

	int remainingJumps;
	RawTime timeSinceGrounded;
};

Entity* Entity_create(void* context, Scene* scene, AnimatedSprite* sprite);
void Entity_destroy(Entity* this);

void Entity_update(Entity* this, RawTime dt);
bool Entity_collides(Entity* this, Entity* other, SDL_Rect* newpos);
bool Entity_wouldCollide(Entity* this, SDL_Rect *rect);

bool Entity_jump(Entity* this);

void EntityPhysics_init(EntityPhysics* this, Sprite* sprite);
void Entity_emptyDraw(void* context, SDL_Renderer* screen, Camera* camera);
bool Entity_emptyOnCollision(void* context, Entity* otherEntity);

// health + abs(health)
void Entity_heal(Entity* this, int health);
