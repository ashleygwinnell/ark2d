/*
 * StateBasedGame.h
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#ifndef STATEBASEDGAME_H_
#define STATEBASEDGAME_H_

#include "../Core/Game.h"
#include "../Namespaces.h"
#include "../Core/GameContainer.h"
#include "../Graphics/Renderer.h"
#include "../Core/GameTimer.h"

#include <vector>
#include <string>
using namespace std;


namespace ARK {
	namespace State {

		/*!
		 * \brief Games with multiple game states should inherit from this class for easy management and transition effects between states.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 * @todo continue updating during transitions.
		 */
		class StateBasedGame : public ARK::Core::Game {
			public:
				StateBasedGame(string title);
				string getTitle();

				void addState(GameState* state);
				void enterState(unsigned int id);
				void enterState(GameState* state);

				void setLoadingState(LoadingState* state);
				LoadingState* getLoadingState();

				GameState* getCurrentState();
				unsigned int getCurrentStateID();

				vector<GameState*> getStates();

				void enterState(unsigned int id, ARK::State::Transition::Transition* leave, ARK::State::Transition::Transition* enter);
				void enterState(GameState* state, ARK::State::Transition::Transition* leave, ARK::State::Transition::Transition* enter);

				void init(GameContainer* container);

				virtual void preUpdate(GameContainer* container, GameTimer* timer);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void postUpdate(GameContainer* container, GameTimer* timer);

				virtual void preRender(GameContainer* container, Renderer* g);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void postRender(GameContainer* container, Renderer* g);

				GameContainer* getContainer();

				bool isTransitioning();
				bool isInitialised();

				virtual void initStates(GameContainer* container) = 0;
				virtual void resize(GameContainer* container, int width, int height);

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);

				virtual ~StateBasedGame();

			private:
				vector<GameState*> m_states;
				GameState* m_from_state;
				GameState* m_current_state;
				GameContainer* m_container;

				LoadingState* m_loading_state;

				ARK::State::Transition::Transition* m_enterTransition;
				ARK::State::Transition::Transition* m_leaveTransition;

				bool m_initialised;
				bool m_autoDeleteTransitions;

			protected:
				bool isDuringTransition();

		};

	}
}

#endif
