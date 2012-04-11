/*
 * Transition.cpp
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#include "Transition.h"
#include "../../Tween/Timeline.h"

namespace ARK {
	namespace State {
		namespace Transition {

			Transition::Transition() {

			}

			void Transition::setEasing(unsigned int easing) {
				m_easing = easing;
			}
			unsigned int Transition::getEasing() {
				return m_easing;
			}

			void Transition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				m_container = container;
				m_from = from;
				m_to = to;
				m_easing = Easing::LINEAR;
			}

			Transition::~Transition() {

			}
		}
	}
}
