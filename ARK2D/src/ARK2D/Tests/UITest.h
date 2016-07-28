/*
 * UITestState.h
 *
 *  Created on: Mar 24, 2011.
 *      Author: ashleygwinnell
 */

#ifndef UITestState_H__
#define UITestState_H__

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../State/GameState.h"
#include "../State/StateBasedGame.h"

#include <vector>
using std::vector;

namespace ARK {
	namespace Tests {

		class ARK2D_API UITestGameState : public GameState {
			public:

				ScrollPanel* rightPanel;

				UITestGameState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);
				//void keyPressed(unsigned int key);
				//void keyReleased(unsigned int key);
				virtual ~UITestGameState();
		};

		class ARK2D_API UITest : public StateBasedGame {
			public:
				UITest();

				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~UITest();
		};

	}
}



#endif /* UITestState_H__ */
