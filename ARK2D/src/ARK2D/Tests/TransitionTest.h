/*
 * TransitionTest.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_TESTS_TRANSITIONTEST_H_
#define ARK_TESTS_TRANSITIONTEST_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/State/GameState.h"
#include "../Core/State/StateBasedGame.h"
#include "../Core/Vector.h"

namespace ARK {
	namespace Tests {

		class ARK2D_API TransitionTestGameState : public GameState {
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

		class ARK2D_API TransitionTest : public StateBasedGame {
			public:
				unsigned int transitionIndex;
                ARK::Core::Vector<ARK::Core::State::Transition::Transition*> leaveTransitions;
                ARK::Core::Vector<ARK::Core::State::Transition::Transition*> entryTransitions;

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



#endif /* ARK_TESTS_TRANSITIONTEST_H_ */
