#include "Enemy.h"
#include "TextureCache.h"
#include "Scene.h"

Enemy* Enemy_createWhiteKnight(Scene* scene, SDL_Point tilePos) {
	Enemy* this = malloc(sizeof(Enemy));
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_get(scene->engine->textureCache, "images/whiteKnight.png")));
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

	sprite->sprite->flip = scene->mirrorTiles;

	Entity* entity = Entity_create(this, scene, sprite);
	entity->draw = Enemy_draw;
	entity->update = Enemy_update;
	entity->destroy = Enemy_destroy;

	entity->physics.bounds.x = tilePos.x * TILE_W * PHYSICS_SCALE;
	entity->physics.bounds.y = tilePos.y * TILE_H * PHYSICS_SCALE;
	entity->physics.bounds.w = 15 * PHYSICS_SCALE; // Make sure Mr. Fatty get's though those slim trapdoors
	entity->physics.bounds.h = 15 * PHYSICS_SCALE;
	entity->physics.belongsToGroups = COLLISION_GROUP_ENEMY;
	entity->physics.collidesWithGroupMask = COLLISION_GROUP_TERRAIN | COLLISION_GROUP_PLAYER;
	entity->offset.x = -10;
	entity->offset.y = -17;
	this->entity = entity;

	return this;
}

void Enemy_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	Enemy* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}
void Enemy_update(void* context, RawTime dt) {

}

void Enemy_destroy(void* context) {
	Enemy* this = context;
	free(this);
}
