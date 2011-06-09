/*
 * MenuState.h
 * 08 Jun 2011
 */

#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "ARK.h"

class DefaultGame;

class MenuState : public GameState {
	public:

		Image* bg;

		MenuState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);

		~MenuState();
};

#endif /* MENUSTATE_H_ */
