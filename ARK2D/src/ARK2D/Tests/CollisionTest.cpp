/*
 * CollisionTestGameState.cpp
 *
 *  Created on: Jan 23, 2012
 *      Author: ashleygwinnell
 */

#include "CollisionTest.h"
#include "../Geometry/Circle.h"
#include "../Geometry/Rectangle.h"
#include "../Geometry/Line.h"
#include "../Geometry/Polygon.h"

namespace ARK {
	namespace Tests {


		CollisionTestGameState::CollisionTestGameState(): GameState(), shapes() {

		}

		void CollisionTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
		void CollisionTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

		unsigned int CollisionTestGameState::id() {
			return 0;
		}

		void CollisionTestGameState::init(GameContainer* container, StateBasedGame* game) {
			circleOne = new Circle<float>(200,200,40);
			circleTwo = new Circle<float>(100,100,30);
			rectangleOne = new ARK::Geometry::Rectangle(50, 300, 50, 40);
			rectangleTwo = new ARK::Geometry::Rectangle(50, 400, 100, 100);
			lineOne = new Line<float>(300, 500, 350, 550);
			lineTwo = new Line<float>(300, 200, 350, 150);
			lineThree = new Line<float>(500, 200, 500, 300);
			lineFour = new Line<float>(500, 200, 700, 200);

			polygonOne = new ARK::Geometry::Polygon<float>();
			polygonOne->addPoint(0,0);
			polygonOne->addPoint(150,150);
			polygonOne->addPoint(0,150);
			polygonOne->setLocationByCenter(300,300);

			polygonTwo = new ARK::Geometry::Polygon<float>();
			polygonTwo->addPoint(400, 50);
			polygonTwo->addPoint(450, 50);
			polygonTwo->addPoint(450, 100);

			current = circleOne;
			shapes.push_back(circleOne);
			shapes.push_back(circleTwo);
			shapes.push_back(rectangleOne);
			shapes.push_back(rectangleTwo);
			shapes.push_back(lineOne);
			shapes.push_back(lineTwo);
			shapes.push_back(lineThree);
			shapes.push_back(lineFour);
			shapes.push_back(polygonOne);
			shapes.push_back(polygonTwo); 
		}

		void CollisionTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

			Input* i = ARK2D::getInput();
			int moveValue = 3;
			if (i->isKeyDown(Input::KEY_DOWN)) {
				current->setLocationByCenter(current->getCenterX(), current->getCenterY() + moveValue);
			}
			if (i->isKeyDown(Input::KEY_UP)) {
				current->setLocationByCenter(current->getCenterX(), current->getCenterY() - moveValue);
			}
			if (i->isKeyDown(Input::KEY_LEFT)) {
				current->setLocationByCenter(current->getCenterX() - moveValue, current->getCenterY());
			}
			if (i->isKeyDown(Input::KEY_RIGHT)) {
				current->setLocationByCenter(current->getCenterX() + moveValue, current->getCenterY());
			}

			if (i->isKeyPressed(Input::KEY_0)) { current = shapes.at(0); }
			else if (i->isKeyPressed(Input::KEY_1)) { current = shapes.at(1); }
			else if (i->isKeyPressed(Input::KEY_2)) { current = shapes.at(2); }
			else if (i->isKeyPressed(Input::KEY_3)) { current = shapes.at(3); }
			else if (i->isKeyPressed(Input::KEY_4)) { current = shapes.at(4); }
			else if (i->isKeyPressed(Input::KEY_5)) { current = shapes.at(5); }
			else if (i->isKeyPressed(Input::KEY_6)) { current = shapes.at(6); }
			else if (i->isKeyPressed(Input::KEY_7)) { current = shapes.at(7); }
			else if (i->isKeyPressed(Input::KEY_8)) { current = shapes.at(8); }
			else if (i->isKeyPressed(Input::KEY_9)) { current = shapes.at(9); }

		}
		void CollisionTestGameState::render(GameContainer* container, StateBasedGame* game, Renderer* g) {
			g->setDrawColor(Color::darker_grey);
			g->fillRect(0,0,container->getWidth(), container->getHeight());

			g->setDrawColor(Color::white);
			g->drawString("Collision Tests", 10, 10);

			for(unsigned int i = 0; i < shapes.size(); i++) {
				g->setLineWidth(1);
				g->setDrawColor(Color::white);
				for(unsigned int j = 0; j < shapes.size(); j++) {
					if (i == j) { continue; }
					if (shapes.at(i)->collides(shapes.at(j))) {
						g->setLineWidth(3);
						g->setDrawColor(Color::red);
					}
				}
				if (shapes.at(i)->isPartlyOffscreen()) {
					g->setLineWidth(3);
					g->setDrawColor(Color::cyan);
				}
				shapes.at(i)->render();
				g->drawStringCenteredAt(StringUtil::append("",i), shapes.at(i)->getCenterX(), shapes.at(i)->getCenterY());
			}
			//
		}


		CollisionTestGameState::~CollisionTestGameState() {

		}











		CollisionTest::CollisionTest(): StateBasedGame("Collision Test") {

		}
		void CollisionTest::initStates(GameContainer* container) {
			addState(new CollisionTestGameState());
			enterState((unsigned int) 0);
		}
		void CollisionTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);
		}
		void CollisionTest::render(GameContainer* container, Renderer* g) {
			StateBasedGame::render(container, g);
		}
		void CollisionTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		CollisionTest::~CollisionTest() {

		}
		int CollisionTest::start() {
			ARK::Tests::CollisionTest* test = new ARK::Tests::CollisionTest();
			GameContainer* container = new GameContainer(*test, 800, 600, 32, false);
			container->start();
			return 0;
		}



	}
}


