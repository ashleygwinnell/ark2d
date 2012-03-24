/*
 * ParticlesTest.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#ifndef PARTICLESTEST_H_
#define PARTICLESTEST_H_

#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		class ParticlesTestGameState : public GameState {
			public:
				Image* defaultParticle;
				ParticleSystem* system;
				ParticlesTestGameState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Graphics* g);
				virtual ~ParticlesTestGameState();
		};

		class ParticlesTest : public StateBasedGame {
			public:
				ParticlesTest();
				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Graphics* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();
				virtual ~ParticlesTest();
		};

	}
}
#endif /* PARTICLESTEST_H_ */
