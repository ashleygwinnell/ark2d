/*
 * main.c
 *
 *  Created on: 07 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "DefaultGame.h"

int main(int argc, char** argv) {

	//OutputWrapper::setEnabled(true);

	DefaultGame game(string("LOVE ME, TOO! Wave Editor"));
	GameContainer container(game, 1020, 768, 32, false);
	container.setResizable(true);
	container.setScaleToWindow(false);
	container.setClearColor(Color::darker_grey);
	container.setIcon("data/icon.ico");
	container.setCursorVisible(true);
	container.start();

	return 0;
}
