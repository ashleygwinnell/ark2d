/*
 * main.c
 *
 *  Created on: 07 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"

int main(int argc, char** argv) {

	DefaultGame game("Armoured Baseball: The Dark Project");
	GameContainer container(game, 700, 700, 32, false);
	container.setClearColor(Color::darker_grey);
	container.setIcon("data/icon.ico");
	container.setCursorVisible(true);
	container.start();

	return 0;
}
