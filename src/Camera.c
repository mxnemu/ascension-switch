#include "Camera.h"

Camera* Camera_create(int w, int h) {
	Camera* this = malloc(sizeof(Camera));
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


bool Camera_moveBy(Camera* this, int dx, int dy) {
	int newX = this->viewport.x + dx;
	int newY = this->viewport.y + dy;
	bool moved = false;
	if (dx != 0 && newX > this->bounds.x && newX + this->viewport.w < this->bounds.x + this->bounds.w ) {
		this->viewport.x = newX;
		moved = true;
	}
	if (dy != 0 && newY > this->bounds.y && newY + this->viewport.h < this->bounds.y + this->bounds.h) {
		this->viewport.y = newY;
		moved = true;
	}
	return moved;
}
