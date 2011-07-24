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

}

void WinState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

}

void WinState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	g->drawString("Win State", 10, 10);
}

void WinState::keyPressed(unsigned int key) {

}
void WinState::keyReleased(unsigned int key) {

}

WinState::~WinState() {

}

