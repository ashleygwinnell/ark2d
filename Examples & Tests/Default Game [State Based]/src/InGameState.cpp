/*
 * InGameState.cpp
 *
 *  Created on: 08 Jun 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "InGameState.h"

#include "DefaultGame.h"

InGameState::InGameState():
	GameState() {

}

unsigned int InGameState::id() {
	return StatesList::STATE_INGAME;
}

void InGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void InGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void InGameState::init(GameContainer* container, StateBasedGame* game) {

}

void InGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

}

void InGameState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	g->drawString("InGame State", 10, 10);
}

void InGameState::keyPressed(unsigned int key) {

}
void InGameState::keyReleased(unsigned int key) {

}

InGameState::~InGameState() {

}

