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


		bool GameState::keyPressed(unsigned int key) {
			return (m_rootUIComponent != NULL && m_rootUIComponent->keyPressed(key));
		}
		bool GameState::keyReleased(unsigned int key) {
			return (m_rootUIComponent != NULL && m_rootUIComponent->keyReleased(key));
		}
		bool GameState::mouseMoved(int x, int y, int oldx, int oldy) {
			return (m_rootUIComponent != NULL && m_rootUIComponent->mouseMoved(x,y,oldx,oldy));
		}
		void GameState::orientationChanged(int orientation) {

		}
		GameState::~GameState() {
			// hmm
		}

	}
}
