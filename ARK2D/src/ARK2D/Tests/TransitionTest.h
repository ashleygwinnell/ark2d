/*
 * TransitionTest.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#ifndef TRANSITIONTEST_H_
#define TRANSITIONTEST_H_

#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		class TransitionTestGameState : public GameState {
			public:
				int index;
				string name;
				TransitionTestGameState(int index, string name);
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);
				virtual ~TransitionTestGameState();
		};

		class TransitionTest : public StateBasedGame {
			public:
				unsigned int transitionIndex;
				ARK::Util::Containers::Vector<ARK::State::Transition::Transition*> leaveTransitions;
				ARK::Util::Containers::Vector<ARK::State::Transition::Transition*> entryTransitions;

				TransitionTest();
				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~TransitionTest();
		};

	}
}



#endif /* TRANSITIONTEST_H_ */
