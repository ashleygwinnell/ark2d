/*
 * FailState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "FailState.h"

#include "DefaultGame.h"

FailState::FailState():
	GameState() {

}

unsigned int FailState::id() {
	return StatesList::STATE_FAIL;
}

void FailState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void FailState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void FailState::init(GameContainer* container, StateBasedGame* game) {
	bg = new Image("data/game/fail.png");
}

void FailState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* m_game = DefaultGame::getGame();

		Input* i = ARK2D::getInput();
		if (i->isKeyPressed(Input::KEY_ENTER)) {
			game->enterState(m_game->state_menu);
		}
}

void FailState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	bg->drawCentered(int(container->getWidth()/2), int(container->getHeight()/2));
}

void FailState::keyPressed(unsigned int key) {

}
void FailState::keyReleased(unsigned int key) {

}

FailState::~FailState() {

}

