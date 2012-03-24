/*
 * GameState.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

//#include "../GameContainer.h"
#include "GameState.h"
#include "StateBasedGame.h"



GameState::GameState(): m_rootUIComponent(NULL) {

}

void GameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void GameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void GameState::keyPressed(unsigned int key) {
	if (m_rootUIComponent != NULL) { m_rootUIComponent->keyPressed(key); }
}
void GameState::keyReleased(unsigned int key) {
	if (m_rootUIComponent != NULL) { m_rootUIComponent->keyReleased(key); }
}
void GameState::mouseMoved(int x, int y, int oldx, int oldy) {
	if (m_rootUIComponent != NULL) { m_rootUIComponent->mouseMoved(x,y,oldx,oldy); }
}
GameState::~GameState() {
	// hmm
}
