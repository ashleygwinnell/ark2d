/*
 * BlankState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "BlankState.h"

#include "DefaultGame.h"

BlankState::BlankState():
	GameState() {

}

unsigned int BlankState::id() {
	return StatesList::STATE_BLANK;
}

void BlankState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void BlankState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void BlankState::init(GameContainer* container, StateBasedGame* game) {

}

void BlankState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

}

void BlankState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	g->drawString("Blank State", 10, 10);
}

void BlankState::keyPressed(unsigned int key) {

}
void BlankState::keyReleased(unsigned int key) {

}
void BlankState::mouseMoved(int x, int y, int oldx, int oldy) {

}

BlankState::~BlankState() {

}

