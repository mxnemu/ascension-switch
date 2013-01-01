#pragma once
#include "Constants.h"

typedef struct Camera {
	SDL_Rect viewport;
} Camera;

Camera* Camera_create();
void Camera_destroy(Camera* this);
