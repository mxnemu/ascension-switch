#include "Camera.h"
#include "Entity.h"

Camera* Camera_create(int w, int h) {
	Camera* this = malloc(sizeof(Camera));
	this->trackedEntity = NULL;
	this->viewport.x = 0;
	this->viewport.y = 0;
	this->viewport.w = w;
	this->viewport.h = h;

	this->bounds.x = 0;
	this->bounds.y = 0;
	this->bounds.w = 0;
	this->bounds.h = 0;
	return this;
}

void Camera_destroy(Camera* this) {
	free(this);
}

void Camera_update(Camera* this) {
	if (this->trackedEntity) {
		SDL_Rect* entityBounds = &((Entity*)this->trackedEntity)->physics.bounds;
		Camera_setTo(this, (entityBounds->x+entityBounds->w)/2 - this->viewport.w/2,
						   (entityBounds->y+entityBounds->h)/2 - this->viewport.h/2);
	}
}

bool Camera_setTo(Camera* this, int newX, int newY) {
	bool moved = false;
	if (newX != this->bounds.x && newX > this->bounds.x && newX + this->viewport.w < this->bounds.x + this->bounds.w ) {
		this->viewport.x = newX;
		moved = true;
	}
	if (newY != this->bounds.y && newY > this->bounds.y && newY + this->viewport.h < this->bounds.y + this->bounds.h) {
		this->viewport.y = newY;
		moved = true;
	}
	return moved;
}

bool Camera_moveBy(Camera* this, int dx, int dy) {
	int newX = this->viewport.x + dx;
	int newY = this->viewport.y + dy;
	return Camera_setTo(this, newX, newY);
}
