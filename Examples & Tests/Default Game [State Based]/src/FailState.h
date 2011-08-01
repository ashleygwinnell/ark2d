/*
 * FailState.h
 * 08 Jun 2011
 */

#ifndef FAILSTATE_H_
#define FAILSTATE_H_

#include "ARK.h"

class DefaultGame;

class FailState : public GameState {
	public:

		FailState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);
		virtual void mouseMoved(int x, int y, int oldx, int oldy);

		~FailState();
};

#endif /* FAILTATE_H_ */
