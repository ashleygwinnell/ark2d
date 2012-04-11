/*
 * LoadingState.cpp
 *
 *  Created on: 1 Apr 2011
 *      Author: Ashley
 */

#include "LoadingState.h"

#include "GameState.h"
#include "../Threading/Thread.h"
#include "../Core/GameContainer.h"
#include "StateBasedGame.h"

namespace ARK {
	namespace State {

		LoadingState::LoadingState(): GameState(), m_thread(NULL), m_loading(true) {
			m_thread = new ARK::Threading::Thread();
		}

		bool LoadingState::isLoading() {
			return m_loading;
		}
		void LoadingState::setLoading(bool b) {
			m_loading = b;
		}

		ARK::Threading::Thread* LoadingState::getThread() {
			return m_thread;
		}

		void LoadingState::init(GameContainer* container, StateBasedGame* game) {
			//m_thread->init((void*) &LoadingState::load);
			//m_thread->start();
		}

		void LoadingState::load() {
			// ..
		}

		void LoadingState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
			GameState::enter(container, game, from);
		}
		void LoadingState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
			GameState::leave(container, game, to);
		}

		LoadingState::~LoadingState() {
			delete m_thread;

			//GameState::~GameState();
		}
	}
}
