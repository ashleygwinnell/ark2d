/*
 * GamepadsTest.h
 *
 *  Created on: May 27 2014
 *      Author: ashleygwinnell
 */

#ifndef GamepadsTestState_H__
#define GamepadsTestState_H__

#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		class ARK2D_API GamepadsTestGameState : public GameState {
			public:
				signed int m_gamepadIndex;

			public:
				GamepadsTestGameState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* r);
				virtual ~GamepadsTestGameState();
		};

		class ARK2D_API GamepadsTest : public StateBasedGame {
			public:
				GamepadsTest();

				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* r);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~GamepadsTest();
		};

	}
}



#endif /* GamepadsTestState_H__ */
