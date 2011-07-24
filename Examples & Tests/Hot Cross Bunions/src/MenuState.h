/*
 * MenuState.h
 */

#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "ARK.h"

class DefaultGame;

class MenuState : public GameState {
	public:
		DefaultGame* m_game;

		Image* bg;

		float m_timer;
		float m_switchTimer;

		MenuState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);
		~MenuState();
};

#endif /* MENUSTATE_H_ */
