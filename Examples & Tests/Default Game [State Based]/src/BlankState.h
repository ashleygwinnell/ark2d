/*
 * BlankState.h
 * 08 Jun 2011
 */

#ifndef BLANKSTATE_H_
#define BLANKSTATE_H_

#include "ARK.h"

class DefaultGame;

class BlankState : public GameState {
	public:
		BlankState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);

		~BlankState();
};

#endif /* BLANKSTATE_H_ */
