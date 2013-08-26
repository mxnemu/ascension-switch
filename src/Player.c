#include "Player.h"
#include "AnimatedSprite.h"
#include "TextureCache.h"
#include "Scene.h"
#include "PlayerCursor.h"

Player* Player_create(Scene* scene, Input* input) {
	Player* this = malloc(sizeof(Player));
	this->input = input;
	this->scene = scene;
	this->money = 25;
	this->opponent = NULL;
	this->controlledEntity.player = this;
	this->controlledEntity.entity = NULL;
	this->controlledEntity.originalContext = NULL;
	this->controlledEntity.originalDestroy = NULL;
	this->controlledEntity.originalDraw = NULL;
	this->controlledEntity.destroyOnRelease= false;
	this->controlledEntity.indicator = NULL;
	this->sidebarUi = NULL;
	this->dungeonMasterUi = NULL;
	this->timeSinceLastEnemySpawn = PLAYER_SPAWN_ENEMY_DELAY;
	this->timeSinceLastSelectionChange = PLAYER_SELECTION_CHANGE_DELAY;
	Player_loadIndicator(this);

	this->entity = Player_spawnPlayerEntity(this);
	ControlledEntity_set(&this->controlledEntity, this->entity);

	return this;
}

Entity* _Player_onEntityDestroyed(void* context) {
	Player* this = context;
	this->money -= 1;
	if (this->controlledEntity.entity == this->entity) {
		this->entity = NULL;
		this->entity = Player_spawnPlayerEntity(this);
	}
	return this->entity;
}

void Player_destroy(void* context) {
	CANCEL_IF_FALSE(context);
	Player* this = context;
	this->controlledEntity.destroyOnRelease = false;
	ControlledEntity_release(&this->controlledEntity);
	free(this);
}

void Player_loadIndicator(Player* this) {
	char* path = malloc(100 * sizeof(char));
	sprintf(path, "images/%s/indicator.png", this->scene->colorPrefix);
	this->controlledEntity.indicator = Sprite_create(TextureCache_getForUnconstantPath(this->scene->engine->textureCache, path));
	free(path);
}

void Player_switchMode(Player* this) {
	if (this->controlledEntity.entity == this->entity) {
		PlayerCursor* cursor = PlayerCursor_create(this->opponent->scene, this->scene->colorPrefix);
		Scene_addEntity(this->opponent->scene, cursor->entity);
		ControlledEntity_set(&this->controlledEntity, cursor->entity);
		this->controlledEntity.destroyOnRelease = true;

		this->dungeonMasterUi = DungeonMasterUi_create(this);
	} else {
		DungeonMasterUi_destroy(this->dungeonMasterUi);
		this->dungeonMasterUi = NULL;
		ControlledEntity_set(&this->controlledEntity, this->entity);
	}
}

void Player_updateEntity(void* context, RawTime dt) {
}

void Player_destroyEntity(void* context) {
}

void Player_onEntityHealthModified(void* context) {
	ControlledEntity* ce = context;
	Player* this = ce->player;
	Healthbar_set(this->sidebarUi->healthbar, this->entity->health, this->entity->maxHealth);
}

void Player_update(void* context, RawTime dt) {
	Player* this = context;
	Player_processInput(this);
	PlayerSidebarUi_update(this->sidebarUi, dt);

	if (this->timeSinceLastEnemySpawn < PLAYER_SPAWN_ENEMY_DELAY) {
		this->timeSinceLastEnemySpawn += dt;
	}
	if (this->timeSinceLastSelectionChange < PLAYER_SELECTION_CHANGE_DELAY) {
		this->timeSinceLastSelectionChange += dt;
	}
}

Entity* Player_spawnPlayerEntity(Player* this) {
	char path[128];
	sprintf(path, "images/%s/knight.png", this->scene->colorPrefix);
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_getForUnconstantPath(this->scene->engine->textureCache, path)));
	Animation* idleAnimation = Animation_create("idle", true);
	List_pushBack(idleAnimation->frames, Frame_create(0,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(64,0, 32, 32, 200));
	List_pushBack(idleAnimation->frames, Frame_create(32,0, 32, 32, 200));
	List_pushBack(sprite->animations, idleAnimation);

	Animation* attack1PrepareAnimation = Animation_create(ANIMATION_PREPARE_ATTACK1, false);
	List_pushBack(attack1PrepareAnimation->frames, Frame_create(0, 32, 32, 32, 60));
	List_pushBack(attack1PrepareAnimation->frames, Frame_create(32,32, 32, 32, 60));
	List_pushBack(attack1PrepareAnimation->frames, Frame_create(0, 32, 32, 32, 60));
	List_pushBack(sprite->animations, attack1PrepareAnimation);

	Animation* attack1Animation = Animation_create(ANIMATION_ATTACK1, false);
	List_pushBack(attack1Animation->frames, Frame_create(64,32, 36, 32, 120));
	List_pushBack(sprite->animations, attack1Animation);

	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	sprite->sprite->flip = this->scene->mirrorTiles;

	Entity* entity = Entity_create(this, this->scene, sprite);
	entity->draw = Player_drawEntity;
	entity->update = Player_updateEntity;
	entity->destroy = Player_destroyEntity;
	entity->onHealthModified = Player_onEntityHealthModified;

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

	return entity;
}

void Player_processInput(Player* this) {
	PlayerCursor* cursor = NULL;
	if (this->dungeonMasterUi && (this->controlledEntity.entity->physics.belongsToGroups & COLLISION_GROUP_CURSOR)) {
		cursor = this->controlledEntity.entity->context;
	}

	int x = Input_getAxis(this->input, horizontal);
	if (x != 0) {
		this->controlledEntity.entity->physics.dx += (x*this->controlledEntity.entity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
		// shitty check to not flip the cursor
		if (!cursor) {
			if (x < 0) {
				this->controlledEntity.entity->animatedSprite->sprite->flip = true;
			} else  if (x > 0) {
				this->controlledEntity.entity->animatedSprite->sprite->flip = false;
			}
		}
	}

	int y = Input_getAxis(this->input, vertical);
	if (cursor) {
		if (y != 0) {
			this->controlledEntity.entity->physics.dy += (y*this->controlledEntity.entity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
		}

		if (Input_isDown(this->input, action) && this->timeSinceLastEnemySpawn >= PLAYER_SPAWN_ENEMY_DELAY) {
			SDL_Point p = {
					.x= this->controlledEntity.entity->animatedSprite->sprite->bounds.x,
					.y= this->controlledEntity.entity->animatedSprite->sprite->bounds.y
			};
			DungeonMasterUi_summonSelectedCard(this->dungeonMasterUi, this->opponent->scene, Scene_positionToTilePosition(p));
			this->timeSinceLastEnemySpawn = 0;
		}

		if (Input_isDown(this->input, attackBow)) {
			DungeonMasterUi_selectPrevious(this->dungeonMasterUi);
			this->timeSinceLastSelectionChange = 0;
		}

		if (Input_isDown(this->input, attackSword) && this->timeSinceLastSelectionChange >= PLAYER_SELECTION_CHANGE_DELAY) {
			DungeonMasterUi_selectNext(this->dungeonMasterUi);
			this->timeSinceLastSelectionChange = 0;
		}

	} else {

		if (Input_isDown(this->input, attackSword) &&
			strcmp(this->controlledEntity.entity->animatedSprite->progress.animation->name, ANIMATION_PREPARE_ATTACK1) != 0 &&
			strcmp(this->controlledEntity.entity->animatedSprite->progress.animation->name, ANIMATION_ATTACK1) != 0) {
			AnimatedSprite_setAnimation(this->controlledEntity.entity->animatedSprite, ANIMATION_PREPARE_ATTACK1);
		}

		bool didJump = false;
		if (Input_isDown(this->input, jump) || y < 0) {
			didJump = Entity_jump(this->controlledEntity.entity);
		}

		if (!didJump && y != 0 && this->controlledEntity.entity->inFrontOfLadder) {
			this->controlledEntity.entity->physics.groundedStatus = onLadder;
			this->controlledEntity.entity->physics.dy += (y*this->controlledEntity.entity->speedMultiplier * PHYSICS_SCALE) / AXIS_MAX;
		}
	}
}

void Player_drawEntity(void* context, SDL_Renderer* renderer, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}

void Player_draw(void* context, SDL_Renderer* renderer) {
	Player* this = context;
	PlayerSidebarUi_draw(this->sidebarUi, renderer);
	if (this->dungeonMasterUi) {
		DungeonMasterUi_draw(this->dungeonMasterUi, renderer);
	}
}

void Player_earnMoney(Player* this, int money) {
	this->money += money;
	if (this->sidebarUi) {
		PlayerSidebarUi_setMoney(this->sidebarUi, this->money);
	}
}

void ControlledEntity_onEntityDestroyed(void* context) {
	ControlledEntity* this = context;
	Entity* nextEntity = _Player_onEntityDestroyed(this->player);

	Entity* oldEntity = this->entity;
	ControlledEntity_release(this);
	oldEntity->destroy(oldEntity->context);
	this->destroyOnRelease = false;
	ControlledEntity_set(this, nextEntity);
}

void ControlledEntity_release(ControlledEntity* this) {
	if (this->entity) {
		this->entity->destroy = this->originalDestroy;
		this->entity->draw = this->originalDraw;
		this->entity->context = this->originalContext;

		if (this->destroyOnRelease) {
			this->entity->destroyed = true;
			this->destroyOnRelease = false;
		}
		this->entity = NULL;
	}
}

void ControlledEntity_set(ControlledEntity* this, Entity* entity) {
	ControlledEntity_release(this);
	this->entity = entity;
	if (entity) {
		this->originalDestroy = entity->destroy;
		this->originalContext = entity->context;
		this->originalDraw = entity->draw;

		this->entity->draw = ControlledEntity_draw;
		this->entity->destroy = ControlledEntity_onEntityDestroyed;
		this->entity->context = this;
	}
}

void ControlledEntity_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	ControlledEntity* this = context;
	if (this->indicator) {
		this->indicator->bounds.y = this->entity->animatedSprite->sprite->bounds.y - this->indicator->bounds.h;
		this->indicator->bounds.x = this->entity->animatedSprite->sprite->bounds.x;
		Sprite_drawOnCamera(this->indicator, renderer, camera);
	}
	this->originalDraw(this->originalContext, renderer, camera);
}
