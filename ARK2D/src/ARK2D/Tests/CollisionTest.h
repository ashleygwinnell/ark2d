/*
 * CollisionTestState.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#ifndef COLLISIONTESTSTATE_H__
#define COLLISIONTESTSTATE_H__

#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		class CollisionTestGameState : public GameState {
			public:

				ARK::Geometry::Circle<float>* circleOne;
				ARK::Geometry::Circle<float>* circleTwo;
				ARK::Geometry::Rectangle<float>* rectangleOne;
				ARK::Geometry::Rectangle<float>* rectangleTwo;
				ARK::Geometry::Line<float>* lineOne;
				ARK::Geometry::Line<float>* lineTwo;
				ARK::Geometry::Polygon<float>* polygonOne;
				ARK::Geometry::Polygon<float>* polygonTwo;

				Shape<float>* current;
				std::vector<Shape<float>*> shapes;

				CollisionTestGameState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);
				virtual ~CollisionTestGameState();
		};

		class CollisionTest : public StateBasedGame {
			public:
				CollisionTest();

				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~CollisionTest();
		};

	}
}



#endif /* COLLISIONTESTSTATE_H_ */
