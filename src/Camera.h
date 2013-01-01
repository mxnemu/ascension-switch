#pragma once
#include "Constants.h"

typedef struct Camera {
	SDL_Rect viewport;
} Camera;

Camera* Camera_create(int w, int h);
void Camera_destroy(Camera* this);
