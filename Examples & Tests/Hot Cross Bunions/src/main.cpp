/*
 * main.c
 *
 *  Created on: 17 Jan 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"


int main(int argc, char** argv) {

	DefaultGame game("HOT CROSS BUNIONS");
	GameContainer* container;
	if (DefaultGame::ARCADE_VERSION) {
		container = new GameContainer(game, 1024, 768, 32, false);
		//container->setScaleToWindow(true);
		//container->setResizable(true);
	} else {
		container = new GameContainer(game, 640, 480, 32, false);
	}

	container->setClearColor(Color::darker_grey);
	container->setCursorVisible(true);
	container->setIcon(container->getResourcePath() + "data/icon.ico");
	container->start();

	return 0;
}
