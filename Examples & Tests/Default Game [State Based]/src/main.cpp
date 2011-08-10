/*
 * main.c
 *
 *  Created on: 07 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"

int main(int argc, char** argv) {

	std::cout << "woo" << std::endl;

	DefaultGame game("ARK2D State Based Game");
	GameContainer container(game, 640, 480, 32, false);
	container.setClearColor(Color::darker_grey);
	container.setIcon("data/icon.ico");
	container.setCursorVisible(true);
	container.start();

	return 0;
}
