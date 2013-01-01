#include "Camera.h"

Camera* Camera_create() {
	Camera* this = malloc(sizeof(Camera));
	return this;
}

void Camera_destroy(Camera* this) {
	free(this);
}
