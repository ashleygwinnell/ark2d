/*
 * WinState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "WinState.h"

#include "DefaultGame.h"

WinState::WinState():
	GameState() {

}

unsigned int WinState::id() {
	return StatesList::STATE_WIN;
}

void WinState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void WinState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void WinState::init(GameContainer* container, StateBasedGame* game) {
	bg = new Image(container->getResourcePath() + "data/game/win.png");
}

void WinState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* m_game = DefaultGame::getGame();

	Input* i = ARK2D::getInput();
	if (i->isKeyPressed(Input::KEY_ENTER)) {
		game->enterState(m_game->state_menu);
	}
}

void WinState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	bg->drawCentered(int(container->getWidth()/2), int(container->getHeight()/2));
}

void WinState::keyPressed(unsigned int key) {

}
void WinState::keyReleased(unsigned int key) {

}

WinState::~WinState() {

}

