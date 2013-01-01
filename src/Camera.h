#pragma once
#include "Constants.h"

typedef struct Camera {
	SDL_Rect viewport;
	SDL_Rect bounds;
} Camera;

Camera* Camera_create(int w, int h);
void Camera_destroy(Camera* this);

bool Camera_moveBy(Camera* this, int dx, int dy);
