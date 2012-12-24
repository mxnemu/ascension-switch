/*
 * Main.c
 *
 *  Created on: 24.12.2012
 *      Author: nehmulos
 */

#include "Constants.h"
#include "GameEngine.h"

int main(int argc, char **argv) {
	GameEngine* engine = GameEngine_create();
	return GameEngine_run(engine);;
}
