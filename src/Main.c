/*
 * Main.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "Constants.h"
#include "GameEngine.h"
#include <stdlib.h>

int main(int argc, char **argv) {
	putenv("SDL_VIDEO_CENTERED=1");
	GameEngine* engine = GameEngine_create();
	return GameEngine_run(engine);
}
