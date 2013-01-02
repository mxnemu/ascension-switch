/*
 * Entity.h
 *
 *  Created on: 02.01.2013
 *      Author: nehmulos
 */

CLASS(Entity) {
	Sprite* sprite;
	int dx, dy;
	void (*update)(void* context, RawTime dt);
	void (*draw)(void* context, SDL_Surface*);
	Scene* scene;
	int team;
}

