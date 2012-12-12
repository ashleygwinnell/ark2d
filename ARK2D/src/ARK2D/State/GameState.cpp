/*
 * GameState.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include "GameState.h"
#include "StateBasedGame.h"

namespace ARK {
	namespace State {

		GameState::GameState(): m_rootUIComponent(NULL) {

		}

		void GameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
			ARK2D::getLog()->v("GameState::enter");
		}
		void GameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
			ARK2D::getLog()->v("GameState::leave");
		}

		void GameState::pause() { 
			ARK2D::getLog()->v("GameState::pause");
		}
		void GameState::resume() { 
			ARK2D::getLog()->v("GameState::resume");
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

	}
}
