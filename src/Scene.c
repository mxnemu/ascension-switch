#include "Scene.h"

Scene* Scene_create() {
	Scene* this = malloc(sizeof(Scene));
	return this;
}

void Scene_destroy(Scene* this) {
	free(this);
}
