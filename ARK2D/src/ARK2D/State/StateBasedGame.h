/*
 * StateBasedGame.h
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#ifndef STATEBASEDGAME_H_
#define STATEBASEDGAME_H_

#include "../Game.h"
#include "../GameContainer.h"
#include "../Graphics.h"
#include "../GameTimer.h"

#include <vector>
#include <string>

class GameState;
class LoadingState;
class Transition;

using namespace std;

/*!
 * \brief Games with multiple game states should inherit from this class for easy management and transition effects between states.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 * @todo continue updating during transitions.
 */
class StateBasedGame : public Game {
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

		void enterState(unsigned int id, Transition* leave, Transition* enter);
		void enterState(GameState* state, Transition* leave, Transition* enter);

		void init(GameContainer* container);

		virtual void preUpdate(GameContainer* container, GameTimer* timer);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void postUpdate(GameContainer* container, GameTimer* timer);

		virtual void preRender(GameContainer* container, Graphics* g);
		virtual void render(GameContainer* container, Graphics* g);
		virtual void postRender(GameContainer* container, Graphics* g);

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

		Transition* m_enterTransition;
		Transition* m_leaveTransition;

		bool m_initialised;
		bool m_autoDeleteTransitions;

	protected:
		bool isDuringTransition();


};

#endif
