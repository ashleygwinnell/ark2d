/*
 * CollisionTestState.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_TESTS_COLLISIONTESTSTATE_H__
#define ARK_TESTS_COLLISIONTESTSTATE_H__

//#include "../../ARK.h"
#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/State/GameState.h"
#include "../Core/State/StateBasedGame.h"

#include <vector>
using std::vector;

namespace ARK {
	namespace Tests {

		class ARK2D_API CollisionTestGameState : public GameState {
			public:

                ARK::Core::Geometry::Circle<float>* circleOne;
				ARK::Core::Geometry::Circle<float>* circleTwo;
				ARK::Core::Geometry::Rectangle* rectangleOne;
				ARK::Core::Geometry::Rectangle* rectangleTwo;
				ARK::Core::Geometry::Line<float>* lineOne;
				ARK::Core::Geometry::Line<float>* lineTwo;
				ARK::Core::Geometry::Line<float>* lineThree;
				ARK::Core::Geometry::Line<float>* lineFour;
				ARK::Core::Geometry::Polygon<float>* polygonOne;
				ARK::Core::Geometry::Polygon<float>* polygonTwo;

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

		class ARK2D_API CollisionTest : public StateBasedGame {
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



#endif /* ARK_TESTS_COLLISIONTESTSTATE_H_ */
