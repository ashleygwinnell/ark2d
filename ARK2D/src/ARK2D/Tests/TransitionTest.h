/*
 * TransitionTest.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#ifndef TRANSITIONTEST_H_
#define TRANSITIONTEST_H_

/*#include "../State/StateBasedGame.h"
#include "../State/GameState.h"
#include "../Util/Containers/Vector.h"
#include "../ARK2D.h"*/
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
				void render(GameContainer* container, StateBasedGame* game, Graphics* g);
				virtual ~TransitionTestGameState();
		};

		class TransitionTest : public StateBasedGame {
			public:
				unsigned int transitionIndex;
				ARK::Util::Containers::Vector<Transition*> leaveTransitions;
				ARK::Util::Containers::Vector<Transition*> entryTransitions;

				TransitionTest();
				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Graphics* g);
				virtual void resize(GameContainer* container, int width, int height);
				virtual ~TransitionTest();
		};

	}
}

int ARK_Tests_TransitionTest_Main() {
	ARK::Tests::TransitionTest* test = new ARK::Tests::TransitionTest();
	GameContainer container(*test, 800, 600, 32, false);
	container.start();
	return 0;
}

#endif /* TRANSITIONTEST_H_ */
