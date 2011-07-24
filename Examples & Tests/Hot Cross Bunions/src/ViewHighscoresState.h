/*
 * InstructionsState.h
 * 05 Jun 2011
 */

#ifndef VIEWHIGHSCORESSTATE_H_
#define VIEWHIGHSCORESSTATE_H_

#include "ARK.h"
#include "HighscoresComponent.h"

class DefaultGame;

class ViewHighscoresState : public GameState {
	public:

		bool m_fromEndGame;
		float m_timer;
		float m_switchTimer;

		HighscoresComponent* highscores;

		ViewHighscoresState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);

		~ViewHighscoresState();
};

#endif /* VIEWHIGHSCORESSTATE_H_ */
