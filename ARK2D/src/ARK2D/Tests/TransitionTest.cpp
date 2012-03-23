/*
 * TransitionTest.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: ashleygwinnell
 */

#include "TransitionTest.h"
#include "../../ARK.h"
//#include "../State/Transition/SlideRectanglesAcrossTransition.h"
//#include "../State/Transition/TranslateOutInTransition.h"


namespace ARK {
	namespace Tests {


		TransitionTestGameState::TransitionTestGameState(int index, string name): GameState() {
			this->index = index;
			this->name = name;
		}
		void TransitionTestGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

		}
		void TransitionTestGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

		}
		unsigned int TransitionTestGameState::id() { return this->index; }
		void TransitionTestGameState::init(GameContainer* container, StateBasedGame* game) {

		}
		void TransitionTestGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

		}
		void TransitionTestGameState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
			g->setDrawColor(Color::darker_grey);
			g->fillRect(10,10,container->getWidth()-20, container->getHeight()-20);

			g->setDrawColor(Color::dark_grey);
			g->fillRect(30,30,container->getWidth()-60, container->getHeight()-60);

			g->setDrawColor(Color::grey);
			g->fillRect(50,50,container->getWidth()-100, container->getHeight()-100);

			g->setDrawColor(Color::black_50a);
			g->fillCircleSpikey(container->getWidth()/2, container->getHeight()/2, container->getHeight()/4, 120);
		}
		TransitionTestGameState::~TransitionTestGameState() {

		}

		TransitionTest::TransitionTest():
			StateBasedGame("Transition Test"),
			transitionIndex(0),
			leaveTransitions(),
			entryTransitions()
		{

		}
		void TransitionTest::initStates(GameContainer* container) {

			Color* myWhite = new Color("#ffffff");
			Color* myWhite2 = myWhite->copy();

			leaveTransitions.add(new SlideRectanglesAcrossTransition(myWhite, 30, Constants::DIRECTION_LEFT, 1.0f));
			entryTransitions.add(NULL);

			leaveTransitions.add(new TranslateOutInTransition(Constants::DIRECTION_LEFT, 1.0f));
			entryTransitions.add(NULL);

			leaveTransitions.add(new FadeToColourTransition(1.0f, myWhite2));
			entryTransitions.add(new FadeFromColourTransition(1.0f, myWhite2));

			//leaveTransitions.add(NULL);
			//entryTransitions.add(NULL);

			addState(new TransitionTestGameState(0, "State One"));
			addState(new TransitionTestGameState(1, "State Two"));
			addState(new TransitionTestGameState(2, "State Three"));
			addState(new TransitionTestGameState(3, "State Four"));

			enterState((unsigned int) 0);
		}
		void TransitionTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);

			Input* i = ARK2D::getInput();
			if (i->isKeyPressed(Input::KEY_SPACE) || i->isKeyPressed(Input::KEY_ENTER)) {
				enterState(transitionIndex, leaveTransitions.get(transitionIndex), entryTransitions.get(transitionIndex));
				transitionIndex++;
				if (transitionIndex >= leaveTransitions.size()) {
					transitionIndex = 0;
				}
			}
		}
		void TransitionTest::render(GameContainer* container, Graphics* g) {
			StateBasedGame::render(container, g);
			g->drawStringCenteredAt(StringUtil::append("",transitionIndex), container->getWidth()/2, container->getHeight()/2);
			if (isTransitioning()) {
				g->drawString("Transitioning...", 10, 10);
			}
		}
		void TransitionTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		TransitionTest::~TransitionTest() {

		}

	}
}



