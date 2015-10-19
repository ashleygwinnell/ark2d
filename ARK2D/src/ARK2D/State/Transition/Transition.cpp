/*
 * Transition.cpp
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#include "Transition.h"
#include "../../Tween/Timeline.h"

 #include "../../Core/Game.h"
#include "../../Core/GameTimer.h"
#include "../../Graphics/Renderer.h"

#include "../GameState.h"
#include "../StateBasedGame.h"

namespace ARK {
	namespace State {
		namespace Transition {

			Transition::Transition(): 
				m_container(NULL), 
				m_from(NULL),
				m_to(NULL),
				m_easing(Easing::LINEAR),
				m_updatingFromState(false),
				m_updatingToState(false)
				{

			}

			void Transition::setEasing(unsigned int easing) {
				m_easing = easing;
			}
			unsigned int Transition::getEasing() {
				return m_easing;
			}

			void Transition::postEnter(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) { 

			}
			void Transition::postLeave(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {

			}

			void Transition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				if (m_updatingFromState && m_from != NULL) {
					m_from->update(container, game, timer);
				}
				if (m_updatingToState && m_to != NULL) {
					m_to->update(container, game, timer);	
				}
			}

			void Transition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				m_container = container;
				m_from = from;
				m_to = to;
				m_easing = Easing::LINEAR;
			}

			void Transition::setUpdatingStates(bool fromState, bool toState) {
				m_updatingFromState = fromState;
				m_updatingToState = toState;
			}

			Transition::~Transition() {

			}
		}
	}
}
