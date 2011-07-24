/*
 * WinState.h
 */

#ifndef WINSTATE_H_
#define WINSTATE_H_

#include "ARK.h"

class DefaultGame;

class WinState : public GameState {
	public:
		DefaultGame* m_game;

		bool m_win;
		signed int m_arcadeInputIndex;

		Image* bg;
		string name;

		vector<signed int> characterIndexes;
		vector<string> characters;

		float m_timer;

		WinState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Graphics* g);

		virtual void keyPressed(unsigned int key);
		virtual void keyReleased(unsigned int key);

		~WinState();
};

#endif /* WINSTATE_H_ */
