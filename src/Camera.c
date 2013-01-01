#include "Camera.h"

Camera* Camera_create(int w, int h) {
	Camera* this = malloc(sizeof(Camera));
	this->viewport.x = 0;
	this->viewport.y = 0;
	this->viewport.w = w;
	this->viewport.h = h;
	return this;
}

void Camera_destroy(Camera* this) {
	free(this);
}
