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

}

void FailState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

}

void FailState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	g->drawString("Fail State", 10, 10);
}

void FailState::keyPressed(unsigned int key) {

}
void FailState::keyReleased(unsigned int key) {

}
void FailState::mouseMoved(int x, int y, int oldx, int oldy) {

}

FailState::~FailState() {

}

