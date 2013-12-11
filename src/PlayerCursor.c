#include "PlayerCursor.h"
#include "TextureCache.h"
#include "Scene.h"

PlayerCursor* PlayerCursor_create(struct Scene* scene, const char* colorPrefix) {
	PlayerCursor* this = malloc(sizeof(PlayerCursor));

	char* path = malloc(128 * sizeof(char));
	sprintf(path, "images/%s/cursor.png", colorPrefix);
	AnimatedSprite* sprite = AnimatedSprite_create(Sprite_create(TextureCache_getForUnconstantPath(scene->engine->textureCache, path)));
	free(path);

	Animation* idleAnimation = Animation_create("idle", true);
	List_pushBack(idleAnimation->frames, Frame_create(0, 0, 32, 32, 2000));
	List_pushBack(sprite->animations, idleAnimation);

	Animation* pa1Animation = Animation_create(ANIMATION_PREPARE_ATTACK1, true);
	List_pushBack(pa1Animation->frames, Frame_create(0, 0, 32, 32, 2000));
	List_pushBack(sprite->animations, pa1Animation);

	Animation* a1Animation = Animation_create(ANIMATION_ATTACK1, true);
	List_pushBack(a1Animation->frames, Frame_create(0, 0, 32, 32, 2000));
	List_pushBack(sprite->animations, a1Animation);

	AnimatedSprite_setFrame(sprite, ((Frame*)idleAnimation->frames->first->data)->rect);
	sprite->progress.animation = idleAnimation;

	this->entity = Entity_create(this, scene, sprite);
	this->entity->physics.belongsToGroups = COLLISION_GROUP_CURSOR;
	this->entity->physics.collidesWithGroupMask = COLLISION_GROUP_NONE;
	this->entity->physics.groundedStatus = immuneToGravity;

	const SDL_Point pos = Scene_positionForTileId(scene, (SCENE_TILES_X/2) + (SCENE_TILES_X*SCENE_TILES_Y)/2);
	this->entity->physics.bounds.x = pos.x * TILE_W * PHYSICS_SCALE;
	this->entity->physics.bounds.y = pos.y * TILE_H * PHYSICS_SCALE;

	this->entity->destroy = PlayerCursor_destroy;
	this->entity->draw = PlayerCursor_draw;

	this->entityUnderCursor = NULL;
	return this;
}

void PlayerCursor_destroy(void* context) {
	PlayerCursor* this = context;
	free(this);
}

void PlayerCursor_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	PlayerCursor* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}
