/*
 * InstructionsState.h
 * 05 Jun 2011
 */

#ifndef INSTRUCTIONSSTATE_H_
#define INSTRUCTIONSSTATE_H_

#include "ARK.h"

class DefaultGame;

class InstructionsState : public GameState {
	public:

		float m_timer;

		InstructionsState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);

		~InstructionsState();
};

#endif /* INSTRUCTIONSSTATE_H_ */
