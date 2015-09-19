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

		GameState::GameState(): 
			scene(NULL) 
		{
			scene = new Scene();
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
			return (scene != NULL && scene->keyPressed(key));
		}
		bool GameState::keyReleased(unsigned int key) {
			return (scene != NULL && scene->keyReleased(key));
		}
		bool GameState::mouseMoved(int x, int y, int oldx, int oldy) {
			return (scene != NULL && scene->mouseMoved(x,y,oldx,oldy));
		}
		void GameState::orientationChanged(int orientation) {

		}
		GameState::~GameState() {
			// hmm
		}

	}
}
