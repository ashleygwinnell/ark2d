/*
 * PathfindingTestGameState.cpp
 *
 *  Created on: 27th August 2015
 *      Author: ashleygwinnell
 */

#include "PathfindingTest.h"
#include "../Pathfinding/AStar.h"
#include "../Core/Geometry/Vector2.h"
#include "../Core/Geometry/Polygon.h"

namespace ARK {
	namespace Tests {


		PathfindingTestGameState::PathfindingTestGameState(): 
			GameState(), 
			pathfindingGraphPolygon(),
			m_type(TYPE_GRID)  {

		}

		void PathfindingTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void PathfindingTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

		unsigned int PathfindingTestGameState::id() {
			return 0;
		}

		void PathfindingTestGameState::init(GameContainer* container, StateBasedGame* game) {
			pathfindingGridSource = new Vector2<float>(5, 15);
			pathfindingGridTarget = new Vector2<float>(15, 5);

			// Grid based pathfinding
			pathfindingGrid = new AStar(20, 20, 0, 0, 20);
			for (unsigned int i = 1; i < 19; i++) {
				pathfindingGrid->getNode(i, i)->setBlocked(true);
				pathfindingGrid->getNode(i + 1, i)->setBlocked(true);
			}
			pathfindingGrid->setAllowDiagonal(true);
			pathfindingGrid->processNeighbours();
			pathfindingGrid->findPath(pathfindingGridSource->getX(), pathfindingGridSource->getY(), pathfindingGridTarget->getX(), pathfindingGridTarget->getY());

			// Visibility Graph based pathfinding. 
			{
				float f = 2.2f;
                ARK::Core::Geometry::Polygon<float> outer;
				outer.addPoint(f * 25, f * 25);
				outer.addPoint(f * 75, f * 100);
				outer.addPoint(f * 25, f * 180);
				outer.addPoint(f * 150, f * 170);
				outer.addPoint(f * 275, f * 180);
				outer.addPoint(f * 225, f * 100);
				outer.addPoint(f * 275, f * 25);
				outer.addPoint(f * 150, f * 30);
				pathfindingGraphPolygon.outers.push_back(outer);

                ARK::Core::Geometry::Polygon<float> hole;
				hole.addPoint(f * 125, f * 75);
				hole.addPoint(f * 125, f * 125);
				hole.addPoint(f * 175, f * 125);
				hole.addPoint(f * 175, f * 75);
				pathfindingGraphPolygon.holes.push_back(hole);
				pathfindingGraphPolygon.updateLines();
			}
			pathfindingGraphSource = new AStarNode(250, 200);
			pathfindingGraphTarget = new AStarNode(400, 200);
			pathfindingGraph = new AStar(&pathfindingGraphPolygon);
			refresh();
		}
		void PathfindingTestGameState::initGraph() {
			
		}

		void PathfindingTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

			Input* in = ARK2D::getInput();
			if (m_type == TYPE_GRID) {
				if (in->isKeyPressed(Input::MOUSE_BUTTON_LEFT)) {
					pathfindingGridSource->set(in->getMouseX() / 20, in->getMouseY() / 20);
					refresh();
				}
				if (in->isKeyPressed(Input::MOUSE_BUTTON_RIGHT)) {
					pathfindingGridTarget->set(in->getMouseX() / 20, in->getMouseY() / 20);
					refresh();
				}
			}
			else if (m_type == TYPE_VISIBILITYGRAPH) {
				if (in->isKeyDown(Input::MOUSE_BUTTON_LEFT)) {
					pathfindingGraphSource->m_x = in->getMouseX();
					pathfindingGraphSource->m_y = in->getMouseY();
					refresh();
				}
				if (in->isKeyDown(Input::MOUSE_BUTTON_RIGHT)) {
					pathfindingGraphTarget->m_x = in->getMouseX();
					pathfindingGraphTarget->m_y = in->getMouseY();
					refresh();
				}
			}

			if (in->isKeyPressed(Input::KEY_SPACE)) {
				if (m_type == TYPE_GRID) {
					m_type = TYPE_VISIBILITYGRAPH;
				} else { 
					m_type = TYPE_GRID;
				}
				// refresh();
			}
		}

		void PathfindingTestGameState::refresh() {
			if (m_type == TYPE_GRID) {
				pathfindingGrid->findPath(pathfindingGridSource->getX(), pathfindingGridSource->getY(), pathfindingGridTarget->getX(), pathfindingGridTarget->getY());
			}
			else if (m_type == TYPE_VISIBILITYGRAPH) {
				
				// Remove temporary nodes from the graph.
				pathfindingGraph->removeNode(pathfindingGraphSource);
				pathfindingGraph->removeNode(pathfindingGraphTarget);
				
				// When refreshing, clear the visibiltiy graph / neighbours of the start and end point.
				pathfindingGraphSource->m_neighbours.clear();
				pathfindingGraphTarget->m_neighbours.clear();

				// We also have to disconnect the start/target points from the existing nodes in the graph.
				pathfindingGraph->clearNeighboursFromNodesThatConnectTo(pathfindingGraphSource);
				pathfindingGraph->clearNeighboursFromNodesThatConnectTo(pathfindingGraphTarget);
			
				// Add start and end points. Make sure they're inside the polygon! 
				pathfindingGraph->addNode(pathfindingGraphSource);
				pathfindingGraph->addNode(pathfindingGraphTarget);

				pathfindingGraph->calculateNeighboursForNode(pathfindingGraphSource, &pathfindingGraphPolygon);
				pathfindingGraph->calculateNeighboursForNode(pathfindingGraphTarget, &pathfindingGraphPolygon);

				pathfindingGraph->findPath(pathfindingGraphSource->m_x, pathfindingGraphSource->m_y, pathfindingGraphTarget->m_x, pathfindingGraphTarget->m_y);
			}
		}

		void PathfindingTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			if (m_type == TYPE_GRID) {
				r->setDrawColor(Color::grey);
				r->fillRect(0, 0, container->getWidth(), container->getHeight());
				pathfindingGrid->render();
			}
			else {
				pathfindingGraphPolygon.render();

				r->setDrawColor(Color::green);
				r->fillCircle(pathfindingGraphSource->m_x, pathfindingGraphSource->m_y, 5, 14);
				
				r->setDrawColor(Color::red);
				r->fillCircle(pathfindingGraphTarget->m_x, pathfindingGraphTarget->m_y, 5, 14);

				pathfindingGraph->render();
			}
		}


		PathfindingTestGameState::~PathfindingTestGameState() {

		}











		PathfindingTest::PathfindingTest(): StateBasedGame("Pathfinding Test") {

		}
		void PathfindingTest::initStates(GameContainer* container) {
			addState(new PathfindingTestGameState());
			enterState((unsigned int) 0);
		}
		void PathfindingTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);
		}
		void PathfindingTest::render(GameContainer* container, Renderer* g) {
			StateBasedGame::render(container, g);
		}
		void PathfindingTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		PathfindingTest::~PathfindingTest() {

		}
		int PathfindingTest::start() {
			ARK::Tests::PathfindingTest* test = new ARK::Tests::PathfindingTest();
			GameContainer* container = new GameContainer(*test, 640, 480, 32, false);
			container->start();
			return 0;
		}



	}
}


