#include "Player.h"

Player* Player_create(Scene* scene, AnimatedSprite* sprite, Input* input) {
	Player* this = malloc(sizeof(Player));
	this->entity = Entity_create(this, scene, sprite);
	this->entity->draw = Player_draw;
	this->entity->update = Player_update;
	this->input = input;

	// test combo
	Combo* c = Combo_create();
	c->action = kickLeft;
	c->name = "left right kick combo";
	Vector_AddElement(this->entity->combos, c);

	Combo* n = Combo_create();
	n->action = kickRight;
	Vector_AddElement(c->followups, n);
	c = n;

	n = Combo_create();
	n->action = kickLeft;
	Vector_AddElement(c->followups, n);
	c = n;

	n = Combo_create();
	n->action = kickRight;
	Vector_AddElement(c->followups, n);

	return this;
}

void Player_destroy(void* context) {
	Player* this = context;
	free(this);
}

void Player_update(void* context, RawTime dt) {
	Player* this = context;
	Player_processInput(this);
}

void Player_processInput(Player* this) {
	int x = Input_getAxis(this->input, horizontal);
	int y = Input_getAxis(this->input, vertical);
	if (x != 0 || 0 != y) {
		this->entity->physics.dx += (x*5 * PHYSICS_SCALE) / AXIS_MAX;
		this->entity->physics.dy += (y*5 * PHYSICS_SCALE) / AXIS_MAX;
		printf("%d, %d\n", this->entity->physics.dx,this->entity->physics.dy);
	}

	if (Input_isDown(this->input, kickLeft)) {
		Entity_performComboAction(this->entity, kickLeft);
	}
	if (Input_isDown(this->input, kickRight)) {
		Entity_performComboAction(this->entity, kickRight);
	}
	if (Input_isDown(this->input, hitLeft)) {
		Entity_performComboAction(this->entity, hitLeft);
	}
	if (Input_isDown(this->input, hitRight)) {
		Entity_performComboAction(this->entity, hitRight);
	}
}

void Player_draw(void* context, SDL_Renderer* renderer, Camera* camera) {
	Player* this = context;
	Sprite_drawOnCamera(this->entity->animatedSprite->sprite, renderer, camera);
}
