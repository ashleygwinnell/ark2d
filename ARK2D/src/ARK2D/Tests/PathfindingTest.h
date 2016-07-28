/*
 * PathfindingTestState.h
 *
 *  Created on: 27th August 2015
 *      Author: ashleygwinnell
 */

#ifndef ARK_TESTS_PATHFINDINGTESTSTATE_H__
#define ARK_TESTS_PATHFINDINGTESTSTATE_H__

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../State/GameState.h"
#include "../State/StateBasedGame.h"
#include "../Geometry/AdvancedPolygon.h"

#include <vector>
using std::vector;

namespace ARK {
	namespace Tests {

		class ARK2D_API PathfindingTestGameState : public GameState {
			public:

				AStar* pathfindingGrid;
				Vector2<float>* pathfindingGridSource;
				Vector2<float>* pathfindingGridTarget;

				AStar* pathfindingGraph;
				AdvancedPolygon pathfindingGraphPolygon;
				AStarNode* pathfindingGraphSource;
				AStarNode* pathfindingGraphTarget;

				unsigned int m_type;
				static const unsigned int TYPE_GRID = 0;
				static const unsigned int TYPE_VISIBILITYGRAPH = 1;

				PathfindingTestGameState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				void initGraph();
				void calculateNeighboursForNode(AStarNode* start);

				void refresh();

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);
				virtual ~PathfindingTestGameState();
		};

		class ARK2D_API PathfindingTest : public StateBasedGame {
			public:
				PathfindingTest();

				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~PathfindingTest();
		};

	}
}



#endif /* ARK_TESTS_PATHFINDINGTESTSTATE_H__ */
