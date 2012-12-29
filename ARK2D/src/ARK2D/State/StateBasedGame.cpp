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
#include "../UI/ErrorDialog.h"

namespace ARK {
	namespace State {

		StateBasedGame::StateBasedGame(string title):
			Game(title),
			m_states(),
			m_from_state(NULL),
			m_current_state(NULL),
			m_next_state(NULL), 
			m_container(NULL),
			m_loading_state(NULL), 
			m_enterTransition(NULL),
			m_leaveTransition(NULL),
			m_initialised(false),
			m_autoDeleteTransitions(false) {

		}

		string StateBasedGame::getTitle() {
			return Game::getTitle();
		}
		void StateBasedGame::addState(GameState* state) {
			m_states.push_back(state);
			m_from_state = state;
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

		void StateBasedGame::enterState(unsigned int id, ARK::State::Transition::Transition* leave, ARK::State::Transition::Transition* enter) {
			GameState* state = NULL;
			for (unsigned int i = 0; i < m_states.size(); i++) {
				if (m_states.at(i)->id() == id) {
					state = m_states.at(i);
					break;
				}
			}
			if (state == NULL) {
				ErrorDialog::createAndShow(StringUtil::append("State with id not found: ", id));
				exit(0);
			} else {
				enterState(state, leave, enter);
			}
		}
		void StateBasedGame::enterState(GameState* state, ARK::State::Transition::Transition* leave, ARK::State::Transition::Transition* enter) {

			m_from_state = m_current_state;
			//m_current_state = state;
			m_next_state = state;

			if (leave == NULL) { // || m_leaveTransition == NULL) {
				ARK2D::getLog()->v("StateBasedGame::enterState - enter was null");
				leave = new EmptyTransition();
			}
			if (enter == NULL) { // || m_enterTransition == NULL) {
				ARK2D::getLog()->v("StateBasedGame::enterState - leave was null");
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
				GameState* s = m_states.at(i);
				ARK2D::getLog()->i(StringUtil::append("initialising state: ", s->id()));
				s->init(container, this);
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
					m_from_state->leave(container, this, m_next_state);

					if (m_autoDeleteTransitions) { delete m_leaveTransition; }
					m_leaveTransition = NULL;

					if (m_enterTransition == NULL) {
						m_current_state = m_next_state;
						m_current_state->enter(container, this, m_from_state);
					} else {
						m_current_state = m_next_state;
						m_enterTransition->init(container, this, m_from_state, m_next_state);
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

					if (m_autoDeleteTransitions) { delete m_enterTransition; }
					m_enterTransition = NULL;
				} else {
					return;
				}
			}


			m_current_state->update(container, this, timer);

			//postUpdate(container, timer);
		}

		void StateBasedGame::preRender(GameContainer* container, Renderer* g) {
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
            #elif defined(ARK2D_IPHONE)
				if (container->getOrientation() == GameContainer::ORIENTATION_LANDSCAPE) {
					glPushMatrix();
					glTranslatef(container->m_platformSpecific.m_glView.bounds.size.width, 0.0f, 0.0f );
					glRotatef(90.0, 0.0, 0.0, 1.0);
				}
				
                g->pushMatrix();
                g->translate(container->getTranslateX(), container->getTranslateY());
                g->pushMatrix();
                g->scale(container->getScale(), container->getScale());
            #elif (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC))

               	/*glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				glLoadIdentity();
				g->setScissorTestEnabled(true);
               	g->scissor(
					container->getTranslateX(),
					container->getTranslateY(),
					container->getWidth() * container->getScaleX(), 
					container->getHeight() * container->getScaleY()
				)*/

				g->pushMatrix();
                g->translate(container->getTranslateX(), container->getTranslateY());
                g->pushMatrix();
                g->scale(container->getScale(), container->getScale());
            #else
                g->pushMatrix(); 
			#endif
		}
		void StateBasedGame::postRender(GameContainer* container, Renderer* g) {
			#if defined(ARK2D_IPHONE)
				g->popMatrix(); // pop scale
				g->popMatrix(); // pop translate
				if (container->getOrientation() == GameContainer::ORIENTATION_LANDSCAPE) {
					g->popMatrix(); // pop rotate
				}
			#elif (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC))
				g->popMatrix(); // pop scale
				g->popMatrix(); // pop translate

				g->drawScissorBoxes(); 
			#elif defined( ARK2D_ANDROID )
				g->popMatrix(); // pop scale
				g->popMatrix(); // pop translate

				g->drawScissorBoxes(); 
			#else
				g->popMatrix();
			#endif
		}
		void StateBasedGame::render(GameContainer* container, Renderer* g) {
			//preRender(container, g);


			// Render pre-enter transition
			if (m_leaveTransition != NULL) {
				m_leaveTransition->preRender(container, this, g);
			} else if (m_enterTransition != NULL) {
				m_enterTransition->preRender(container, this, g);
			}

			// Render Current State.
			if (m_current_state != NULL) {
				//ARK2D::getLog()->v("StateBasedGame::render - render current state");
				m_current_state->render(container, this, g);
			}

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

		void StateBasedGame::resize(GameContainer* container, int width, int height) 
		{
			Game::resize(container, width, height);
		}

		void StateBasedGame::pause() 
		{
			Game::pause();
		}
		void StateBasedGame::resume()
		{
			Game::resume();
		}


		void StateBasedGame::keyPressed(unsigned int key) {
			Game::keyPressed(key);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }
			
			m_current_state->keyPressed(key);
		}
		void StateBasedGame::keyReleased(unsigned int key) {
			Game::keyReleased(key);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			m_current_state->keyReleased(key);
		}
		void StateBasedGame::mouseMoved(int x, int y, int oldx, int oldy) {
			Game::mouseMoved(x, y, oldx, oldy);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			m_current_state->mouseMoved(x, y, oldx, oldy);
		}

		StateBasedGame::~StateBasedGame() {

		}
	}
}
