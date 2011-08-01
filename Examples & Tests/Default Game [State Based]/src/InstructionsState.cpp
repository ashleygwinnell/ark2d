/*
 * InstructionsState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "InstructionsState.h"

#include "DefaultGame.h"

InstructionsState::InstructionsState():
	GameState() {

}

unsigned int InstructionsState::id() {
	return StatesList::STATE_INSTRUCTIONS;
}

void InstructionsState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void InstructionsState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void InstructionsState::init(GameContainer* container, StateBasedGame* game) {

}

void InstructionsState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

}

void InstructionsState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	g->drawString("Instructions State", 10, 10);
}

void InstructionsState::keyPressed(unsigned int key) {

}
void InstructionsState::keyReleased(unsigned int key) {

}
void InstructionsState::mouseMoved(int x, int y, int oldx, int oldy) {

}

InstructionsState::~InstructionsState() {

}

