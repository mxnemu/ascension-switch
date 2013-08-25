#include "Entity.h"
#include "Hitbox.h"
#include "Utils.h"
#include "Scene.h"
#include "Tile.h"

Entity* Entity_create(void* context, Scene* scene, AnimatedSprite* sprite) {
	Entity* this = malloc(sizeof(Entity));
	this->scene = scene;
	this->animatedSprite = sprite;
	this->destroyed = false;
	EntityPhysics_init(&this->physics, sprite ? sprite->sprite : NULL);
	this->update = emptyUpdate;
	this->draw = Entity_emptyDraw;
	this->destroy = emptyDestroy;
	this->onCollision = Entity_emptyOnCollision;
	this->onHealthModified = Entity_emptyOnHealthModified;
	this->context = context;
	this->speedMultiplier = 5;
	this->inFrontOfLadder = false;
	this->offset.x = 0;
	this->offset.y = 0;
	this->health = 100;
	this->maxHealth = 100;

	this->remainingJumps = 10;
	this->timeSinceGrounded = 0;

	return this;
}

void Entity_destroy(Entity* this) {
	this->destroy(this->context);
	free(this);
}

bool Entity_collides(Entity* this, Entity* other, SDL_Rect* newpos) {
	if ((this->physics.collidesWithGroupMask & other->physics.belongsToGroups) != 0 &&
	    SDL_Rect_touches(newpos, &other->physics.bounds)) {
		int blocks = this->onCollision(this->context, other);
		blocks = other->onCollision(other->context, this) || blocks;
		return blocks; //TODO
	}
	return false;
}


void EntityPhysics_init(EntityPhysics* this, Sprite* sprite) {
	if (sprite) {
		this->bounds.x = 0;
		this->bounds.y = 0;
		this->bounds.w = sprite->bounds.w * PHYSICS_SCALE;
		this->bounds.h = sprite->bounds.h * PHYSICS_SCALE;
	} else {
		SDL_Rect_init(&this->bounds);
	}
	this->dx = 0;
	this->dy = 0;
	this->collidesWithGroupMask = COLLISION_GROUP_TERRAIN;
	this->belongsToGroups = COLLISION_GROUP_NONE;
	this->groundedStatus = inAir;
}

void Entity_update(Entity* this, RawTime dt) {
	this->update(this->context, dt);

	if (this->animatedSprite) {
		AnimationProgress_update(&this->animatedSprite->progress, dt);
	}

	SDL_Rect newPosition = this->physics.bounds;
	newPosition.x += this->physics.dx;
	if (!Entity_wouldCollide(this, &newPosition)) {
		this->physics.bounds.x += this->physics.dx;
	}
	newPosition.x -= this->physics.dx;
	newPosition.y += this->physics.dy;
	if (!Entity_wouldCollide(this, &newPosition)) {
		this->physics.bounds.y += this->physics.dy;
		if (this->physics.groundedStatus == grounded || (this->physics.groundedStatus == onLadder && !this->inFrontOfLadder)) {
			this->physics.groundedStatus = inAir;
		}
	}
	this->physics.dx = this->physics.dy = 0;

	if (this->animatedSprite) {
		this->animatedSprite->sprite->bounds.x = this->offset.x + (this->physics.bounds.x / PHYSICS_SCALE);
		this->animatedSprite->sprite->bounds.y = this->offset.y + (this->physics.bounds.y / PHYSICS_SCALE);
	}

	if (this->physics.groundedStatus == grounded) {
		this->timeSinceGrounded += dt;
	} else {
		this->timeSinceGrounded = 0;
	}
}

bool Entity_jump(Entity* this) {
	if (this->physics.groundedStatus == onLadder ||
		(this->physics.groundedStatus == grounded && this->timeSinceGrounded > 120)) {
		this->remainingJumps = 10;
	}
	if (this->remainingJumps > 0) {
		if (this->physics.groundedStatus == onLadder) {
			this->physics.dy -= (32*5) / PHYSICS_SCALE;
		} else {
			this->physics.dy -= (32*10) / PHYSICS_SCALE;
		}
		this->remainingJumps--;
		return true;
	}
	return false;
}

bool Entity_wouldCollide(Entity* this, SDL_Rect *rect) {
	// test scene
	if (rect->x < this->scene->walkableBounds.x ||
		rect->x + rect->w > this->scene->walkableBounds.x + this->scene->walkableBounds.w ||
		rect->y + rect->h < this->scene->walkableBounds.y) {
		return true;
	}
	if (rect->y + rect->h > this->scene->walkableBounds.y + this->scene->walkableBounds.h) {
		if (this->physics.groundedStatus != immuneToGravity) {
			this->physics.groundedStatus = grounded;
		}
		return true;
	}

	// test entities
	if (this->physics.collidesWithGroupMask & (COLLISION_GROUP_ENEMY | COLLISION_GROUP_COLLECTABLE | COLLISION_GROUP_PLAYER)) {
		for (int i=0; i < this->scene->entities->usedElements; ++i) {
			Entity* it = this->scene->entities->elements[i];
			if (it != NULL) {

				for (int j=0; j < this->scene->entities->usedElements; ++j) {
					Entity* jt = this->scene->entities->elements[j];
					if (jt != NULL && jt != it && Entity_collides(it, jt, rect)) {
						return true;
					}
				}
			}
		}
	}

	this->inFrontOfLadder = false;
	// ineffective lazy check of all tiles
	for (int i=0; i < this->scene->tiles->usedElements; ++i) {
		Tile* tile = Vector_Get(this->scene->tiles, i);
		if (NULL != tile) {
			SDL_Rect tr = tile->physics.bounds;
			if (SDL_Rect_touches(rect, &tr)) {
				if (this->physics.groundedStatus != immuneToGravity && tile->type == TILE_LADDER) {
					this->inFrontOfLadder = true;
				}
				if (this->physics.collidesWithGroupMask & COLLISION_GROUP_TERRAIN) {
					if (tile->blocks && SDL_Rect_above(rect, &tile->physics.bounds)) {
						this->physics.groundedStatus = grounded;
					}
					if (tile->blocks) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

void EntityPhysics_destroy(EntityPhysics* this) {
	free(this);
}

void Entity_modifyHealth(Entity* this, const int health) {
	this->health += health;
	this->health = MIN(this->health, 100);
	this->onHealthModified(this->context);
}

void Entity_emptyDraw(void* context, SDL_Renderer* renderer, Camera* camera) {}
bool Entity_emptyOnCollision(void* context, Entity* otherEntity) { return false; }
void Entity_emptyOnHealthModified(void* context) {}
