/*
 * StateBasedGame.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include <vector>
#include "StateBasedGame.h"
#include "GameState.h"
#include "LoadingState.h"
#include "Transition/Transition.h"
#include "Transition/EmptyTransition.h"

StateBasedGame::StateBasedGame(string title):
	Game(title),
	m_states(),
	m_from_state(NULL),
	m_current_state(NULL),
	m_container(NULL),
	m_loading_state(NULL),
	m_enterTransition(NULL),
	m_leaveTransition(NULL),
	m_initialised(false) {

}

string StateBasedGame::getTitle() {
	return Game::getTitle();
}
void StateBasedGame::addState(GameState* state) {
	m_states.push_back(state);
	m_current_state = state;
}
void StateBasedGame::setLoadingState(LoadingState* state) {
	m_loading_state = state;
}
LoadingState* StateBasedGame::getLoadingState() {
	return m_loading_state;
}
void StateBasedGame::enterState(unsigned int id) {
	for (unsigned int i = 0; i < m_states.size(); i++) {
		if (m_states.at(i)->id() == id) {
			enterState(m_states.at(i));
			break;
		}
	}
}
void StateBasedGame::enterState(GameState* state) {
	enterState(state, new EmptyTransition(), new EmptyTransition());
}

void StateBasedGame::enterState(GameState* state, Transition* leave, Transition* enter) {

	m_from_state = m_current_state;
	m_current_state = state;

	if (leave == NULL) {
		//std::cout << "leave was null :(" << std::endl;
		leave = new EmptyTransition();
	}
	if (enter == NULL) {
		//std::cout << "enter was null :(" << std::endl;
		enter = new EmptyTransition();
	}

	m_leaveTransition = leave;
	m_enterTransition = enter;

	m_leaveTransition->init(m_container, this, m_from_state, state);
}
GameState* StateBasedGame::getCurrentState() {
	return m_current_state;
}
unsigned int StateBasedGame::getCurrentStateID() {
	return m_current_state->id();
}

vector<GameState*> StateBasedGame::getStates() {
	return m_states;
}

void StateBasedGame::init(GameContainer* container) {
	m_container = container;

	initStates(container);

	for (unsigned int i = 0; i < m_states.size(); i++) {
		m_states.at(i)->init(container, this);
	}

	m_initialised = true;
}

void StateBasedGame::preUpdate(GameContainer* container, GameTimer* timer) {

}
void StateBasedGame::postUpdate(GameContainer* container, GameTimer* timer) {

}
void StateBasedGame::update(GameContainer* container, GameTimer* timer) {

	//preUpdate(container, timer);

	Game::update(container, timer);

	if (m_leaveTransition != NULL) {
		//std::cout << "leave wasn't null once! :(" << std::endl;
		m_leaveTransition->update(container, this, timer);
		if (m_leaveTransition->isComplete()) {
			m_from_state->leave(container, this, m_current_state);

			delete m_leaveTransition;
			m_leaveTransition = NULL;

			if (m_enterTransition == NULL) {
				m_current_state->enter(container, this, m_from_state);
			} else {
				m_enterTransition->init(container, this, m_from_state, m_current_state);
			}
		} else {
			return;
		}
	}

	if (m_enterTransition != NULL) {
		//std::cout << "enter wasn't null once! :(" << std::endl;
		m_enterTransition->update(container, this, timer);
		if (m_enterTransition->isComplete()) {
			m_current_state->enter(container, this, m_from_state);

			delete m_enterTransition;
			m_enterTransition = NULL;
		} else {
			return;
		}
	}


	m_current_state->update(container, this, timer);

	//postUpdate(container, timer);
}

void StateBasedGame::preRender(GameContainer* container, Graphics* g) {
	#if defined( ARK2D_ANDROID )


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		/*g->setScissorTestEnabled(true);
		glScissor(container->getTranslateX(), container->getTranslateY(),
			float(container->getWidth()) * container->getScaleX(),
			float(container->getHeight()) * container->getScaleY());
			*/

		/*
		 	g->setScissorTestEnabled(true);
		g->scissor(
			container->getTranslateX(),
			container->getTranslateY(),
			(int) float(container->getWidth()) * container->getScaleX(),
			(int) float(container->getHeight()) * container->getScaleY()
		);
		 */

		/*g->setScissorTestEnabled(true);
		g->scissor(
			container->getTranslateX(),
			container->getTranslateY(),
			container->getWidth() * container->getScaleX(),
			container->getHeight() * container->getScaleY()
		);*/

		g->pushMatrix();
		g->translate(container->getTranslateX(), container->getTranslateY());
		g->pushMatrix();
		g->scale(container->getScale(), container->getScale());
	#endif
}
void StateBasedGame::postRender(GameContainer* container, Graphics* g) {
	#if defined( ARK2D_ANDROID )
		g->popMatrix(); // pop scale
		g->popMatrix(); // pop translate

		// draw scissor boxes because glScissor does not work on some HTC phones.
		Color cc = container->getClearColor();
		g->setDrawColor(cc.getRed(), cc.getGreen(), cc.getBlue(), cc.getAlpha());

		// left edge
		g->fillRect(0,0, container->getTranslateX(), container->getHeight() * container->getScaleY());

		// right edge
		g->fillRect(container->getTranslateX() + container->getWidth() * container->getScaleX(), 0,
					container->getTranslateX(), container->getHeight() * container->getScaleY());

		// top edge
		g->fillRect(0,0, container->getWidth() * container->getScaleX(), container->getTranslateY());

		// bottom edge
		g->fillRect(0, container->getTranslateY() + container->getHeight() * container->getScaleY(),
					container->getWidth() * container->getScaleX(), container->getTranslateY());

	#endif
}
void StateBasedGame::render(GameContainer* container, Graphics* g) {
	//preRender(container, g);


	// Render pre-enter transition
	if (m_leaveTransition != NULL) {
		m_leaveTransition->preRender(container, this, g);
	} else if (m_enterTransition != NULL) {
		m_enterTransition->preRender(container, this, g);
	}

	// Render Current State.
	m_current_state->render(container, this, g);

	// Render post-enter transition
	if (m_leaveTransition != NULL) {
		m_leaveTransition->postRender(container, this, g);
	} else if (m_enterTransition != NULL) {
		m_enterTransition->postRender(container, this, g);
	}

	//postRender(container, g);
}

bool StateBasedGame::isTransitioning() {
	return (m_enterTransition != NULL || m_leaveTransition != NULL);
}
bool StateBasedGame::isInitialised() {
	return m_initialised;
}

GameContainer* StateBasedGame::getContainer() {
	return m_container;
}

void StateBasedGame::resize(GameContainer* container, int width, int height) {
	Game::resize(container, width, height);
}

void StateBasedGame::keyPressed(unsigned int key) {
	Game::keyPressed(key);
	m_current_state->keyPressed(key);
}
void StateBasedGame::keyReleased(unsigned int key) {
	Game::keyReleased(key);
	m_current_state->keyReleased(key);
}
void StateBasedGame::mouseMoved(int x, int y, int oldx, int oldy) {
	Game::mouseMoved(x, y, oldx, oldy);
	m_current_state->mouseMoved(x, y, oldx, oldy);
}

StateBasedGame::~StateBasedGame() {

}
