/*
 * DefaultGame.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "DefaultGame.h"

DefaultGame::DefaultGame(const char* title): StateBasedGame(title) {

}

void DefaultGame::initStates(GameContainer* container) {
	state_blank = new BlankState();
	state_menu = new MenuState();
	state_instructions = new InstructionsState();
	state_ingame = new InGameState();
	state_win = new WinState();
	state_fail = new FailState();

	addState(state_blank);
	addState(state_menu);
	addState(state_instructions);
	addState(state_ingame);
	addState(state_win);
	addState(state_fail);

	enterState(state_blank);
}

void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	StateBasedGame::update(container, timer);

	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_NUMPAD_ADD) || i->isKeyPressed(Input::KEY_RIGHT)) {
		unsigned int newStateId = getCurrentStateID() + 1;
		if (newStateId == StatesList::END) {
			newStateId = 0;
		}
		enterState(newStateId);
	}
}

void DefaultGame::render(GameContainer* container, Graphics* g) {
	StateBasedGame::render(container, g);
}

void DefaultGame::resize(GameContainer* container, int width, int height) {

}

DefaultGame::~DefaultGame() {

}
